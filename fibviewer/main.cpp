#include <QtGui/QApplication>
#include "mainwindow.h"

#include "QDebug"
#include "glwidget.h"

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
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Fibviewer");
    w.show();

    if (qApp->arguments().indexOf(QRegExp("-writefib"))!=-1 || qApp->arguments().indexOf(QRegExp("-screenshot"))!=-1){
        return 0;
    }

    return a.exec();
}
