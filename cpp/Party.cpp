
#include <vector>
#include "Party.hh"
#include <RoomVisitor.hh>
    
Party::Party(const int size, const int power) {
    for (int i = 0; i < size; i++) {
        Creature newCreature = Creature(power);
        this->append(newCreature);
    }
    priority = 0;
}

Party::~Party() {}

void Party::adjustStats() {
    int n = this->getSize();

    for (int i = 0; i < n; i++) {
        int newVal = this->getElem(i)->getAtt() * 2 / n + 1;
        this->getElem(i)->setAtt(newVal);
        newVal = this->getElem(i)->getRes() * 2 / n + 1;
        this->getElem(i)->setRes(newVal);
    }
}

void Party::append(Creature newMember) {
    this->members.push_back(newMember);
}
  
void Party::removeElem(int id) {
    this->members.erase(this->members.begin() + id - 1);
}

int Party::getSize() {
    return this->members.size();
}
    
Creature* Party::getElem(int id) {
    Creature* elem = &this->members.at(id);
    return elem;
}

void Party::show() {
    for (int i = 0; i < this->getSize(); i++) {
        Creature temp = *this->getElem(i);
        temp.show();
    }
}

void Party::access()
{
    RoomVisitor v;
    v.accessParty(this);
}
