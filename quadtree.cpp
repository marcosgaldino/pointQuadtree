#include <cstdlib>
#include <cstring>
#include <QTextEdit>
#include <QString>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QPen>
#include <QFile>
#include <QTextStream>
#include <graphviz/gvc.h>
#include <exception>
#include <QProgressDialog>
#include <algorithm>    // std::min_element, std::max_element
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <dirent.h>
#include <direct.h>
#include <fstream>
#include <time.h>
#include "quadtree.h"

using namespace std;

quadtree::quadtree(){
    raiz = NULL;
}

quadtree::quadtree(noQuadtree* r){
    raiz = r;
}

quadtree::~quadtree(){
    //dtor
}

void quadtree::apagar_arvore(){
    if (raiz != NULL) {
        quadtree t;
        t.raiz = raiz->getNe();
        t.apagar_arvore();
        t.raiz = raiz->getNo();
        t.apagar_arvore();
        t.raiz = raiz->getSe();
        t.apagar_arvore();
        t.raiz = raiz->getSo();
        t.apagar_arvore();
        delete raiz;
        raiz = NULL;
    }
}

void quadtree::imprimeHierarquia(QGraphicsScene* cena,std::string busca){
    QProgressDialog progresso;
    progresso.show();
    progresso.setWindowTitle("Carregando Hierarquia dos nós...");
    progresso.setMinimumDuration(1000);
    progresso.setCancelButton(0);
    progresso.setWindowModality(Qt::WindowModal);
    progresso.setFixedSize(350,80);
    progresso.setLabelText("Gerando código DOT");
    progresso.setValue(0);

    cena->clear();
    QPixmap pix;
    progresso.setValue(10);
    QString string=QString::fromStdString(gera_dot_string(busca));
    progresso.setValue(20);
    QByteArray array = string.toLatin1();
    progresso.setValue(30);
    char* dot = array.data();

    progresso.setValue(40);
    Agraph_t *g;
    GVC_t *gvc;

    gvc = gvContext();
    progresso.setLabelText("Lendo código DOT");
    progresso.setValue(50);
    g = agmemread(dot);
    progresso.setLabelText("Preparando layout");
    progresso.setValue(60);
    gvLayout(gvc, g, "dot");

    unsigned int lenght;
    char* result;

    progresso.setLabelText("Renderizando imagem");
    progresso.setValue(70);
    gvRenderData(gvc,g,"png",&result,&lenght);
    progresso.setLabelText("Gerando imagem");
    progresso.setValue(80);
    QByteArray image(result,lenght);
    pix.loadFromData(image);
    progresso.setLabelText("Adicionando imagem à cena");
    progresso.setValue(90);
    cena->addPixmap(pix);
    progresso.setValue(100);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    progresso.close();
}

