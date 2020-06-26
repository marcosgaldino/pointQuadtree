#include "noQuadtree.h"
#include <iostream>
#include <string>
#include <sstream>

noQuadtree::noQuadtree(){
}

noQuadtree::noQuadtree(int x,int y){
    val.x = x;
    val.y = y;
}

noQuadtree::noQuadtree(int x,int y,std::string nome){
    val.x = x;
    val.y = y;
    val.nome = nome;
}

noQuadtree::~noQuadtree(){
    //dtor
}

void noQuadtree::setVal(dados val){
    this->val = val;
}

dados noQuadtree::getVal(){
    return this->val;
}

void noQuadtree::setNome(std::string nome){
    this->val.nome = nome;
}

std::string noQuadtree::getNome(){
    return this->val.nome;
}

void noQuadtree::setX(int x){
    this->val.x = x;
}

int noQuadtree::getX(){
    return this->val.x;
}

void noQuadtree::setY(int y){
    this->val.y = y;
}

int noQuadtree::getY(){
    return this->val.y;
}

void noQuadtree::setSe(noQuadtree* se){
    this->se = se;
}

noQuadtree* noQuadtree::getSe(){
    return this->se;
}

void noQuadtree::setSo(noQuadtree* so){
    this->so = so;
}

noQuadtree* noQuadtree::getSo(){
    return this->so;
}

void noQuadtree::setNe(noQuadtree* ne){
    this->ne = ne;
}

noQuadtree* noQuadtree::getNe(){
    return this->ne;
}

void noQuadtree::setNo(noQuadtree* no){
    this->no = no;
}

noQuadtree* noQuadtree::getNo(){
    return this->no;
}

//Recebe dois nós e retorna se eles têm coordenadas iguais ou não
bool noQuadtree::coord_iguais(noQuadtree * p){
    if (p == NULL && this == NULL){
        return true;
    } else if (p != NULL && this != NULL && (p->val.x == this->val.x) && (p->val.y == this->val.y)){
        return true;
    } else {
        return false;
    }
}

//Retorna o quadrante da point quadtree com raiz no nó r a que o nó P pertence
posicao noQuadtree::compara_nos(noQuadtree p){
    if (p.val.x < this->val.x){
        if (p.val.y < this->val.y) {
            return sudoeste;
        } else {
            return noroeste;
        }
    } else {
        if (p.val.y < this->val.y) {
            return sudeste;
        } else {
            return nordeste;
        }
    }
}

//Recebe nó pai e retorna o nó filho a uma determinada posição
noQuadtree** noQuadtree::filho(posicao p){
    noQuadtree ** f;
    switch (p){
        case (sudeste):
            f = &this->se;
            break;
        case (sudoeste):
            f = &this->so;
            break;
        case (nordeste):
            f = &this->ne;
            break;
        case (noroeste):
            f = &this->no;
            break;
    }
    return f;
}

//Recebe os nós pai e filho e retorna a posição do nó filho em relação ao nó pai
posicao noQuadtree::posicao_filho(noQuadtree * p) {
    if (this->getSe()->coord_iguais(p)) {
        return sudeste;
    }  else if (this->getSo()->coord_iguais(p)) {
        return sudoeste;
    }  else if (this->getNe()->coord_iguais(p)) {
        return nordeste;
    }  else  {
        return noroeste;
    }
}

noQuadtree noQuadtree::incrementa_coord(int r){
    this->val.x += r;
    this->val.y += r;
    return *this;
}

noQuadtree noQuadtree::decrementa_coord(int r){
    this->val.x -= r;
    this->val.y -= r;
    return *this;
}

void noQuadtree::imprime(){
    std::cout << this->val.nome << " " << this->val.x << " " << this->val.y;
}

std::string noQuadtree::imprime_string(){
    std::stringstream saida;
    saida << this->val.nome << " (" << this->val.x << "," << this->val.y <<")";
    return saida.str();
}
