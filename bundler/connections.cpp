#include <QtCore/QCoreApplication>
#include "connections.h"
#include <qfile.h>
#include <qtextstream.h>

#include <QtDebug>

#include <QStringList>

#include "qmath.h"

Connections::Connections(QString nname, QString ename)
{
    params();
    prefix = nname;
    QFile n(nname);
    qDebug() << nname;
    if (!n.open(QIODevice::ReadOnly)) qDebug("nodes unreadable");
    QTextStream ns(&n);
    QString nl;

    while(!ns.atEnd()) {
        nl = ns.readLine();

        QStringList vals = nl.split(" ", QString::SkipEmptyParts);
        QVector3D* anode;
        //x,y,z
        anode = new QVector3D(((QString)(vals.at(0))).toFloat(),
                              ((QString)(vals.at(1))).toFloat(),
                              ((QString)(vals.at(2))).toFloat());
        qDebug() << anode->x() << anode->y() << anode->z();
        nodes << *anode;

    }
    n.close();
    qDebug() << "nodes read";

    QFile e(ename);
    if (!e.open(QIODevice::ReadOnly)) qDebug("edges unreadable");
    QTextStream es(&e);
    QString el;
    while(!es.atEnd()) {
        int f;
        int t;
        el = es.readLine();

        QStringList evals = el.split(" ",QString::SkipEmptyParts);
        f = ((QString)(evals.at(0))).toInt();
        t = ((QString)(evals.at(1))).toInt();

        Edge* aedge;
        aedge = new Edge(nodes.at(f), nodes.at(t));
        qDebug() << f << t;
        edges << aedge;
    }
    e.close();

    qDebug() << edges.length() << " edges...";
}

Connections::Connections(QString fib){
    //TODO: Das hier so umbiegen, dass ich Gabys Daten laden kann...
    params();
    prefix = fib;
    QFile n(fib);
    if (!n.open(QIODevice::ReadOnly)) qDebug() << "vtk unreadable: " << fib;
    QTextStream ns(&n);
    QString nl;
    QDataStream ins(&n);
    ins.setByteOrder(QDataStream::BigEndian);
    ins.setFloatingPointPrecision(QDataStream::SinglePrecision);
    nl = ns.readLine(); //skip first lines;
    nl = ns.readLine(); //TODO: Other types of stuff...
    nl = ns.readLine();
    nl = ns.readLine();
    nl = ns.readLine();
    //ns.pos();
    qDebug() << ns.pos(); //TODO: Das hier sollte nichts ausmachen, tuts aber...
    qDebug() << nl;
    QStringList vals = nl.split(" ");
    float np = ((QString)(vals.at(1))).toInt();
    qDebug() << np;
    for (int i = 0; i < np; i++) {
        QVector3D* anode;
        float x,y,z;
        ins >> x;
        ins >> y;
        ins >> z;
        anode = new QVector3D(x,y,z);
        nodes << *anode;
    }
    qDebug() << ns.pos(); //TODO: WTF, siehe oben?
    ns.seek(n.pos() + np*3*4 + 1); //Textstream aufs Zeichen nach den Punkten...
    qDebug() << ns.pos();
    nl = ns.readLine();
    //qDebug() << nl;
    vals = nl.split(" ");
    float ncons = ((QString)(vals.at(1))).toInt();

    qDebug() << ns.pos();
    for (int i = 0; i < ncons; i++) {
        qint32 numpoints;
        ins >> numpoints;
        //qDebug() << numpoints;
        qint32 ps[numpoints];
        for (int pn = 0; pn < numpoints; pn++){
            ins >> ps[pn];
        }
        Edge* aedge;
        aedge = new Edge(nodes.at(ps[0]), nodes.at(ps[numpoints-1]));
        aedge->points.removeLast();
        for (int pn = 1; pn < numpoints; pn++){
            aedge->points << nodes.at(ps[pn]);
        }
        edges << aedge;
    }
    n.close();
    qDebug() << "nodes read";
}