void quadtree::imprimeParticionamento(QGraphicsScene* cena){
    if (raiz != NULL){
        QRectF rect(cena->sceneRect());
        QPen pen;
        if ((raiz->getSe() != NULL) || (raiz->getSo() != NULL)||(raiz->getNe() != NULL) || (raiz->getNo() != NULL)){
            quadtree t;
            pen.setColor(Qt::lightGray);
            pen.setWidth(0);
            if(raiz->getNe()!= NULL){
                cena->addLine(raiz->getX(),cena->sceneRect().y(),raiz->getX(),cena->sceneRect().bottom(),pen);//imprime linha vertical
                cena->addLine(cena->sceneRect().x(),raiz->getY(),cena->sceneRect().right(),raiz->getY(),pen);//imprime linha horizontal

                t.raiz = raiz->getNe();
                cena->setSceneRect(raiz->getX(),raiz->getY(),cena->width()-(raiz->getX()-cena->sceneRect().x()), cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                t.imprimeParticionamento(cena);
                cena->setSceneRect(rect);

                t.raiz = raiz->getNo();
                cena->setSceneRect(cena->sceneRect().x(),raiz->getY(),raiz->getX()-cena->sceneRect().x(),cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                t.imprimeParticionamento(cena);
                cena->setSceneRect(rect);

                t.raiz = raiz->getSe();
                cena->setSceneRect(raiz->getX(),cena->sceneRect().y(),cena->width()-(raiz->getX()-cena->sceneRect().x()), raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                t.imprimeParticionamento(cena);
                cena->setSceneRect(rect);

                t.raiz = raiz->getSo();
                cena->setSceneRect(cena->sceneRect().x(),cena->sceneRect().y(),raiz->getX()-cena->sceneRect().x(),raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                t.imprimeParticionamento(cena);
                cena->setSceneRect(rect);
            } else {
                if(raiz->getNo()!= NULL){
                    cena->addLine(raiz->getX(),cena->sceneRect().y(),raiz->getX(),cena->sceneRect().bottom(),pen);//imprime linha vertical
                    cena->addLine(cena->sceneRect().x(),raiz->getY(),cena->sceneRect().right(),raiz->getY(),pen);//imprime linha horizontal

                    t.raiz = raiz->getNe();
                    cena->setSceneRect(raiz->getX(),raiz->getY(),cena->width()-(raiz->getX()-cena->sceneRect().x()), cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                    t.imprimeParticionamento(cena);
                    cena->setSceneRect(rect);

                    t.raiz = raiz->getNo();
                    cena->setSceneRect(cena->sceneRect().x(),raiz->getY(),raiz->getX()-cena->sceneRect().x(),cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                    t.imprimeParticionamento(cena);
                    cena->setSceneRect(rect);

                    t.raiz = raiz->getSe();
                    cena->setSceneRect(raiz->getX(),cena->sceneRect().y(),cena->width()-(raiz->getX()-cena->sceneRect().x()), raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                    t.imprimeParticionamento(cena);
                    cena->setSceneRect(rect);

                    t.raiz = raiz->getSo();
                    cena->setSceneRect(cena->sceneRect().x(),cena->sceneRect().y(),raiz->getX()-cena->sceneRect().x(),raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                    t.imprimeParticionamento(cena);
                    cena->setSceneRect(rect);
                } else {
                    if(raiz->getSe()!= NULL){
                        cena->addLine(raiz->getX(),cena->sceneRect().y(),raiz->getX(),cena->sceneRect().bottom(),pen);//imprime linha vertical
                        cena->addLine(cena->sceneRect().x(),raiz->getY(),cena->sceneRect().right(),raiz->getY(),pen);//imprime linha horizontal

                        t.raiz = raiz->getNe();
                        cena->setSceneRect(raiz->getX(),raiz->getY(),cena->width()-(raiz->getX()-cena->sceneRect().x()), cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                        t.imprimeParticionamento(cena);
                        cena->setSceneRect(rect);

                        t.raiz = raiz->getNo();
                        cena->setSceneRect(cena->sceneRect().x(),raiz->getY(),raiz->getX()-cena->sceneRect().x(),cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                        t.imprimeParticionamento(cena);
                        cena->setSceneRect(rect);

                        t.raiz = raiz->getSe();
                        cena->setSceneRect(raiz->getX(),cena->sceneRect().y(),cena->width()-(raiz->getX()-cena->sceneRect().x()), raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                        t.imprimeParticionamento(cena);
                        cena->setSceneRect(rect);

                        t.raiz = raiz->getSo();
                        cena->setSceneRect(cena->sceneRect().x(),cena->sceneRect().y(),raiz->getX()-cena->sceneRect().x(),raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                        t.imprimeParticionamento(cena);
                        cena->setSceneRect(rect);
                    } else {
                        if(raiz->getSo()!= NULL){
                            cena->addLine(raiz->getX(),cena->sceneRect().y(),raiz->getX(),cena->sceneRect().bottom(),pen);//imprime linha vertical
                            cena->addLine(cena->sceneRect().x(),raiz->getY(),cena->sceneRect().right(),raiz->getY(),pen);//imprime linha horizontal

                            t.raiz = raiz->getNe();
                            cena->setSceneRect(raiz->getX(),raiz->getY(),cena->width()-(raiz->getX()-cena->sceneRect().x()), cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                            t.imprimeParticionamento(cena);
                            cena->setSceneRect(rect);

                            t.raiz = raiz->getNo();
                            cena->setSceneRect(cena->sceneRect().x(),raiz->getY(),raiz->getX()-cena->sceneRect().x(),cena->height()-(raiz->getY()-cena->sceneRect().y()));//x, y, largura e altura
                            t.imprimeParticionamento(cena);
                            cena->setSceneRect(rect);

                            t.raiz = raiz->getSe();
                            cena->setSceneRect(raiz->getX(),cena->sceneRect().y(),cena->width()-(raiz->getX()-cena->sceneRect().x()), raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                            t.imprimeParticionamento(cena);
                            cena->setSceneRect(rect);

                            t.raiz = raiz->getSo();
                            cena->setSceneRect(cena->sceneRect().x(),cena->sceneRect().y(),raiz->getX()-cena->sceneRect().x(),raiz->getY()-cena->sceneRect().y());//x, y, largura e altura
                            t.imprimeParticionamento(cena);
                            cena->setSceneRect(rect);
                        }
                    }
                }
            }
        }
        pen.setColor(Qt::black);
        pen.setWidth(3);
        QGraphicsRectItem *ponto = cena->addRect(raiz->getX()-1,raiz->getY()-1,2,2,pen);//imprime ponto
        ponto->setToolTip(QString::fromStdString(raiz->imprime_string()));
        ponto->setZValue(1);
    }
}

//encontrar o pai do nó p na árvore raiz, raiz é filha de f
noQuadtree* quadtree::encontrar_pai(noQuadtree* p, noQuadtree* f){
    if (raiz->coord_iguais(p)){
        return f;
    }else{
        quadtree t (*raiz->filho(raiz->compara_nos(*p)));
        return t.encontrar_pai(p,raiz);
    }
}

void quadtree::insere_no(int coordx, int coordy, std::string rotulo){
    try{
        noQuadtree * f = NULL;
        noQuadtree * t = NULL;
        posicao q;
        noQuadtree p(coordx, coordy, rotulo);
        if (raiz == NULL ) {
            raiz = new noQuadtree;
            raiz->setVal(p.getVal());
            raiz->setSe(NULL);
            raiz->setSo(NULL);
            raiz->setNe(NULL);
            raiz->setNo(NULL);
        } else {
            t = raiz;
            while ((t != NULL) && (!(t->coord_iguais(&p)))) {
                f = t;
                q = t->compara_nos (p);
                t = (*t->filho(q));
            }
            if (t == NULL){
                t = new noQuadtree;
                t->setVal(p.getVal());
                t->setSe(NULL);
                t->setSo(NULL);
                t->setNe(NULL);
                t->setNo(NULL);
                (* f->filho(q)) = t;
            }else{
                QStringList nomeList;
                QStringList nomeNovoList(QString::fromStdString(rotulo).split(",",QString::SkipEmptyParts));
                QString nomeTemp;
                QString nomeNovoTemp;
                QString nomeResult(QString::fromStdString(t->getNome()));
                bool existe = false;
                while(!nomeNovoList.isEmpty()){
                    nomeNovoTemp=nomeNovoList.takeFirst();
                    nomeList=nomeResult.split(",",QString::SkipEmptyParts);
                    while(!nomeList.isEmpty()){
                        nomeTemp=nomeList.takeFirst();
                        if(nomeNovoTemp==nomeTemp)
                            existe=true;
                    }
                    if(!existe){
                        nomeResult = nomeResult + "," + nomeNovoTemp;
                    }else{
                        existe=false;
                    }
                }
                t->setNome(nomeResult.toStdString());
            }
        }
    }
    catch(std::bad_alloc){
        QMessageBox::warning(NULL, "Erro: bad_alloc", "Não será possível inserir outro nó.\n\nNão há memória disponível.");
    }
}

void quadtree::insere_no(int coordx,int coordy,std::string rotulo,QGraphicsScene* cenaP,QGraphicsScene* cenaH){
    try{
         noQuadtree* f=NULL;
         noQuadtree* t=NULL;
         posicao q;
         noQuadtree p(coordx,coordy,rotulo);
         std::stringstream str_dot;
         QMessageBox msg(QMessageBox::Information,"Inclusão:","",QMessageBox::Ok);
         msg.setWindowModality(Qt::WindowModal);
         QPen pen;
         pen.setWidth(3);
         QGraphicsRectItem *ponto;

         if (raiz == NULL){
            raiz = new noQuadtree;
            raiz->setVal(p.getVal());
            raiz->setSe(NULL);
            raiz->setSo(NULL);
            raiz->setNe(NULL);
            raiz->setNo(NULL);

            str_dot << "\"" << raiz->getNome() << "\\n" << "x=" << raiz->getX() << "\\n" << "y=" << raiz->getY() << "\\n" << "\"[color=\"red\" fontcolor=\"white\" style=\"filled\"];" << std::endl;
            imprimeHierarquia(cenaH,str_dot.str());

            pen.setColor(Qt::red);
            ponto = cenaP->addRect(raiz->getX()-1,raiz->getY()-1,2,2,pen);//imprime ponto
            ponto->setToolTip(QString::fromStdString(raiz->imprime_string()));
            ponto->setZValue(2);
            msg.setText("O nó " + QString::fromStdString(raiz->imprime_string()) + " foi inserido.");
            msg.exec();
         }
         else {
            t = raiz;
            while((t != NULL) && (!(t->coord_iguais(&p)))){
                 f = t;
                 q = t->compara_nos(p);
                 t = (*t->filho(q));
                 if(t != NULL){
                     str_dot << "\"" << f->getNome() << "\\n" << "x=" << f->getX() << "\\n" << "y=" << f->getY() << "\\n" << "\"[color=\"green2\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
                     if(!(t->coord_iguais(&p)))
                         str_dot << "\"" << f->getNome() << "\\n" << "x=" << f->getX() << "\\n" << "y=" << f->getY() << "\\n" << "\"->\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\" [color=\"green2\"];" << std::endl;

                     pen.setColor("#00ee00");
                     ponto = cenaP->addRect(f->getX()-1,f->getY()-1,2,2,pen);//imprime ponto que sai
                     ponto->setToolTip(QString::fromStdString(f->imprime_string()));
                     ponto->setZValue(2);
                 }
            }
            if(t == NULL){
                 t = new noQuadtree;
                 t->setVal(p.getVal());
                 t->setSe(NULL);
                 t->setSo(NULL);
                 t->setNe(NULL);
                 t->setNo(NULL);
                 (*f->filho(q))=t;

                 str_dot << "\"" << f->getNome() << "\\n" << "x=" << f->getX() << "\\n" << "y=" << f->getY() << "\\n"  << "\"[color=\"green2\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
                 str_dot << "\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n"  << "\"[color=\"red\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
                 str_dot << "\"" << f->getNome() << "\\n" << "x=" << f->getX() << "\\n" << "y=" << f->getY() << "\\n" << "\"->\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\" [color=\"green2\"];" << std::endl;

                 imprimeHierarquia(cenaH,str_dot.str());

                 pen.setColor("#00ee00");
                 ponto = cenaP->addRect(f->getX()-1,f->getY()-1,2,2,pen);//imprime ponto que sai
                 ponto->setToolTip(QString::fromStdString(f->imprime_string()));
                 ponto->setZValue(2);
                 pen.setColor(Qt::red);
                 ponto = cenaP->addRect(t->getX()-1,t->getY()-1,2,2,pen);//imprime ponto substituto
                 ponto->setToolTip(QString::fromStdString(t->imprime_string()));
                 ponto->setZValue(2);
                 imprimeParticionamento(cenaP);

                 msg.setText("O nó " + QString::fromStdString(t->imprime_string()) + " foi inserido.");
                 msg.exec();
            }else{
                QStringList nomeList;
                QStringList nomeNovoList(QString::fromStdString(rotulo).split(",",QString::SkipEmptyParts));
                QString nomeTemp;
                QString nomeNovoTemp;
                QString nomeResult(QString::fromStdString(t->getNome()));
                bool existe = false;
                bool atualizou = false;
                while(!nomeNovoList.isEmpty())
                {
                    nomeNovoTemp = nomeNovoList.takeFirst();
                    nomeList = nomeResult.split(",",QString::SkipEmptyParts);
                    while(!nomeList.isEmpty())
                    {
                        nomeTemp = nomeList.takeFirst();
                        if(nomeNovoTemp == nomeTemp)
                            existe = true;
                    }
                    if(!existe)
                    {
                        nomeResult = nomeResult + "," + nomeNovoTemp;
                        atualizou = true;
                    }else{
                        existe = false;
                    }
                }
                t->setNome(nomeResult.toStdString());
                if(!atualizou)
                {
                    str_dot << "\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\"[color=\"green2\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
                    if(f!=NULL)
                        str_dot << "\"" << f->getNome() << "\\n" << "x=" << f->getX() << "\\n" << "y=" << f->getY() << "\\n" << "\"->\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\" [color=\"green2\"];" << std::endl;
                    imprimeHierarquia(cenaH,str_dot.str());
                    pen.setColor("#00ee00");
                    ponto = cenaP->addRect(t->getX()-1,t->getY()-1,2,2,pen);
                    ponto->setToolTip(QString::fromStdString(t->imprime_string()));
                    ponto->setZValue(2);
                    msg.setText("Nó " + QString::fromStdString(t->imprime_string()) + " já existente.");
                    msg.exec();
                }else{
                    str_dot << "\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\"[color=\"magenta\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
                    if(f!=NULL)
                        str_dot << "\"" << f->getNome() << "\\n" << "x=" << f->getX() << "\\n" << "y=" << f->getY() << "\\n" << "\"->\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\" [color=\"green2\"];" << std::endl;

                    imprimeHierarquia(cenaH,str_dot.str());
                    pen.setColor("#ff00ff");
                    ponto = cenaP->addRect(t->getX()-1,t->getY()-1,2,2,pen);
                    ponto->setToolTip(QString::fromStdString(t->imprime_string()));
                    ponto->setZValue(2);
                    msg.setText("Nó " + QString::fromStdString(t->imprime_string()) + " com nome acrescentado.");
                    msg.exec();
                }
            }
         }
     }
    catch (std::bad_alloc){
        QMessageBox::warning(NULL,"Erro: bad_alloc","Não é possível inserir outro nó.\n\nNão há memória disponível.");
    }
}

noQuadtree* quadtree::busca_no(int x,int y){
    noQuadtree p(x,y);
    if (raiz == NULL){
        return NULL;
    }
    if (raiz->coord_iguais(&p)){
        return raiz;
    }
    quadtree t (*raiz->filho(raiz->compara_nos(p)));
    return t.busca_no(x,y);
}

void quadtree::imprime_no(int x,int y,QTextEdit *text,QGraphicsScene *cena,std::string& busca){
    noQuadtree* t;
    t=busca_no(x,y);
    if(t!=NULL){
        std::stringstream str_dot;
        str_dot << "\"" << t->getNome() << "\\n" << "x=" << t->getX() << "\\n" << "y=" << t->getY() << "\\n" << "\"[color=\"goldenrod1\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
        busca=str_dot.str();

        text->append(QString::fromStdString(t->imprime_string()));
        QPen pen;
        pen.setColor("#ffc125");
        pen.setWidth(3);
        QGraphicsRectItem *ponto = cena->addRect(t->getX()-1,t->getY()-1,2,2,pen);//imprime ponto
        ponto->setToolTip(QString::fromStdString(t->imprime_string()));
        ponto->setZValue(2);
    } else {
        QMessageBox::warning(text->parentWidget(),"Busca por Nó:","Nó inexistente.");
    }
}

bool quadtree::verifica_circulo(noQuadtree p, int r){
    if ((r*r)>=((p.getX() - raiz->getX())*(p.getX() - raiz->getX())+(p.getY() - raiz->getY())*(p.getY() - raiz->getY())))
        return true;
    else
        return false;
}

void quadtree::busca_vizinhanca(int x, int y, int r,QTextEdit *text,QGraphicsScene *cena,std::string& busca){
    noQuadtree p(x, y);
    if (raiz != NULL){
        if(verifica_circulo(p, r)){
            std::stringstream str_dot;
            str_dot << "\"" << raiz->getNome() << "\\n" << "x=" << raiz->getX() << "\\n" << "y=" << raiz->getY() << "\\n\"[color=\"goldenrod1\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
            busca = busca+str_dot.str();

            text->append(QString::fromStdString(raiz->imprime_string()));
            QPen pen;
            pen.setColor("#ffc125");
            pen.setWidth(3);
            QGraphicsRectItem *ponto = cena->addRect(raiz->getX()-1,raiz->getY()-1,2,2,pen);//imprime ponto
            ponto->setZValue(2);
        }
        quadtree t;

        t.raiz=raiz->getSo();
        t.busca_vizinhanca(x, y, r, text, cena, busca);

        t.raiz=raiz->getNo();
        t.busca_vizinhanca(x, y, r, text, cena, busca);

        t.raiz=raiz->getSe();
        t.busca_vizinhanca(x, y, r, text, cena, busca);

        t.raiz=raiz->getNe();
        t.busca_vizinhanca(x, y, r, text, cena, busca);
    }
}

bool quadtree::verifica_retangulo(noQuadtree p1, noQuadtree p2){
    if((p1.getX() <= raiz->getX()) && (raiz->getX() <= p2.getX()) && (p1.getY() <= raiz->getY()) && (raiz->getY() <= p2.getY()))
        return true;
    else
        return false;
}

void quadtree::busca_regiao(int x1, int y1, int x2, int y2, QTextEdit *text, QGraphicsScene *cena, std::string& busca){
    noQuadtree p1 (x1, y1), p2 (x2, y2);
    if (raiz != NULL){
        if (verifica_retangulo(p1,p2)){
            std::stringstream str_dot;
            str_dot << "\"" << raiz->getNome() << "\\n" << "x=" << raiz->getX() << "\\n" << "y=" << raiz->getY() << "\\n\"[color=\"goldenrod1\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
            busca=busca+str_dot.str();

            text->append(QString::fromStdString(raiz->imprime_string()));
            QPen pen;
            pen.setColor("#ffc125");
            pen.setWidth(3);
            QGraphicsRectItem *ponto = cena->addRect(raiz->getX()-1,raiz->getY()-1,2,2,pen);//imprime ponto
            ponto->setZValue(2);
        }
        quadtree t;

        t.raiz = raiz->getSo();
        t.busca_regiao(x1,y1,x2,y2,text,cena,busca);

        t.raiz = raiz->getNo();
        t.busca_regiao(x1,y1,x2,y2,text,cena,busca);

        t.raiz = raiz->getSe();
        t.busca_regiao(x1,y1,x2,y2,text,cena,busca);

        t.raiz = raiz->getNe();
        t.busca_regiao(x1,y1,x2,y2,text,cena,busca);
    }
}

void quadtree::gera_quadtree_randomica(int numeroNos){
    if(!opendir("../arquivos")){
        mkdir("../arquivos");
    }
    int numero_de_letras = 4;
    int x, y, posicao;
    char letras[27] = "abcdefghijklmnopqrstuvxywz", nome[numero_de_letras];
    char letra[1];

    fstream arquivo;
    arquivo.open("../arquivos/random001.qdt", ios::out);
    arquivo.clear();

    //Inicializando o seed
    srand(time(NULL));

    for(int i = 0; i < numeroNos; i++){
        memset(&nome, 0, sizeof(nome));
        x = rand() % 250;
        y = rand() % 250;
        for(int j = 0; j < numero_de_letras; j++){
            posicao = rand() % 26;
            letra[0] = letras[posicao];
            nome[j] = letra[0];
        }
        QMessageBox msg(QMessageBox::Information,nome,"",QMessageBox::Ok);
        arquivo << nome << "\t" << x << "\t" << y << "\n";
    }
    arquivo << "\n";
    arquivo.close();
}

std::string quadtree::gera_dot_string(std::string busca){//gera uma string com o código em DOT da árvore
    std::string dot;
    dot = "digraph quadtree {graph[dpi=\"150\" charset=\"latin1\" nodesep=\"0,1\" ranksep=\"0\" size=\"\" bgcolor=\"white\"] node[fontsize=\"8\" height=\"0\" width=\"0\" margin=\"0,0\"] edge[arrowsize=\"0.5\"]";//Linux ","
    gera_dot_string_recursao(dot);
    dot = dot+busca+"}";//nós encontrados em alguma busca
    return dot;
}

std::string quadtree::gera_dot_string_recursao(std::string& dot){
    //Constrói os quatro nós filhos na árvore.
    std::stringstream str_dot;
    if (raiz != NULL) {
        str_dot << "\"" << raiz->getNome() << "\\n" << "x=" << raiz->getX() << "\\n" << "y=" << raiz->getY() << "\\n\"->{";

        if (raiz->getNo() != NULL){
            str_dot << "\"" << raiz->getNo()->getNome() << "\\n" << "x=" << raiz->getNo()->getX() << "\\n" << "y=" << raiz->getNo()->getY() << "\\n\";";
        }else{
            str_dot << "\"(" << raiz->getX() << "," << raiz->getY() << ")\\nNo=NULL" << "\"[color=\"lightblue\" style=\"filled\"];";
        }

        if (raiz->getNe() != NULL){
            str_dot << "\"" << raiz->getNe()->getNome() << "\\n" << "x=" << raiz->getNe()->getX() << "\\n" << "y=" << raiz->getNe()->getY() << "\\n\";";
        }else{
            str_dot << "\"(" << raiz->getX() << "," << raiz->getY() << ")\\nNe=NULL" << "\"[color=\"lightblue\" style=\"filled\"];";
        }

        if (raiz->getSo() != NULL){
            str_dot << "\"" << raiz->getSo()->getNome() << "\\n" << "x=" << raiz->getSo()->getX() << "\\n" << "y=" << raiz->getSo()->getY() << "\\n\"";
        }else{
            str_dot << "\"(" << raiz->getX() << "," << raiz->getY() << ")\\nSo=NULL" << "\"[color=\"lightblue\" style=\"filled\"]";
        }

        if (raiz->getSe() != NULL){
            str_dot << "\"" << raiz->getSe()->getNome() << "\\n" << "x=" << raiz->getSe()->getX() << "\\n" << "y=" << raiz->getSe()->getY() << "\\n\";";
        }else{
            str_dot << "\"(" << raiz->getX() << "," << raiz->getY() << ")\\nSe=NULL" << "\"[color=\"lightblue\" style=\"filled\"];";
        }

        str_dot << "};" << std::endl;

        dot=dot+str_dot.str();
        quadtree t;
        t.raiz=raiz->getSe();
        t.gera_dot_string_recursao(dot);
        t.raiz=raiz->getSo();
        t.gera_dot_string_recursao(dot);
        t.raiz=raiz->getNe();
        t.gera_dot_string_recursao(dot);
        t.raiz=raiz->getNo();
        t.gera_dot_string_recursao(dot);
        return dot;
    }else{
        return dot;
    }
}

void quadtree::string_pontos(QString& pontos){//gera uma string com os pontos de uma árvore, em ordem
    std::stringstream str_pontos;
    if(raiz != NULL){
        str_pontos << raiz->getNome() << "\t" << raiz->getX() << "\t" << raiz->getY() << std::endl;
        pontos.operator += (QString::fromStdString(str_pontos.str()));
        quadtree t;
        t.raiz = raiz->getNo();
        t.string_pontos(pontos);
        t.raiz = raiz->getNe();
        t.string_pontos(pontos);
        t.raiz = raiz->getSo();
        t.string_pontos(pontos);
        t.raiz = raiz->getSe();
        t.string_pontos(pontos);
    }
}

//Retorna o candidato filho a uma determinada posição para substituir o nó que será excluído.

noQuadtree* quadtree::encontra_candidato(noQuadtree* p, posicao q){
     if(p == NULL){
         noQuadtree * inf = NULL;
         inf = new noQuadtree;
         inf->setSe(NULL);
         inf->setSo(NULL);
         inf->setNe(NULL);
         inf->setNo(NULL);
         inf->setX(5000);
         inf->setY(5000);
         inf->setNome("Nulo");
         return inf;
     } else {
         while(*p->filho(opQuad(q)) != NULL){
            p = *p->filho(opQuad(q));
         }
         return p;
     }
}

// Escolhe o nó candidato, em um determinado quadrante, a substituir o nó a ser deletado.

noQuadtree* quadtree::escolhe_candidato(noQuadtree* p){
    noQuadtree * candSe, * candSo, * candNe, * candNo, * escolhido;
    int quantos = 0;
    candSe = encontra_candidato(p->getSe(),sudeste);
    candSo = encontra_candidato(p->getSo(),sudoeste);
    candNe = encontra_candidato(p->getNe(),nordeste);
    candNo = encontra_candidato(p->getNo(),noroeste);

    //Critério 1
    if((candSe->getX() - p->getX()) < (p->getX() - candSo->getX()) && (candSe->getX() - p->getX()) < (candNe->getX() - p->getX()) &&
    (p->getY() - candSe->getY()) < (p->getY() - candSo->getY()) && (p->getY() - candSe->getY()) < (candNe->getY() - p->getY())
    && (candSe->getX() != 5000)){
        quantos++;
        escolhido = candSe;
    }
    if((p->getX() - candSo->getX()) < (candSe->getX() - p->getX()) && (p->getX() - candSo->getX()) < (p->getX() - candNo->getX()) &&
    (p->getY() - candSo->getY()) < (p->getY() - candSe->getY()) && (p->getY() - candSo->getY()) < (candNo->getY() - p->getY())
    && (candSo->getX() != 5000)){
        quantos++;
        escolhido = candSo;
    }
    if((candNe->getX() - p->getX()) < (candSe->getX() - p->getX()) && (candNe->getX() - p->getX()) < (p->getX() - candNo->getX()) &&
    (candNe->getY() - p->getY()) < (p->getY() - candSe->getY()) && (candNe->getY() - p->getY()) < (candNo->getY()- p->getY())
    && (candNe->getX() != 5000)){
        quantos++;
        escolhido = candNe;
    }
    if((p->getX() - candNo->getX())< (candNe->getX() - p->getX()) && (p->getX() - candNo->getX()) < (p->getX() - candSo->getX()) &&
    (candNo->getY() - p->getY()) < (p->getY() - candSo->getY()) && (candNo->getY() - p->getY()) < (candNe->getY() - p->getY())
    && (candNo->getX() != 5000)){
        quantos++;
        escolhido = candNo;
    }

    if(quantos != 1){
        //Critério 2 - Métrica de Manhattan
        int * l1;
        int valores[4];
        if(candSe->getX() != 5000 && candSe != p){
            valores[0] = (candSe->getX() - p->getX()) + (p->getY() - candSe->getY());
        } else {
            valores[0] = 5000;
        }

        if(candSo->getX() != 5000 && candSo != p){
            valores[1] = (p->getX() - candSo->getX()) + (p->getY() - candSo->getY());
        } else {
            valores[1] = 5000;
        }

        if(candNe->getX() != 5000 && candNe != p){
            valores[2] = (candNe->getX() - p->getX()) + (candNe->getY() - p->getY());
        } else {
            valores[2] = 5000;
        }

        if(candNo->getX() != 5000 && candNo != p){
            valores[3] = (p->getX() - candNo->getX()) + (candNo->getY() - p->getY());
        } else {
            valores[3] = 5000;
        }

        l1 = std::min_element(valores, valores+4);

        if(*l1 == valores[0]){
            escolhido = candSe;
        } else {
            if(*l1 == valores[1]){
                escolhido = candSo;
            } else {
                if(*l1 == valores[2]){
                    escolhido = candNe;
                } else {
                    escolhido = candNo;
                }
            }
        }
    }
    return escolhido;
}

//Retorna o quadrante vizinho no sentido horário
posicao quadtree::sentidoH(posicao q){
    posicao sentH;
    switch(q){
        case 0:
            sentH = nordeste;
            break;
        case 1:
            sentH = sudeste;
            break;
        case 2:
            sentH = sudoeste;
            break;
        case 3:
            sentH = noroeste;
            break;
    }
    return sentH;
}

//Retorna o quadrante vizinho no sentido anti-horário
posicao quadtree::sentidoAH(posicao q){
    posicao sentAH;
    switch(q){
        case 0:
            sentAH = sudoeste;
            break;
        case 1:
            sentAH = noroeste;
            break;
        case 2:
            sentAH = nordeste;
            break;
        case 3:
            sentAH = sudeste;
            break;
    }
    return sentAH;
}


noQuadtree* quadtree::realca_delecao(noQuadtree p,QTextEdit *text, int quantos, posicao unicoFilho, QGraphicsScene* cenaP,QGraphicsScene* cenaH, std::string& busca){
    noQuadtree * r = NULL;
    std::stringstream str_dot;
    QMessageBox msg(QMessageBox::Information,"Exclusão:","",QMessageBox::Ok);
    msg.setWindowModality(Qt::WindowModal);
    QPen pen;
    pen.setWidth(3);
    QGraphicsRectItem *ponto;
    if (quantos == 0){ //p é uma folha
        str_dot << "\"" << p.getNome() << "\\n" << "x=" << p.getX() << "\\n" << "y=" << p.getY() << "\\n" << "\"[color=\"red\" fontcolor=\"white\" style=\"filled\"];" << std::endl;
        busca=busca+str_dot.str();
        imprimeHierarquia(cenaH,busca);
        text->setTextColor(Qt::darkCyan);
        text->append("O nó " + QString::fromStdString(p.imprime_string()) + " será substituído por NULL.");
        text->setTextColor(Qt::black);

        pen.setColor(Qt::red);
        ponto = cenaP->addRect(p.getX()-1,p.getY()-1,2,2,pen);//imprime ponto
        ponto->setToolTip(QString::fromStdString(p.imprime_string()));
        ponto->setZValue(2);
        msg.setText("O nó " + QString::fromStdString(p.imprime_string()) + " será substituído por NULL.");
        msg.exec();
    } else {
        str_dot << "\"" << p.getNome() << "\\n" << "x=" << p.getX() << "\\n" << "y=" << p.getY() << "\\n" << "\"[color=\"red\" fontcolor=\"white\" style=\"filled\"];" << std::endl;
        r = *p.filho(unicoFilho);
        str_dot << "\"" << r->getNome() << "\\n" << "x=" << r->getX() << "\\n" << "y=" << r->getY() << "\\n" << "\"[color=\"green2\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
        busca=busca+str_dot.str();
        imprimeHierarquia(cenaH,busca);
        text->setTextColor(Qt::darkCyan);
        text->append("O nó " + QString::fromStdString(p.imprime_string()) + " será substituído por "+ QString::fromStdString(r->imprime_string())+ ".");
        text->setTextColor(Qt::black);

        pen.setColor(Qt::red);
        ponto = cenaP->addRect(p.getX()-1,p.getY()-1,2,2,pen);//imprime ponto
        ponto->setToolTip(QString::fromStdString(p.imprime_string()));
        ponto->setZValue(2);
        pen.setColor("#00ee00");
        ponto = cenaP->addRect(r->getX()-1,r->getY()-1,2,2,pen);//imprime ponto substituto
        ponto->setToolTip(QString::fromStdString(r->imprime_string()));
        ponto->setZValue(2);
        msg.setText("O nó " + QString::fromStdString(p.imprime_string()) + " será substituído por "+ QString::fromStdString(r->imprime_string())+ ".");
        msg.exec();
    }
    return NULL;
}

noQuadtree* quadtree::realca_delecao(noQuadtree p,QTextEdit *text, int quantos, noQuadtree* substituto, QGraphicsScene* cenaP,QGraphicsScene* cenaH, std::string& busca){
    noQuadtree * r = NULL;
    std::stringstream str_dot;
    QMessageBox msg(QMessageBox::Information,"Exclusão:","",QMessageBox::Ok);
    msg.setWindowModality(Qt::WindowModal);
    QPen pen;
    pen.setWidth(3);
    QGraphicsRectItem *ponto;
    str_dot << "\"" << p.getNome() << "\\n" << "x=" << p.getX() << "\\n" << "y=" << p.getY() << "\\n" << "\"[color=\"red\" fontcolor=\"white\" style=\"filled\"];" << std::endl;
    r = substituto;
    str_dot << "\"" << r->getNome() << "\\n" << "x=" << r->getX() << "\\n" << "y=" << r->getY() << "\\n" << "\"[color=\"green2\" fontcolor=\"black\" style=\"filled\"];" << std::endl;
    busca=busca+str_dot.str();
    imprimeHierarquia(cenaH,busca);
    text->setTextColor(Qt::darkCyan);
    text->append("O nó " + QString::fromStdString(p.imprime_string()) + " será substituído por "+ QString::fromStdString(r->imprime_string())+ ".");
    text->setTextColor(Qt::black);

    pen.setColor(Qt::red);
    ponto = cenaP->addRect(p.getX()-1,p.getY()-1,2,2,pen);//imprime ponto
    ponto->setToolTip(QString::fromStdString(p.imprime_string()));
    ponto->setZValue(2);
    pen.setColor("#00ee00");
    ponto = cenaP->addRect(r->getX()-1,r->getY()-1,2,2,pen);//imprime ponto substituto
    ponto->setToolTip(QString::fromStdString(r->imprime_string()));
    ponto->setZValue(2);
    msg.setText("O nó " + QString::fromStdString(p.imprime_string()) + " será substituído por "+ QString::fromStdString(r->imprime_string())+ ".");
    msg.exec();
    return NULL;
}

//Deleta o nó t da quadtree com raiz no nó r. Se a raiz da árvore foi deletada, reinicializa r.
std::string quadtree::exclui_no(int x, int y,QTextEdit* text,QGraphicsScene* cenaP,QGraphicsScene* cenaH){
    noQuadtree *substituto, *t, txt;
    std::string nome, busca;
    std::stringstream str_dot;
    QMessageBox msg(QMessageBox::Information,"Exclusão:","",QMessageBox::Ok);
    msg.setWindowModality(Qt::WindowModal);
    t = busca_no(x,y);
    if(t != NULL){
        txt = *t;
        nome = t->imprime_string();
        //Contando o número de filhos do nó a ser excluído
        noQuadtree * filhoSE, * filhoSO, * filhoNE, * filhoNO, * lixo, * temp;
        posicao unicoFilho;
        filhoSE = *t->filho(sudeste);
        filhoSO = *t->filho(sudoeste);
        filhoNE = *t->filho(nordeste);
        filhoNO = *t->filho(noroeste);
        int quantos = 0;
        if(filhoSE != NULL){
            quantos++;
            unicoFilho = sudeste;
        }
        if(filhoSO != NULL){
            quantos++;
            unicoFilho = sudoeste;
        }
        if(filhoNE != NULL){
            quantos++;
            unicoFilho = nordeste;
        }
        if(filhoNO != NULL){
            quantos++;
            unicoFilho = noroeste;
        }

        //Excluindo nó com apenas um ou nenhum filho
        if((quantos == 0) || (quantos == 1)){
            noQuadtree * pai;
            pai = encontrar_pai(t, raiz);

            if(pai == t){
                //Raiz da árvore, sem filhos.
                if(quantos == 0){
                    lixo = raiz;
                    raiz = NULL;
                    delete lixo;
                //Raiz da árvore com filho em um quadrante.
                } else {
                    lixo = raiz;
                    raiz = (*t->filho(unicoFilho));
                    delete lixo;
                }
            } else {
                //Nó não-raiz, sem filhos.
                if(quantos == 0){
                    (*pai->filho(pai->posicao_filho(t))) = NULL;
                    delete t;
                //Nó não-raiz com filho em um quadrante.
                } else {
                    (*pai->filho(pai->posicao_filho(t))) = *t->filho(unicoFilho);
                    delete t;
                }
            }
        //Excluindo nó com dois ou mais filhos.
        } else {
            substituto = quadtree::escolhe_candidato(t);

            posicao q, sentH, sentAH;
            //Recebendo o quadrante em que o substituto de t se encontra.
            q = t->compara_nos(*substituto);

            //Copia os valores das coordenadas do substituto para o nó a ser deletado.
            t->setX(substituto->getX());
            t->setY(substituto->getY());
            t->setNome(substituto->getNome());
            //Define os quadrantes adjacentes.
            sentH = sentidoH(q);
            sentAH = sentidoAH(q);
            //Rearranja os quadrantes adjacentes.
            adjQuad(sentH, opQuad(q), sentH, t, t);
            adjQuad(sentAH, opQuad(q), sentAH, t, t);

            noQuadtree * teste = NULL;
            teste = *t->filho(q);

            if(t->compara_nos(**t->filho(q)) != q){
                temp = *t->filho(q);
                *t->filho(q) = NULL;
                insere_no(temp->getX(), temp->getY(), temp->getNome());
                insereQuadrante(temp, t);
            } else {
                novaRaiz(q, *t->filho(q), t, t);
            }
        }

        msg.setText("A exclusão do nó " + QString::fromStdString(txt.imprime_string()) + " será concluída.");
        msg.exec();

        text->setTextColor(Qt::red);
        text->append("O nó " + QString::fromStdString(txt.imprime_string()) + " foi excluído.");
        text->setTextColor(Qt::blue);
        text->append("---Fim da Exclusão---");
        text->append("");
        text->setTextColor(Qt::black);
        return nome;
    }else{
        return "";
    }
}

std::string quadtree::exclui_no(int x, int y,QTextEdit* text,QGraphicsScene* cenaP,QGraphicsScene* cenaH, std::string& busca){
    noQuadtree *substituto, *t, *n, txt;
    std::string nome, cor;
    t = busca_no(x,y);
    if(t != NULL){
        txt = *t;
        nome = t->imprime_string();
        //Contando o número de filhos do nó a ser excluído
        noQuadtree * filhoSE, * filhoSO, * filhoNE, * filhoNO, * temp;
        posicao unicoFilho;
        filhoSE = *t->filho(sudeste);
        filhoSO = *t->filho(sudoeste);
        filhoNE = *t->filho(nordeste);
        filhoNO = *t->filho(noroeste);
        int quantos = 0;
        if(filhoSE != NULL){
            quantos++;
            unicoFilho = sudeste;
        }
        if(filhoSO != NULL){
            quantos++;
            unicoFilho = sudoeste;
        }
        if(filhoNE != NULL){
            quantos++;
            unicoFilho = nordeste;
        }
        if(filhoNO != NULL){
            quantos++;
            unicoFilho = noroeste;
        }

        //Excluindo nó com mais de um filho
        if(quantos <= 1){
            n = realca_delecao(*t, text, quantos, unicoFilho, cenaP, cenaH, busca);
        } else {
            substituto = quadtree::escolhe_candidato(t);
            n = realca_delecao(*t, text, quantos, substituto, cenaP, cenaH, busca);

            posicao q, sentH, sentAH;
            //Recebendo o quadrante em que o substituto de t se encontra.
            q = t->compara_nos(*substituto);

            //Define os quadrantes adjacentes.
            sentH = sentidoH(q);
            sentAH = sentidoAH(q);
            //Rearranja os quadrantes adjacentes.
            cor = "lightsalmon";
            adjQuad(sentH, opQuad(q), sentH, t, t, cenaP, cenaH, busca, cor);
            adjQuad(sentAH, opQuad(q), sentAH, t, t, cenaP, cenaH, busca, cor);

            noQuadtree * teste = NULL;
            teste = *t->filho(q);

            if(t->compara_nos(**t->filho(q)) != q){
                temp = *t->filho(q);
                colore_no(temp, cenaP, busca, cor);
                insereQuadrante(temp, cenaP, cenaH, busca, cor);
            } else {
                novaRaiz(q, *t->filho(q), t, t, cenaP, cenaH, busca);
            }
            cor = "green2";
            colore_no(substituto, cenaP, busca, cor);
            imprimeHierarquia(cenaH,busca);
        }

        return nome;
    }else{
        return "";
    }
}

//Rerorna o quadrante oposto(a 180º) do quadrante do no avaliado.
posicao quadtree::opQuad(posicao p){
    posicao oposta;
    switch (p){
        case (sudeste):
            oposta = noroeste;
            break;
        case (sudoeste):
            oposta = nordeste;
            break;
        case (nordeste):
            oposta = sudoeste;
            break;
        case (noroeste):
            oposta = sudeste;
            break;
    }
    return oposta;
}

/*Rearranja o subquadrante filho(f, s) do quadrante q da árvore enraizada em r.
r é o nó que será deletado. d é o quadrante em que os nós do subquadrante com
raiz em filho(f, s) podem ter de serem reinseridos. Em outros casos eles permanecem no quadrante q.*/
void quadtree::adjQuad(posicao q, posicao d, posicao s, noQuadtree* f, noQuadtree *r){
     noQuadtree * t = NULL;
     t = *f->filho(s);
     if(t == NULL){
        return;
     } else {
          if(t->compara_nos(*r) == q){
            //O nó t e os subquadrantes filho(t, q) e filho(t, opQuad(d))não precisam ser reinseridos.
            adjQuad(q, d, opQuad(q), t, r);
            adjQuad(q, d, d, t, r);
          } else {
            *f->filho(s) = NULL;
            insere_no(t->getX(), t->getY(), t->getNome());
            insereQuadrante(t, r);
          }
     }
}

void quadtree::adjQuad(posicao q, posicao d, posicao s, noQuadtree* f, noQuadtree *r, QGraphicsScene* cenaP,QGraphicsScene* cenaH, std::string& busca, std::string& cor){
     noQuadtree * t = NULL;
     t = *f->filho(s);
     if(t == NULL){
        return;
     } else {
          if(t->compara_nos(*r) == q){
            //O nó t e os subquadrantes filho(t, q) e filho(t, opQuad(d))não precisam ser reinseridos.
            adjQuad(q, d, opQuad(q), t, r, cenaP, cenaH, busca,cor);
            adjQuad(q, d, d, t, r, cenaP, cenaH, busca, cor);
          } else {
            colore_no(t, cenaP, busca, cor);
            insereQuadrante(t, cenaP, cenaH, busca, cor);
          }
     }
}

/*Rearranja o quadrante que contém o no substituto para o nó deletado - isto é,
  o quadrante q do nó r. r é o nó que será deletado. s é a raiz do subquadrante atualmente
  sendo processado e é o filho de f.*/
void quadtree::novaRaiz(posicao q, noQuadtree* s, noQuadtree* r, noQuadtree* f){
    posicao sentH, sentAH;
    sentH = sentidoH(q);
    sentAH = sentidoAH(q);
    if(*s->filho(opQuad(q)) == NULL){
        // s é o nó de substituição. Insere seus subquadrantes e deleta o pai do nó substituto.
        insereQuadrante(s, r);
        insereQuadrante(*s->filho(sentH), r);
        insereQuadrante(*s->filho(sentAH), r);
        if(r != f){
            *f->filho(opQuad(q)) = *s->filho(q);
        } else {
            *f->filho(q) = *s->filho(q);
        }

    } else {
        adjQuad(q, sentH, sentH, s, r);
        adjQuad(q, sentAH, sentAH, s, r);
        noQuadtree * temp;
        temp = *s->filho(opQuad(q));
        if(temp->compara_nos(*r) != q){
            f = *s->filho(opQuad(q));
            *s->filho(opQuad(q)) = NULL;
            insere_no(f->getX(), f->getY(), f->getNome());
            insereQuadrante(f, r);
        } else {
            novaRaiz(q, *s->filho(opQuad(q)), r, s);
        }
        delete temp;
    }
}

void quadtree::novaRaiz(posicao q, noQuadtree* s, noQuadtree* r, noQuadtree* f, QGraphicsScene* cenaP,QGraphicsScene* cenaH, std::string& busca){
    posicao sentH, sentAH;
    std::string cor;
    cor = "yellow3";
    sentH = sentidoH(q);
    sentAH = sentidoAH(q);
    if(*s->filho(opQuad(q)) == NULL){
        // s é o nó de substituiç?o. Insere seus subquadrantes e deleta o pai do nó substituto.
        insereQuadrante(s, cenaP, cenaH, busca, cor);
        insereQuadrante(*s->filho(sentH), cenaP, cenaH, busca, cor);
        insereQuadrante(*s->filho(sentAH), cenaP, cenaH, busca, cor);
    } else {
        adjQuad(q, sentH, sentH, s, r, cenaP, cenaH, busca, cor);
        adjQuad(q, sentAH, sentAH, s, r, cenaP, cenaH, busca, cor);
        noQuadtree * temp;
        temp = *s->filho(opQuad(q));
        if(temp->compara_nos(*r) != q){
            f = *s->filho(opQuad(q));
            colore_no(f, cenaP, busca, cor);
            insereQuadrante(f, cenaP, cenaH, busca, cor);
        } else {
            novaRaiz(q, *s->filho(opQuad(q)), r, s, cenaP, cenaH, busca);
        }
    }
}

//Insere os nós da subárvore com raiz em P na subárvore com raiz em R.
void quadtree::insereQuadrante(noQuadtree* p, noQuadtree* r){
    if(p != NULL && r != NULL){
        noQuadtree * filhoSE, * filhoSO, * filhoNE, * filhoNO;

        filhoSE = *p->filho(sudeste);
        filhoSO = *p->filho(sudoeste);
        filhoNE = *p->filho(nordeste);
        filhoNO = *p->filho(noroeste);

        if(filhoSE != NULL){
            *p->filho(sudeste) = NULL;
            insere_no(filhoSE->getX(), filhoSE->getY(), filhoSE->getNome());
            insereQuadrante(filhoSE, r);
        }
        if(filhoSO != NULL){
            *p->filho(sudoeste) = NULL;
            insere_no(filhoSO->getX(), filhoSO->getY(), filhoSO->getNome());
            insereQuadrante(filhoSO, r);
        }
        if(filhoNE != NULL){
            *p->filho(nordeste) = NULL;
            insere_no(filhoNE->getX(), filhoNE->getY(), filhoNE->getNome());
            insereQuadrante(filhoNE, r);
        }
        if(filhoNO != NULL){
            *p->filho(noroeste) = NULL;
            insere_no(filhoNO->getX(), filhoNO->getY(), filhoNO->getNome());
            insereQuadrante(filhoNO, r);
        }
    }
}

void quadtree::insereQuadrante(noQuadtree* p, QGraphicsScene* cenaP,QGraphicsScene* cenaH, std::string& busca, std::string& cor){
    if(p != NULL){
        noQuadtree * filhoSE, * filhoSO, * filhoNE, * filhoNO;

        filhoSE = *p->filho(sudeste);
        filhoSO = *p->filho(sudoeste);
        filhoNE = *p->filho(nordeste);
        filhoNO = *p->filho(noroeste);

        colore_no(p, cenaP, busca, cor);

        insereQuadrante(filhoSE, cenaP, cenaH, busca, cor);
        insereQuadrante(filhoSO, cenaP, cenaH, busca, cor);
        insereQuadrante(filhoNE, cenaP, cenaH, busca, cor);
        insereQuadrante(filhoNO, cenaP, cenaH, busca, cor);
    }
}

void quadtree::colore_no(noQuadtree* p, QGraphicsScene* cenaP, std::string& busca, std::string& cor ){
    std::stringstream str_dot;
    QPen pen;
    pen.setWidth(3);
    QGraphicsRectItem *ponto;

    str_dot << "\"" << p->getNome() << "\\n" << "x=" << p->getX() << "\\n" << "y=" << p->getY() << "\\n" << "\"[color=\"" << cor << "\" fontcolor=\"white\" style=\"filled\"];" << std::endl;
    busca=busca+str_dot.str();

    if(cor == "lightsalmon"){
        pen.setColor("#ffa07a");
    } else {
        if(cor == "green2"){
            pen.setColor("#00ee00");
        } else {
            pen.setColor("#cdcd00");
        }
    }

    ponto = cenaP->addRect(p->getX()-1, p->getY()-1, 2 ,2, pen);//imprime ponto substituto
    ponto->setToolTip(QString::fromStdString(p->imprime_string()));
    ponto->setZValue(2);
}
