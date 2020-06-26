#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialograndomwindow.h"
#include "dialograndomwindow.h"
#include "dialogsobre.h"
#include "quadtree.h"
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QPalette>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <cstdlib>
#include <iostream>
#include <QMatrix>
#include <QErrorMessage>
#include <QProgressDialog>
#include <QTextCodec>
#include <fstream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::connect(ui->spinBoxCordX1BuscaRegiao,SIGNAL(valueChanged(int)),this,SLOT(limitarValorMinX2BuscaVizinhanca(int)));//limita o valor min de x2 ao valor atual de x1
    QApplication::connect(ui->spinBoxCordY1BuscaRegiao,SIGNAL(valueChanged(int)),this,SLOT(limitarValorMinY2BuscaVizinhanca(int)));//limita o valor min de y2 ao valor atual de y1
    iniciaGraphicsScenes();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::limitarValorMinX2BuscaVizinhanca(int limiteMin){
    ui->horizontalSliderCordX2BuscaRegiao->setMinimum(limiteMin);
    ui->spinBoxCordX2BuscaRegiao->setMinimum(limiteMin);
}

void MainWindow::limitarValorMinY2BuscaVizinhanca(int limiteMin){
    ui->horizontalSliderCordY2BuscaRegiao->setMinimum(limiteMin);
    ui->spinBoxCordY2BuscaRegiao->setMinimum(limiteMin);
}

//Funções
void MainWindow::iniciaGraphicsScenes(){
    cenaParticionamento = new QGraphicsScene();
    cenaParticionamento->setSceneRect(0,0,250,250);
    imprimeParticionamento();
    QTransform t = ui->graphicsViewParticionamento->transform().rotate(180,Qt::XAxis);
    ui->graphicsViewParticionamento->setTransform(t);
    ui->graphicsViewParticionamento->setScene(cenaParticionamento);

    cenaHierarquia = new QGraphicsScene();
    ui->graphicsViewHierarquia->setScene(cenaHierarquia);
    ui->graphicsViewHierarquia->setDragMode(QGraphicsView::ScrollHandDrag);
    imprimeHierarquia();
}

void MainWindow::imprimeParticionamento(){
    QProgressDialog progresso;
    progresso.setWindowTitle("Carregando particionamento do espaço...");
    progresso.setMinimumDuration(3000);
    progresso.setCancelButton(0);
    progresso.setWindowModality(Qt::WindowModal);
    progresso.setFixedSize(350,80);
    progresso.setLabelText("Gerando imagem");
    progresso.setValue(0);
    progresso.show();

    cenaParticionamento->clear();
    cenaParticionamento->addRect(cenaParticionamento->sceneRect().x()-1,cenaParticionamento->sceneRect().y()-1,cenaParticionamento->sceneRect().right()+2,cenaParticionamento->sceneRect().bottom()+2);
    progresso.setValue(10);
    QGraphicsSimpleTextItem *xY = new QGraphicsSimpleTextItem("(0,0)");
    QGraphicsSimpleTextItem *rightBotton = new QGraphicsSimpleTextItem("(250,250)");
    progresso.setValue(20);
    xY->setPos(0,-2);
    rightBotton->setPos(cenaParticionamento->sceneRect().right()-60,cenaParticionamento->sceneRect().bottom()+20);
    progresso.setValue(30);
    QTransform t;
    t = xY->transform().rotate(180,Qt::XAxis);
    xY->setTransform(t);

    t = rightBotton->transform().rotate(180,Qt::XAxis);
    rightBotton->setTransform(t);
    progresso.setValue(40);
    cenaParticionamento->addItem(xY);
    cenaParticionamento->addItem(rightBotton);

    progresso.setValue(50);
    arvore.imprimeParticionamento(cenaParticionamento);
    progresso.setValue(100);
    progresso.close();
}

void MainWindow::imprimeHierarquia(std::string busca){
    cenaHierarquia->clear();
    arvore.imprimeHierarquia(cenaHierarquia,busca);
}

void MainWindow::imprime_retangulo(){
    imprimeParticionamento();
    QPen pen(Qt::blue);
    QGraphicsRectItem * retangulo = cenaParticionamento->addRect(ui->spinBoxCordX1BuscaRegiao->value(),ui->spinBoxCordY1BuscaRegiao->value(),ui->spinBoxCordX2BuscaRegiao->value()-ui->spinBoxCordX1BuscaRegiao->value(),ui->spinBoxCordY2BuscaRegiao->value()-ui->spinBoxCordY1BuscaRegiao->value(),pen);
    retangulo->setZValue(3);
}