void Connections::params() {
    c_thr = 0.8;
    start_i = 10;
    numcycles = 10;
    bell = 5;
    smooth = 3;
}

void Connections::subdivide(int newp) {
    for (int i = 0; i < edges.size(); ++i) {
        Edge* e = edges.at(i);;
        e->subdivide(newp);
    }
}

void Connections::attract(){

    //for all edges...
    #pragma omp parallel for
    for (int ie = 0; ie < edges.size(); ++ie) {
        Edge* e = edges.at(ie);
        //for every point...
        for (int i=1; i<e->points.length()-1; i++){
            QVector3D p = e->points.at(i);
            double fsum = 0;
            QVector3D f(0,0,0);
            //for all attracting points...
            for (int ef=0; ef<edges.length(); ef++){
                float c = comp(ie,ef);
                if (c > c_thr) {
                    QVector3D pe;
                    if (e->flip(edges.at(ef))){
                        pe = edges.at(ef)->points.at(i);
                    } else {
                        pe = edges.at(ef)->points.at((edges.at(ef)->points.length()-1)-i);
                    }

                    float de = (pe-p).length();
                    double weight = qExp(-(de*de)/(2*bell*bell));
                    fsum += weight;
                    f += weight*pe;
                    QVector3D df;
                }
            }

            f /= fsum;
            QVector3D force = (f-p);
            e->forces.replace(i,force);
        }
    }
    for (int e=0; e<edges.size(); e++){
        edges.at(e)->applyForces();
    }
}

void Connections::fullAttract() {

    calcComps();

    double spfac = 1.3;
    double spnow = 1;
    int i = start_i;
    for (int cycle = 0; cycle < numcycles; cycle++){
        int sps = qRound(spnow);
        subdivide(sps);
        qDebug() << "starting " << i << " iterations with c_thr:" << c_thr << "segments: " << edges.first()->points.length()-1;
        for (int j = 0; j<i; j++){
            //qDebug() << j;
            attract();
        }
        i--;
        spnow *= spfac;
    }
    //for further subdivision without attraction
    for (int i=1; i<smooth; i++){
        subdivide(qRound(spnow)+i);
        qDebug() << "number of subd. points: " << qRound(spnow)+i;
    }
}

void Connections::calcComps(){
    comps = new float[edges.size()*edges.size()];

    #pragma omp parallel for num_threads (7)
    for (int i=0; i<edges.length(); i++){
        for (int j=0; j<edges.length(); j++){
            if (i==j) {
                comps[i+edges.size()*j]=0;
            } else {
                Edge* ei = edges.at(i);
                Edge* ej = edges.at(j);
                //calculate compatibility btw. edge i and j
                //angle
                double angle_comp;
                if (!ei->flip(ej)) {
                    angle_comp = QVector3D::dotProduct(ei->fn-ei->tn,ej->tn-ej->fn);
                } else {
                    angle_comp = QVector3D::dotProduct(ei->fn-ei->tn,ej->fn-ej->tn);
                }
                angle_comp /= ei->length()*ej->length();
                //length
                double lavg = (ei->length()+ej->length())/2.0;
                double l_comp = 2 / ((lavg/qMin(ei->length(),ej->length())) + (qMax(ei->length(),ej->length())/lavg));
                //position
                QVector3D mi = (ei->fn+ei->tn)/2;
                QVector3D mj = (ej->fn+ej->tn)/2;
                double p_comp = lavg / (lavg + (mi-mj).length());
                //visibility
                if (angle_comp * l_comp * p_comp > 0.9) {
                    double vis_comp = qMin(vis_c(ei,ej),vis_c(ej,ei));
                    comps[i+edges.size()*j] = angle_comp * l_comp * p_comp * vis_comp;
                } else {
                    comps[i+edges.size()*j] = angle_comp * l_comp * p_comp;
                }

            }
        }
    }
}

