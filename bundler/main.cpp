#include <QtCore/QCoreApplication>

#include <QtDebug>
#include <QStringList>
#include "connections.h"

QString arg(QString argname) {
    int nodespos = qApp->arguments().indexOf(QRegExp("-"+argname+"*"));
    if (nodespos!=-1) {
        return qApp->arguments().at(nodespos+1);
    } else {
        return "";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (qApp->arguments().indexOf(QRegExp("-help"))!=-1 || qApp->arguments().length()==1) {
            qDebug() << "bundler (-nodes <nodes> -cons <connections> / -fib <.fib-file>) [-c_thr <compatibility threshold>] [-start_i <iterations in 1st cycle>] [-numcycles <number of cycles>]";
            return 1;
    }

    qDebug() << "nodesname: " + arg("nodes");
    qDebug() << "consname: " + arg("cons");
    Connections* cons;
    if (arg("cons")!="") {
        cons = new Connections(arg("nodes"),arg("cons"));
    } else if (arg("fib")!="") {
        cons = new Connections(arg("fib"));
    } else {
        qDebug() << "no dataset given";
        return 0;
    }

    if (arg("c_thr")!="") cons->c_thr = arg("c_thr").toDouble();
    if (arg("start_i")!="") cons->start_i = arg("start_i").toInt();
    if (arg("numcycles")!="") cons->numcycles = arg("numcycles").toInt();

    qDebug() << cons->name();

    cons->fullAttract();
    if (arg("out")!="") {
        cons->writeBinaryVTK(arg("out"));
    } else {
        cons->writeBinaryVTK();
    }
    return 1;
}