void MainWindow::imprime_circunferencia(){
    imprimeParticionamento();
    QPen pen(Qt::blue);
    QGraphicsEllipseItem * elipse = cenaParticionamento->addEllipse(ui->spinBoxCordXBuscaViz->value()-ui->spinBoxRaioBuscaViz->value(),ui->spinBoxCordYBuscaViz->value()-ui->spinBoxRaioBuscaViz->value(),2*ui->spinBoxRaioBuscaViz->value(),2*ui->spinBoxRaioBuscaViz->value(),pen);
    elipse->setZValue(3);
}

void MainWindow::desabilitaBotoes(){
    ui->pushButtonBuscaNo->setEnabled(false);
    ui->pushButtonBuscaRegiao->setEnabled(false);
    ui->pushButtonBuscaViz->setEnabled(false);
    ui->pushButtonExcluir->setEnabled(false);
    ui->pushButtonInserir->setEnabled(false);
    ui->pushButtonLimpaBuscaNo->setEnabled(false);
    ui->pushButtonLimpaBuscaRegiao->setEnabled(false);
    ui->pushButtonLimpaBuscaViz->setEnabled(false);
    ui->pushButtonLimparExcluir->setEnabled(false);
    ui->pushButtonLimparInserir->setEnabled(false);
    ui->menuArvore->setEnabled(false);
}

void MainWindow::habilitaBotoes(){
    ui->pushButtonBuscaNo->setEnabled(true);
    ui->pushButtonBuscaRegiao->setEnabled(true);
    ui->pushButtonBuscaViz->setEnabled(true);
    ui->pushButtonExcluir->setEnabled(true);
    ui->pushButtonInserir->setEnabled(true);
    ui->pushButtonLimpaBuscaNo->setEnabled(true);
    ui->pushButtonLimpaBuscaRegiao->setEnabled(true);
    ui->pushButtonLimpaBuscaViz->setEnabled(true);
    ui->pushButtonLimparExcluir->setEnabled(true);
    ui->pushButtonLimparInserir->setEnabled(true);
    ui->menuArvore->setEnabled(true);
}

QString MainWindow::verificaCodigo(QFile* file,QString tipo){
    QString retorno;
    QTextStream erro(&retorno);
    QString nome;
    QString x;
    QString y;
    float fx;
    float fy;
    bool ok;
    int linha = 1;
    QTextStream stream(file);
    while (!stream.atEnd()){
        QString line = stream.readLine();
        if (!line.isEmpty()){
            QStringList field = line.split("\t");
            if (field.length() == 3){
                nome = field[0];
                if(nome.isEmpty())
                    erro << "Linha " << linha << QString::fromUtf8(": Nome nulo.") << "\n";
                x = field[1];
                fx = x.toFloat(&ok);
                if(!ok){
                    erro << "Linha " << linha << QString::fromUtf8(": Valor do parâmetro x inválido.") << "\n";
                } else {
                    if(tipo == "cartesiana" && (fx < 0 || fx > 250)){
                        erro << "Linha " << linha << QString::fromUtf8(": Valor do parâmetro x fora da faixa permitida (0 a 250).") << "\n";
                    } else if(tipo == "utm" && (fx < -72.896f || fx > -32.411f)){
                        erro << "Linha " << linha << QString::fromUtf8(": Valor do parâmetro x fora da faixa permitida (-72.896 a -32.411).") << "\n";
                    }
                }
                y = field[2];
                fy = y.toFloat(&ok);
                if(!ok){
                    erro << "Linha " << linha << QString::fromUtf8(": Valor do parâmetro y inválido.") << "\n";
                } else {
                    if(tipo == "cartesiana" && (fy < 0 || fy > 250)){
                        erro << "Linha " << linha << QString::fromUtf8(": Valor do parâmetro y fora da faixa permitida (0 a 250).") << "\n";
                    } else if(tipo == "utm" && (fy < -33.691f || fy > 4.596f)){
                        erro << "Linha " << linha << QString::fromUtf8(": Valor do parâmetro y fora da faixa permitida (-33.691 a 4.596).") << "\n";
                    }
                }
            }else if(field.length()<3){
                erro << "Linha " << linha << QString::fromUtf8(": Quantidade de parâmetros abaixo do permitido.") << "\n";
            }else{
                erro << "Linha " << linha << QString::fromUtf8(": Quantidade de parâmetros ou tabs acima do permitido.") << "\n";
            }
        }
        linha++;
    }
    return retorno;
}

