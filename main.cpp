#include <QtGui>
#include <stdlib.h>
#include "cntrlr/eventcontrol.h"
#include "view/virtualboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
#ifdef ARM
    app.setGraphicsSystem("raster");
    QFont f_24("helvetica",24, QFont::Bold);
    qApp->setFont(f_24);
    EventControl e;
    e.start();
#endif
#ifndef ARM
    virtualBoard vb;
    vb.show();
#endif
    return app.exec();
}

