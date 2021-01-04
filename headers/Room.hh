
#pragma once
#include <Party.hh>
#include <Loot.hh>
#include <variant>
#include <iostream>

using ContentType = std::variant<Loot, Party>;
using HVector = std::vector<ContentType>;

class Room
{
public:
    
    Room();
    ~Room();
    void sortContByPriority();
    
private:
    HVector content;
};

