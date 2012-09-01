#include "artificialconnections.h"

float frand(){
    return qrand()/(float)RAND_MAX;
}

ArtificialConnections::ArtificialConnections()
{
    int m = 30;

    for (int i = 0; i<m*10; i++){
        QVector3D* anode2 = new QVector3D(0+frand()/10,frand(),frand());
        QVector3D* anode1 = new QVector3D(10+frand()/10,frand(),frand());
        nodes << *anode1 << *anode2;
        Edge* aedge = new Edge(*anode1,*anode2);
        edges << aedge;
    }

    for (int i = 0; i<m*1; i++){
        QVector3D* anode1 = new QVector3D(0+frand()/10,2+frand(),frand());
        QVector3D* anode2 = new QVector3D(10+frand()/10,3+frand(),frand());
        nodes << *anode1 << *anode2;
        Edge* aedge = new Edge(*anode1,*anode2);
        edges << aedge;
    }

    for (int i = 0; i<m*10; i++){
        QVector3D* anode1 = new QVector3D(0+frand()/10,5+frand(),frand());
        QVector3D* anode2 = new QVector3D(10+frand()/10,5+frand(),frand());
        nodes << *anode1 << *anode2;
        Edge* aedge = new Edge(*anode1,*anode2);
        edges << aedge;
    }

    createPrims();
    calculateBounds();
}
