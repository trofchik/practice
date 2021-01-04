
#pragma once
#include <iostream>
#include <Content.hh>

class Loot: public Content
{
private:
    std::string text;
public:
    Loot();
    ~Loot();
    void show();
    void access();
    std::string getText(); void setText();
};
