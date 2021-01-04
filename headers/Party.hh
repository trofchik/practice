
#pragma once
#include <Creature.hh>
#include <Exterrnals.hh>
#include <Content.hh>

class Party: public Content 
{
private:
    std::vector<Creature> members;
    
public:
    Party(const int size = 0, const int power = 0);
    ~Party();
    
    void show();
    int getSize();
    
    void append(Creature newMember);
    Creature* getElem(int id);
    void removeElem(int id);
    void adjustStats();
    void access();
};