//Menu
void MainWindow::on_actionNova_triggered(){
    path.clear();
    ui->actionSalvar_Como->setEnabled(false);
    arvore.apagar_arvore();
    delete cenaHierarquia;
    cenaHierarquia = new QGraphicsScene();
    ui->graphicsViewHierarquia->setScene(cenaHierarquia);
    imprimeParticionamento();
    imprimeHierarquia();
    setWindowTitle("Visualizador de Point Quadtree");
}

void MainWindow::on_actionRandomica_triggered(){
    DialogRandomWindow d;
    d.setModal(true);
    d.exec();
    int numero_nos;
    numero_nos = d.on_pushButton_OkRand_clicked();
    if(numero_nos!= 0){
        try{
            arvore.gera_quadtree_randomica(numero_nos);
            QString pathTemp;
            pathTemp = "../arquivos/random001.qdt";
            QFile arquivo(pathTemp);
            arquivo.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&arquivo);
            QString nome;
            QString x;
            QString y;
            arvore.apagar_arvore();
            while (!in.atEnd()){
                QString line = in.readLine();
                if (!line.isEmpty()){
                    QStringList field = line.split("\t");
                    nome = field[0];
                    x = field[1];
                    y = field[2];
                    arvore.insere_no(x.toFloat(),y.toFloat(),nome.toStdString());
                }
            }
            setWindowTitle(arquivo.fileName().section('/',-1,-1) + " - Visualizador de Point Quadtree");
            arquivo.close();
            path=pathTemp;
            delete cenaHierarquia;
            cenaHierarquia = new QGraphicsScene();
            ui->graphicsViewHierarquia->setScene(cenaHierarquia);
            imprimeParticionamento();
            imprimeHierarquia();
        }catch(QString erro){
            QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
        }catch(std::string erroNoCodigo){
            QMessageBox erro(QMessageBox::Warning,"Erro no código de entrada:","Foram encontrados erros no código de entrada.\t\nClique em \"Mostrar Detalhes...\" para visualizar:",QMessageBox::Ok,this);
            erro.setInformativeText("Modelo: nome(tab)x(tab)y\nEx.: Abcd\t10\t20");
            erro.setDetailedText(QString::fromStdString(erroNoCodigo));
            erro.exec();
        }
    }
}

void MainWindow::on_actionVisualizador_triggered(){
    DialogSobre s;
    s.exec();
}

void MainWindow::on_actionCoordenada_UTM_triggered(){
    try{
        QString pathTemp;
        QFileDialog abrir(this,"Abrir coordenadas UTM:","","*.utm");
        abrir.setAcceptMode(QFileDialog::AcceptOpen);
        if(abrir.exec())
        {
            pathTemp=abrir.selectedFiles()[0];
            QFile testeCodigo(pathTemp);
                if(testeCodigo.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QString codigoOk = verificaCodigo(&testeCodigo,"utm");
                    if (!codigoOk.isEmpty())
                        throw codigoOk.toStdString();
                }else
                    throw testeCodigo.errorString();
                QFile file(pathTemp);
                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream in(&file);
                    QString nome;
                    QString x;
                    QString y;
                    arvore.apagar_arvore();
                    while (!in.atEnd()) {
                        QString line = in.readLine();
                        if (!line.isEmpty()){
                            QStringList field = line.split("\t");
                            nome = field[0];
                            x = field[1];
                            y = field[2];
                            arvore.insere_no(((x.toFloat()+72.896)*1000)/161.94,((y.toFloat()+33.691)*1000)/153.148,nome.toStdString());
                        }
                    }
                    setWindowTitle(file.fileName().section('/',-1,-1) + " - Visualizador de Point Quadtree");
                    file.close();
                    path.clear();
                    ui->actionSalvar_Como->setEnabled(false);
                    delete cenaHierarquia;
                    cenaHierarquia = new QGraphicsScene();
                    ui->graphicsViewHierarquia->setScene(cenaHierarquia);
                    imprimeParticionamento();
                    imprimeHierarquia();
                }else
                    throw file.errorString();
         }
    }catch(QString erro){
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
    }catch(std::string erroNoCodigo){
        QMessageBox erro(QMessageBox::Warning,"Erro no código de entrada:","Foram encontrados erros no código de entrada.\t\nClique em \"Mostrar Detalhes...\" para visualizar:",QMessageBox::Ok,this);
        erro.setInformativeText("Modelo: nome(tab)x(tab)y\nEx.: Abcd\t10\t20");
        erro.setDetailedText(QString::fromStdString(erroNoCodigo));
        erro.exec();
    }
}

