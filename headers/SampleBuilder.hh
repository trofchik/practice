
#pragma once
#include <iostream>
#include <Builder.hh>
#include <unordered_map>

using CoordVector = std::vector<std::pair<int, int>>;

class SampleBuilder: public Builder
{   
private:
    Dungeon* dungeon;
    
    int lookRight(
        const std::pair<int, int> &currPos,
        bool** lockedCells
    );
    
    int randRange(const int distance);
    
    int lookLeft(
        const std::pair<int, int> &currPos,
        bool** lockedCells
    );
    
    int lookUp(
        const std::pair<int, int> &currPos,
        bool** lockedCells
    );
    
    int lookDown(
        const std::pair<int, int> &currPos,
        bool** lockedCells
    );
    
    
    bool moveRight(const std::pair<int, int> &currPos, 
                   CoordVector *line,
                   bool** lockedCells);
    
    bool moveLeft(const std::pair<int, int> &currPos, 
                  CoordVector *line, 
                  bool** lockedCells);
    
    bool moveUp(const std::pair<int, int> &currPos, 
                CoordVector *line,
                bool** lockedCells);
    
    bool moveDown(const std::pair<int, int> &currPos, 
                  CoordVector *line,
                  bool** lockedCells);
    
    bool extend(const std::pair<int, int> &currPos,
                           CoordVector *line,
                           bool** lockedCells);            
    
    bool isVertical(const CoordVector *line);
    
    bool isHorizontal(const CoordVector *line);
    
    // legacy
    void lockCellsDiagonalToPos(bool** lockedCells, const std::pair<int, int> &currPos);
    void lockCellsHorizontalToPos(bool** lockedCells, const std::pair<int, int> &currPos);
    void lockCellsVerticalToPos(bool** lockedCells, const std::pair<int, int> &currPos);
    // end legacy
    
    void scanAndLockAroundPos(bool** lockedCells, 
                              const std::pair<int, int> &currPos, 
                              Room*** tLayout);
    
    void displayLockedCellsDB(bool** lockedCells, const unsigned int H, const unsigned int W);
    
    void removeDepricatedCells(CoordVector *line, bool** lockedCells);
    
    bool isInLine(CoordVector *line, std::pair<int, int> pos);
    
    void addLineToContainer(std::vector< CoordVector > &container, 
                            CoordVector *line);
    
    std::vector<std::pair<int,int>> pickLine(
        const std::vector< CoordVector > &lineContainer);
    
public:
    SampleBuilder();
    SampleBuilder(Dungeon* dungeonInConstruction);
    ~SampleBuilder();
    
    void initNewDungeon(int threashold, int heightLimit, int widthLimit);
    void buildEntrance();
    void buildRoom(int x, int y);
    void generateLayout();
    
    void placeEnemies() {};
    void placeTraps() {};
    void placeLoot() {};
    
    Dungeon* returnResult();
};
