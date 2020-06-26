#ifndef QUADTREE_H
#define QUADTREE_H

#include "noQuadtree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QTextEdit>
#include <QGraphicsScene>

class quadtree
{
public:
    quadtree();
    quadtree(noQuadtree*);
    virtual ~quadtree();
    void insere_no(int,int,std::string);
    void insere_no(int,int,std::string,QGraphicsScene*,QGraphicsScene*);
    void colore_no(noQuadtree*, QGraphicsScene*, std::string&, std::string&);
    std::string exclui_no(int,int,QTextEdit*,QGraphicsScene*,QGraphicsScene*);
    std::string exclui_no(int,int,QTextEdit*,QGraphicsScene*,QGraphicsScene*, std::string&);
    void apagar_arvore();
    void imprime();
    noQuadtree* busca_no(int,int);
    void busca_vizinhanca(int,int,int,QTextEdit*,QGraphicsScene*,std::string&);
    void busca_regiao(int,int,int,int,QTextEdit*,QGraphicsScene*,std::string&);
    void gera_quadtree_randomica(int);
    void gera_dot_arquivo();
    void gera_dot_arquivo_recursao(std::ofstream&);
    void imprime_no(int,int,QTextEdit*,QGraphicsScene*,std::string&);
    noQuadtree* encontra_candidato(noQuadtree*, posicao);
    noQuadtree* escolhe_candidato(noQuadtree*);
    posicao sentidoH(posicao);
    posicao sentidoAH(posicao);
    posicao opQuad(posicao);
    void adjQuad(posicao, posicao, posicao, noQuadtree*, noQuadtree*);
    void adjQuad(posicao, posicao, posicao, noQuadtree*, noQuadtree*, QGraphicsScene*, QGraphicsScene*, std::string&, std::string&);
    void novaRaiz(posicao , noQuadtree* , noQuadtree* , noQuadtree*);
    void novaRaiz(posicao , noQuadtree* , noQuadtree* , noQuadtree*, QGraphicsScene*, QGraphicsScene*, std::string&);
    void insereQuadrante(noQuadtree*, noQuadtree*);
    void insereQuadrante(noQuadtree*, QGraphicsScene*, QGraphicsScene*, std::string&, std::string&);
    std::string gera_dot_string(std::string=" ");
    void imprimeHierarquia(QGraphicsScene*,std::string=" ");
    void imprimeParticionamento(QGraphicsScene*);
    void string_pontos(QString&);
private:
    noQuadtree * raiz;
    noQuadtree* encontrar_pai(noQuadtree*,noQuadtree*);
    noQuadtree* realca_delecao(noQuadtree, QTextEdit*, int, posicao, QGraphicsScene*,QGraphicsScene*,std::string& busca);
    noQuadtree* realca_delecao(noQuadtree, QTextEdit*, int, noQuadtree*, QGraphicsScene*,QGraphicsScene*,std::string& busca);
    bool verifica_circulo(noQuadtree, int);
    bool verifica_retangulo(noQuadtree,noQuadtree);
    std::string gera_dot_string_recursao(std::string&);
};

#endif // QUADTREE_H