void MainWindow::on_actionCoordenada_Cartesiana_triggered(){
    try{
        QString pathTemp;
        QFileDialog abrir(this,"Abrir coordenadas cartesianas:","","*.qdt");
        abrir.setAcceptMode(QFileDialog::AcceptOpen);
        if(abrir.exec()){
            pathTemp=abrir.selectedFiles()[0];
            QFile testeCodigo(pathTemp);
            if(testeCodigo.open(QIODevice::ReadOnly | QIODevice::Text)){
                QString codigoOk = verificaCodigo(&testeCodigo,"cartesiana");
                if (!codigoOk.isEmpty()) {
                    throw codigoOk.toStdString();
                }
            } else {
                throw testeCodigo.errorString();
            }
            QFile file(pathTemp);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                ui->actionSalvar_Como->setEnabled(true);
                QTextStream in(&file);
                QString nome;
                QString x;
                QString y;
                arvore.apagar_arvore();
                while (!in.atEnd()){
                    QString line = in.readLine();
                    if (!line.isEmpty()){
                        QStringList field = line.split("\t");
                        nome = field[0];
                        x = field[1];
                        y = field[2];
                        arvore.insere_no(x.toFloat(),y.toFloat(),nome.toStdString());
                    }
                }
                setWindowTitle(file.fileName().section('/',-1,-1) + " - Visualizador de Point Quadtree");
                file.close();
                path=pathTemp;
                delete cenaHierarquia;
                cenaHierarquia = new QGraphicsScene();
                ui->graphicsViewHierarquia->setScene(cenaHierarquia);
                imprimeParticionamento();
                imprimeHierarquia();
            } else {
                throw file.errorString();
            }
        }
    }catch(QString erro){
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
    }catch(std::string erroNoCodigo){
        QMessageBox erro(QMessageBox::Warning,"Erro no código de entrada:","Foram encontrados erros no código de entrada.\t\nClique em \"Mostrar Detalhes...\" para visualizar:",QMessageBox::Ok,this);
        erro.setInformativeText("Modelo: nome(tab)x(tab)y\nEx.: Abcd\t10\t20");
        erro.setDetailedText(QString::fromStdString(erroNoCodigo));
        erro.exec();
    }
}

void MainWindow::on_actionSalvar_triggered(){
    try{
        QFile file;
        QString pathTemp;
        QString pontos;
        qint64 ok;
        if(!path.isEmpty()){
            file.setFileName(path);
            if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
                arvore.string_pontos(pontos);
                ok = file.write(pontos.toUtf8());
                if (ok == -1)
                    throw file.errorString();
                setWindowTitle(file.fileName().section('/',-1,-1) + " - Visualizador de Point Quadtree");
                file.close();
            }else
                throw file.errorString();
        }else{
            QFileDialog salvar(this,"Salvar como:","nova árvore","*.qdt");
            salvar.setAcceptMode(QFileDialog::AcceptSave);
            salvar.setDefaultSuffix("qdt");
            if(salvar.exec()){
                pathTemp=salvar.selectedFiles()[0];
                if(!pathTemp.isEmpty()){
                    file.setFileName(pathTemp);
                    if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
                        arvore.string_pontos(pontos);
                        ok = file.write(pontos.toUtf8());
                        if (ok==-1)
                            throw file.errorString();
                        ui->actionSalvar_Como->setEnabled(true);
                        path=pathTemp;
                        setWindowTitle(file.fileName().section('/',-1,-1) + " - Visualizador de Point Quadtree");
                        file.close();
                    }else
                        throw file.errorString();
                }
            }
        }
    }catch(QString erro){
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
    }
}

