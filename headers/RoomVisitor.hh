
#pragma once
#include <iostream>
#include <Loot.hh>
#include <Party.hh>

struct RoomVisitor
{
    void accessLoot(Loot* loot)
    {
        std::cout << loot->getText() << std::endl;
    }
    
    void accessParty(Party* data)
    {
        data->show();
    }
};
