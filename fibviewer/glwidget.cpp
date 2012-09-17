#include <QtGui>
#include <QtOpenGL>

#include <QtDebug>

#include "glwidget.h"
#include <QMatrix4x4>

#include "artificialconnections.h"

QString GLWidget::arg(QString argname) {
    int nodespos = qApp->arguments().indexOf(QRegExp("-"+argname+"*"));
    if (nodespos!=-1) {
        return qApp->arguments().at(nodespos+1);
    } else {
        return "";
    }
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    resize(800,800);

    QString filename = qApp->arguments().value(1,"");
    qDebug() << "arglength: " << qApp->arguments().length();
    if (filename == "artificial" || qApp->arguments().length() == 1) {
        qDebug() << "creating new artificial data";
        cons = new ArtificialConnections();

    } else if (arg("nodes")!=""){
        qDebug() << arg("nodes");
        cons = new Connections(arg("nodes"), arg("cons"));
    } else {
        qDebug() << filename;
        cons = new Connections(filename);
    }

    view = new GLfloat[16];
    stuffAlpha = 0.99;
    QVector3D size = cons->max-cons->min;
    float largest = qMax(size.x(),size.y());
    scale = (1/largest)*0.95;
    bg = 1;
    p1 = true;
    p2 = true;

    if (qApp->arguments().indexOf(QRegExp("-writefib"))!=-1) cons->writeBinaryVTK(filename+".fib");
    if (qApp->arguments().indexOf(QRegExp("-writeobj"))!=-1) cons->writeOBJ(filename+".obj");

    if (qApp->arguments().indexOf(QRegExp("-screenshot"))!=-1) screenshot(filename+".png");
    if (qApp->arguments().indexOf(QRegExp("-csv"))!=-1) cons->writeCSVs();
    setFocus();

}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    selected = new QVector3D(0,0,0);

    glEnable(GL_RESCALE_NORMAL);

    //glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    static GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    static GLfloat specular[] = {0.5f, 0.5f, 0.5f , 1.0f};
    glMateriali(GL_FRONT, GL_SHININESS, 1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    static GLfloat lightPosition[4] = { 10000, 10000, 50000, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glGetFloatv(GL_MODELVIEW_MATRIX, view);
}

void GLWidget::paintGL()
{
    glClearColor(bg,bg,bg,1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glScaled(scale,scale,scale);

    glMultMatrixf(view);

    glTranslatef(cons->piv.x(),cons->piv.y(),cons->piv.z());

    glEnable(GL_DEPTH_TEST);

    glColor4f(1,0,0,stuffAlpha);
    cons->paintGL();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    ar = (float)width/(float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (ar>1){
        glOrtho(ar*-0.5, ar*+0.5, -0.5, +0.5, -1000, 1000);
    } else {
        glOrtho(-0.5, +0.5, -0.5/ar, +0.5/ar, -1000, 1000);
    }
    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    select(event);
}

bool GLWidget::select(QMouseEvent *event){
    if (event->modifiers() && Qt::ControlModifier){
        //select Point...
        double modelview[16], projection[16];
        int viewport[4];
        float z;
        GLdouble objx, objy, objz;

        //get the modelview matrix
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        //get the projection matrix
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        //get the viewport
        glGetIntegerv(GL_VIEWPORT, viewport);

        //Read the window z co-ordinate
        //(the z value on that point in unit cube)
        glReadPixels(event->x(), viewport[3] - event->y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

        //Unproject the window co-ordinates to
        //find the world co-ordinates
        gluUnProject(event->x(), viewport[3] - event->y(), z, modelview, projection, viewport, &objx, &objy, &objz);

        qDebug() << objx << " , " << objy << " , " << objz << "\n";

        selected->setX(objx);
        selected->setY(objy);
        selected->setZ(objz);
        cons->selectForPoint(selected);
        return true;
    } else {
        return false;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    if (!select(event)){

        glLoadIdentity();

        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
            QMatrix4x4 mat(view[0],view[4],view[8],view[12],view[1],view[5],view[9],view[13],view[2],view[6],view[10],view[14],view[3],view[7],view[11],view[15]);
            QVector3D orig(0, 0, 0);
            QVector3D m = mat.map(orig);
            glTranslatef(m.x(), m.y(), m.z());
            glRotatef(qSqrt(dx*dx+dy*dy)/2.0, dy, dx, 0);
            glTranslatef(-m.x(), -m.y(), -m.z());
        } else if (event->buttons() & Qt::RightButton) {
            glTranslatef(dx/(float)width()*ar/scale, -dy/(float)height()/scale, 0);
        }
        lastPos = event->pos();
        glPushMatrix();
        glMultMatrixf(view);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        glPopMatrix();
    }

    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    cons->sortCons();
    updateGL();
}

void GLWidget::wheelEvent (QWheelEvent *event)
{
    int d = event->delta();
    qDebug() << "scale" << scale;
    scale *= 1.0-d/1200.0;
    updateGL();
}

void GLWidget::screenshot(QString name){
    qDebug() << "screenshot";
    QPixmap map = this->renderPixmap(this->width(),this->height(),false);
    QPainter p(&map);
    map.save(name);
}

void GLWidget::save(){
    qDebug() << "start save";
    QString name = "screen.png";
    screenshot(name);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    qDebug() << "key:" << event->key();
    if (event->key() == Qt::Key_C) {
        if (bg == 1) {
            bg = 0;
        } else {
            bg =1;
        }
    }
    if (event->key() == Qt::Key_1) p1 = !p1;
    if (event->key() == Qt::Key_2) p2 = !p2;
    updateGL();
}

void GLWidget::stuffSliderChanged(int i){
    stuffAlpha = i/(float)100;
    qDebug() << stuffAlpha;
    updateGL();
}