void MainWindow::on_actionSalvar_Como_triggered(){
    try{
        qint64 ok;
        QString pathTemp;
        QFileDialog salvar(this,"Salvar como:",path,"*.qdt");
        salvar.setAcceptMode(QFileDialog::AcceptSave);
        salvar.setDefaultSuffix("qdt");
        if(salvar.exec()){
            pathTemp=salvar.selectedFiles()[0];

            if(!pathTemp.isEmpty()){
                QFile file(pathTemp);
                if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
                    QString pontos;
                    arvore.string_pontos(pontos);
                    ok = file.write(pontos.toUtf8());
                    if(ok == -1)
                        throw file.errorString();
                    path=pathTemp;
                    setWindowTitle(file.fileName().section('/',-1,-1) + " - Visualizador de Point Quadtree");
                    file.close();
                }else
                    throw file.errorString();
            }
        }
    }catch(QString erro){
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
    }
}

void MainWindow::on_actionApagar_triggered(){
    arvore.apagar_arvore();
    delete cenaHierarquia;
    cenaHierarquia = new QGraphicsScene();
    ui->graphicsViewHierarquia->setScene(cenaHierarquia);
    imprimeParticionamento();
    imprimeHierarquia();
}

void MainWindow::on_actionSair_triggered(){
    arvore.apagar_arvore();
    QApplication::quit();
}

void MainWindow::on_actionInserir_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(0);
}

void MainWindow::on_actionExcluir_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(1);
}

void MainWindow::on_actionBuscaNo_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(2);
}

void MainWindow::on_actionBuscaRegiao_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(3);
}

void MainWindow::on_actionBuscaVizinhanca_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(4);
}

//Botões
void MainWindow::on_pushButtonLimparInserir_clicked(){
    ui->spinBoxCordXInserir->setValue(0);
    ui->spinBoxCordYInserir->setValue(0);
    ui->lineEditRotuloInserir->clear();
    ui->lineEditRotuloInserir->setFocus();
}

void MainWindow::on_pushButtonLimparExcluir_clicked(){
    ui->spinBoxCordXExcluir->setValue(0);
    ui->spinBoxCordYExcluir->setValue(0);
    ui->textEditStatusExcluir->clear();
    ui->spinBoxCordXExcluir->setFocus();
}

void MainWindow::on_pushButtonLimpaBuscaNo_clicked(){
    ui->spinBoxCordXBuscaNo->setValue(0);
    ui->spinBoxCordYBuscaNo->setValue(0);
    ui->textEditResultadoBuscaNo->clear();
    ui->spinBoxCordXBuscaNo->setFocus();
    imprimeParticionamento();
    imprimeHierarquia();
}

void MainWindow::on_pushButtonLimpaBuscaRegiao_clicked(){
    ui->spinBoxCordX1BuscaRegiao->setValue(0);
    ui->spinBoxCordY1BuscaRegiao->setValue(0);
    ui->spinBoxCordX2BuscaRegiao->setValue(ui->spinBoxCordX2BuscaRegiao->maximum());
    ui->spinBoxCordY2BuscaRegiao->setValue(ui->spinBoxCordY2BuscaRegiao->maximum());
    ui->textEditResultadoBuscaRegiao->clear();
    ui->spinBoxCordX1BuscaRegiao->setFocus();
    imprimeParticionamento();
    imprimeHierarquia();
}

void MainWindow::on_pushButtonLimpaBuscaViz_clicked(){
    ui->spinBoxRaioBuscaViz->setValue(0);
    ui->spinBoxCordXBuscaViz->setValue(0);
    ui->spinBoxCordYBuscaViz->setValue(0);
    ui->textEditResultadoBuscaViz->clear();
    ui->spinBoxRaioBuscaViz->setFocus();
    imprimeParticionamento();
    imprimeHierarquia();
}

void MainWindow::on_pushButtonInserir_clicked(){
    if(ui->lineEditRotuloInserir->text().isEmpty()){
         QMessageBox::critical(this,"Inclusão:","Favor digitar um rótulo válido.");
     } else {
         desabilitaBotoes();
         arvore.insere_no(ui->spinBoxCordXInserir->value(),ui->spinBoxCordYInserir->value(),ui->lineEditRotuloInserir->text().toStdString(),cenaParticionamento,cenaHierarquia);
         habilitaBotoes();
         imprimeParticionamento();
         imprimeHierarquia();
     }
}