double Connections::vis_c(Edge* ep, Edge* eq) {
    QVector3D i0 = proj(ep->fn,ep->tn, eq->fn);
    QVector3D i1 = proj(ep->fn,ep->tn, eq->tn);
    QVector3D im = (i0+i1)/2;
    QVector3D pm = (ep->fn+ep->tn)/2;

    return qMax(1-2*(pm-im).length()/(i0-i1).length(),0.0);
}

QVector3D Connections::proj(QVector3D a, QVector3D b, QVector3D p) {
    QVector3D ba = b-a;
    QVector3D pa = p-a;
    return a + ba*QVector3D::dotProduct(ba,pa) / ba.lengthSquared();
}

float Connections::comp(int i, int j) {
    return comps[i+edges.size()*j];
}

void Connections::writeVTK(){
    qDebug() << "writing file";

    QFile file("brain_dti.vtk");
    if (!file.open(QIODevice::WriteOnly)) qDebug() << "Error opening file for writing";
    QTextStream out(&file);

    int n = edges.size();
    int m = edges.at(0)->points.size();

    out << "# vtk DataFile Version 3.0" << endl;
    out << "I am a header! Yay!" << endl;
    out << "ASCII" << endl;
    out << "DATASET POLYDATA" << endl;
    out << "POINTS " << m*n << " float" << endl;

    for (int e = 0; e<n; e++){
        Edge* ed = edges.at(e);
        for (int p=0; p<ed->points.size(); p++){
            QVector3D po = ed->points.at(p);
            out << (float)po.x() << " " << (float)po.y()  << " " << (float)po.z() << endl;
        }
    }

    out << "LINES " << n << " " << n*(m+1) << endl;
    int i = 0;
    for (int e = 0; e<n; e++){
        out << m;
        for (int p=0; p<m; p++){
            out << " " << i++;
        }
        out << endl;
    }

    file.close();
    qDebug() << "file written";
}

void Connections::writeBinaryVTK(){
    qDebug() << "writing binary vtk file";
    writeBinaryVTK(name());
}

void Connections::writeBinaryVTK(QString name){

    qDebug() << "writing: " << name << ".fib";
    QFile file(name+".fib");
    if (!file.open(QIODevice::WriteOnly)) qDebug() << "error opening file for writing";
    QDataStream out(&file);
    QTextStream outt(&file);

    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    int n = edges.size();
    int m = edges.at(0)->points.size();

    outt << "# vtk DataFile Version 3.0" << endl;
    outt << "I am a header! Yay!" << endl;
    outt << "BINARY" << endl;
    outt << "DATASET POLYDATA" << endl;
    outt << "POINTS " << m*n << " float" << endl;

    for (int e = 0; e<n; e++){
        Edge* ed = edges.at(e);
        for (int p=0; p<ed->points.size(); p++){
            QVector3D po = ed->points.at(p);
            out << (float)po.x() << (float)po.y() << (float)po.z();
        }
    }
    outt << endl;

    outt << "LINES " << n << " " << n*(m+1) << endl;
    int i = 0;
    for (int e = 0; e<n; e++){
        out << m;
        for (int p=0; p<m; p++){
            out << i++;
        }
    }
    outt << endl;

    file.close();
    qDebug() << "file written";
}


void Connections::writeSegments(){
    qDebug() << "writing segments file";

    QFile file("segments");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    int n = edges.size();

    for (int e = 0; e<n; e++){
        Edge* ed = edges.at(e);
        for (int p=0; p<ed->points.size(); p++){
            QVector3D po = ed->points.at(p);
            out << (float)po.x() << " " << (float)po.y()  << " " << (float)po.z() << " " << e << endl;
        }
    }

    file.close();
    qDebug() << "file written";
}

QString Connections::name() {
    return prefix +
            "_c_thr" + QString::number(c_thr,'f',4) +
            "_start_i" + QString("%1").arg(start_i,4,10,QLatin1Char('0')) +
            "_numcycles" + QString("%1").arg(numcycles,2,10,QLatin1Char('0') );
}
