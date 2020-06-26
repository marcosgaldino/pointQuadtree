#include "dialograndomwindow.h"
#include "ui_dialograndomwindow.h"
#include "quadtree.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

DialogRandomWindow::DialogRandomWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRandomWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

DialogRandomWindow::~DialogRandomWindow()
{
    delete ui;
}

int DialogRandomWindow::on_pushButton_OkRand_clicked(){
    int numero;
    numero = ui->spinBoxNumNos->value();
    arvore.gera_quadtree_randomica(numero);
    return numero;
}
