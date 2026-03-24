#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   QFile styleFile(":/styles/darkstyle.qss");
if (styleFile.open(QFile::ReadOnly)) {
    qApp->setStyleSheet(styleFile.readAll());
} else {
    qWarning() << "Nie mogę załadować stylu!";
}

    MainWindow w;
    w.show();
    return a.exec();
}

