
#pragma once
#include <Room.hh>
#include <memory>

using RoomGrid = std::unique_ptr< std::unique_ptr<std::unique_ptr<Room>[]>[] >;

class Dungeon
{
private:
    RoomGrid layout;
    unsigned int height, width;
    int entryX, entryY;
    
public:
    Dungeon(unsigned int n, unsigned int m);
    ~Dungeon();
    
    unsigned int getEntryX(); unsigned int getEntryY();
    unsigned int getHeight(); unsigned int getWidth();
    RoomGrid& getLayout();
    
    void setEntrance(int x = 0, int y = 0);
    void displayMap();

};
