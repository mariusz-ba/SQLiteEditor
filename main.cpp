#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->setStyleSheet(QString("QToolButton {background-color: transparent;}"
                                "QToolButton:hover {"
                                    "background: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 rgba(255,255,255,255), stop: 0.5 #ebebeb, stop:1.0 rgba(255,255,255,255));"
                                    "border: 1px solid rgb(193, 193, 193);"
                                    "border-radius: 3px;"
                                "}"));

    MainWindow w;
    w.show();

    return a.exec();
}
