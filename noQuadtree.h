#ifndef NOQUADTREE_H
#define NOQUADTREE_H

#include <string>

enum posicao {sudeste, sudoeste, noroeste, nordeste};
/*Define a estrutura de um n� em uma Quadtree que cont�m oito campos a saber:
As quatro dire��es poss�veis para filhos, as suas coordenadas x e y,
o nome do n� e um ponteiro para lista de colis�o. A nossa
implementa��o n�o utilizar� lista de colis�o.*/
struct dados {
    int x, y;
    std::string nome;
    int* listaPtr;
};

class noQuadtree{
private:
    dados val;
    noQuadtree * se, * so, * no, * ne;
public:
    noQuadtree();
    noQuadtree(int,int);
    noQuadtree(int,int,std::string);
    virtual ~noQuadtree();
    void setVal(dados);
    dados getVal();
    void setNome(std::string nome);
    std::string getNome();
    void setX(int x);
    int getX();
    void setY(int y);
    int getY();
    void setSe(noQuadtree*);
    noQuadtree* getSe();
    void setSo(noQuadtree*);
    noQuadtree* getSo();
    void setNo(noQuadtree*);
    noQuadtree* getNo();
    void setNe(noQuadtree*);
    noQuadtree* getNe();
    bool coord_iguais(noQuadtree*);
    posicao compara_nos(noQuadtree);
    noQuadtree** filho(posicao);
    //noQuadtree** filho(noQuadtree, posicao);
    posicao posicao_filho(noQuadtree*);
    noQuadtree incrementa_coord(int);
    noQuadtree decrementa_coord(int);
    void imprime();
    std::string imprime_string();
};

#endif // NOQUADTREE_H
