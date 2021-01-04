
#include <iostream>
#include <Loot.hh>
#include <RoomVisitor.hh>

Loot::Loot()
{
    this->text = "penis";
}

Loot::~Loot() {}

void Loot::access()
{
    RoomVisitor v;
    v.accessLoot(this);
}

std::string Loot::getText() { return this->text; }

void Loot::show()
{
    std::cout << this->text << std::endl;
}