void MainWindow::on_pushButtonExcluir_clicked(){
    std::string busca="";
    desabilitaBotoes();
    imprimeParticionamento();
    QString noExcluido(QString::fromStdString(arvore.exclui_no(ui->spinBoxCordXExcluir->value(),ui->spinBoxCordYExcluir->value(),ui->textEditStatusExcluir,cenaParticionamento,cenaHierarquia,busca)));
    habilitaBotoes();
    if(noExcluido.isEmpty()){
        QMessageBox::warning(this,"Exclusão:","Nó inexistente.");
    }else{
        imprimeHierarquia(busca);
        QString noExcluido(QString::fromStdString(arvore.exclui_no(ui->spinBoxCordXExcluir->value(),ui->spinBoxCordYExcluir->value(),ui->textEditStatusExcluir,cenaParticionamento,cenaHierarquia)));
        imprimeHierarquia();
        imprimeParticionamento();
    }
}

void MainWindow::on_pushButtonBuscaNo_clicked(){
    std::string busca="";
    imprimeParticionamento();
    ui->textEditResultadoBuscaNo->clear();
    arvore.imprime_no(ui->spinBoxCordXBuscaNo->value(),ui->spinBoxCordYBuscaNo->value(),ui->textEditResultadoBuscaNo,cenaParticionamento,busca);
    imprimeHierarquia(busca);
}

void MainWindow::on_pushButtonBuscaRegiao_clicked(){
    std::string busca="";
    imprimeParticionamento();
    ui->textEditResultadoBuscaRegiao->clear();
    imprime_retangulo();
    arvore.busca_regiao(ui->spinBoxCordX1BuscaRegiao->value(),ui->spinBoxCordY1BuscaRegiao->value(),ui->spinBoxCordX2BuscaRegiao->value(),ui->spinBoxCordY2BuscaRegiao->value(),ui->textEditResultadoBuscaRegiao,cenaParticionamento,busca);
    imprimeHierarquia(busca);
    if(busca == ""){
        QMessageBox::warning(this,"Busca por Região:","Nenhum nó foi encontrado na faixa fornecida.");
    }
}

void MainWindow::on_pushButtonBuscaViz_clicked(){
    std::string busca="";
    imprimeParticionamento();
    ui->textEditResultadoBuscaViz->clear();
    imprime_circunferencia();
    arvore.busca_vizinhanca(ui->spinBoxCordXBuscaViz->value(),ui->spinBoxCordYBuscaViz->value(),ui->spinBoxRaioBuscaViz->value(),ui->textEditResultadoBuscaViz,cenaParticionamento,busca);
    imprimeHierarquia(busca);
    if(busca == ""){
        QMessageBox::warning(this,"Busca por Vizinhança:","Nenhum nó foi encontrado na faixa fornecida.");
    }
}

void MainWindow::on_pushButtonMenosBotoesHieraquia_clicked(){
    ui->graphicsViewHierarquia->scale(0.75,0.75);
}

void MainWindow::on_pushButtonMaisBotoesHieraquia_clicked(){
    ui->graphicsViewHierarquia->scale(1.25,1.25);
}

void MainWindow::on_pushButtonCemBotoesHieraquia_clicked(){
    ui->graphicsViewHierarquia->resetTransform();
}

void MainWindow::on_spinBoxRaioBuscaViz_valueChanged(int arg1){
    imprime_circunferencia();
}

void MainWindow::on_spinBoxCordXBuscaViz_valueChanged(int arg1){
    imprime_circunferencia();
}

void MainWindow::on_spinBoxCordYBuscaViz_valueChanged(int arg1){
    imprime_circunferencia();
}

void MainWindow::on_spinBoxCordX1BuscaRegiao_valueChanged(int arg1){
    imprime_retangulo();
}

void MainWindow::on_spinBoxCordY1BuscaRegiao_valueChanged(int arg1){
    imprime_retangulo();
}

void MainWindow::on_spinBoxCordX2BuscaRegiao_valueChanged(int arg1){
    imprime_retangulo();
}

void MainWindow::on_spinBoxCordY2BuscaRegiao_valueChanged(int arg1){
    imprime_retangulo();
}
