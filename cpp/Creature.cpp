
#include <iostream>
#include "Creature.hh"  

Creature::Creature(const int power) {
    this->res = rand() % (11 + power);
    this->trueRes = this->res;
    this->att = 10 + power - this->res + 1;
    
    if (this->res >= this->att * 15 / 10) {
        this->blockCh = (this->res - this->att - power * 2) * 10;
        this->blockStr = this->att;
    }
    else
        this->dodge = this->att * 2 - this->res;
}

Creature::~Creature() {}

void Creature::show() {
    std::cout << trueRes << std::endl;
    std::cout << att << std::endl;
    std::cout << blockCh << std::endl;
    std::cout << blockStr << std::endl;
    std::cout << dodge << std::endl;
    std::cout << std::endl;
}

    // Setters
void Creature::setTrueRes(int newTrueRes) { 
    if (newTrueRes <= 0) {
        std::cout << "%death message placeholder%" << std::endl;
        this->~Creature();
    }
    else
        this->trueRes = newTrueRes;  
}

void Creature::setRes(int newRes) {
    this->res = newRes;
}

void Creature::setAtt(int newAtt) {
    this->att = newAtt;
}

void Creature::setBlockStr(int newBlockStr) {     
    this->blockStr = newBlockStr; 
}

void Creature::setBlockCh(int newBlockCh) { 
    this->blockCh = newBlockCh; 
}

void Creature::setDodge(int newDodge) { 
    this->dodge = newDodge; 
}

int Creature::setParryCh(int newParryCh) {
    return this->parryCh; 
}

// Getters
int Creature::getTrueRes() { 
    return this->trueRes; 
}

int Creature::getRes() {
    return this->res; 
}

int Creature::getAtt() {
    return this->att;
}

int Creature::getBlockStr() { 
    return this->blockStr; 
}

int Creature::getBlockCh() {     
    return this->blockCh; 
}

int Creature::getDodge() {
    return this->dodge; 
}

int Creature::getParryCh() { 
    return this->parryCh; 
}
