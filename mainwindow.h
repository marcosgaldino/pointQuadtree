#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFile>
#include "quadtree.h"
#include "dialograndomwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        //Funções
        void limitarValorMinX2BuscaVizinhanca(int);
        void limitarValorMinY2BuscaVizinhanca(int);
        void iniciaGraphicsScenes();
        void imprimeParticionamento();
        void imprimeHierarquia(std::string=" ");
        void imprime_retangulo();
        void imprime_circunferencia();
        void desabilitaBotoes();
        void habilitaBotoes();
        //Menu
        void on_actionNova_triggered();
        void on_actionRandomica_triggered();
        void on_actionCoordenada_UTM_triggered();
        void on_actionCoordenada_Cartesiana_triggered();
        void on_actionSair_triggered();
        void on_actionInserir_triggered();
        void on_actionExcluir_triggered();
        void on_actionBuscaNo_triggered();
        void on_actionBuscaRegiao_triggered();
        void on_actionSalvar_triggered();
        void on_actionSalvar_Como_triggered();
        void on_actionApagar_triggered();
        void on_actionBuscaVizinhanca_triggered();
        void on_actionVisualizador_triggered();
        //Botões
        void on_pushButtonLimparInserir_clicked();
        void on_pushButtonLimparExcluir_clicked();
        void on_pushButtonLimpaBuscaNo_clicked();
        void on_pushButtonLimpaBuscaRegiao_clicked();
        void on_pushButtonLimpaBuscaViz_clicked();
        void on_pushButtonInserir_clicked();
        void on_pushButtonExcluir_clicked();
        void on_pushButtonBuscaNo_clicked();
        void on_pushButtonBuscaRegiao_clicked();
        void on_pushButtonBuscaViz_clicked();
        void on_pushButtonMenosBotoesHieraquia_clicked();
        void on_pushButtonMaisBotoesHieraquia_clicked();
        void on_pushButtonCemBotoesHieraquia_clicked();
        void on_spinBoxRaioBuscaViz_valueChanged(int arg1);
        void on_spinBoxCordXBuscaViz_valueChanged(int arg1);
        void on_spinBoxCordYBuscaViz_valueChanged(int arg1);
        void on_spinBoxCordX1BuscaRegiao_valueChanged(int arg1);
        void on_spinBoxCordY1BuscaRegiao_valueChanged(int arg1);
        void on_spinBoxCordX2BuscaRegiao_valueChanged(int arg1);
        void on_spinBoxCordY2BuscaRegiao_valueChanged(int arg1);
    private:
        Ui::MainWindow *ui;
        quadtree arvore;
        QGraphicsScene *cenaParticionamento;
        QGraphicsScene *cenaHierarquia;
        QString path;
        QString verificaCodigo(QFile*,QString);
};

#endif // MAINWINDOW_H
