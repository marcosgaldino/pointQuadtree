#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QSplashScreen>
#include <QTimer>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/imagens/vpq.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QObject::trUtf8("Versão 1.0\n\n\n\nCarregando..."), Qt::AlignRight|Qt::AlignBottom, Qt::black);

    MainWindow w;
    QTimer::singleShot(3000, &splash, SLOT(close()));
    QTimer::singleShot(3000, &w, SLOT(show()));

    return a.exec();
}
