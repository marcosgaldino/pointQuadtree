#ifndef DIALOGRANDOMWINDOW_H
#define DIALOGRANDOMWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QFile>
#include "quadtree.h"
#include "mainwindow.h"

namespace Ui {
    class DialogRandomWindow;
}

class DialogRandomWindow : public QDialog
{
    Q_OBJECT

    public:
        explicit DialogRandomWindow(QWidget *parent = 0);
        ~DialogRandomWindow();
        int on_pushButton_OkRand_clicked();

    private:
        Ui::DialogRandomWindow *ui;
        quadtree arvore;
};

#endif // DIALOGRANDOMWINDOW_H
