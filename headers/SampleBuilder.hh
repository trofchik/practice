
#pragma once
#include <iostream>
#include <Builder.hh>
#include <unordered_map>

using CoordVector = std::vector<std::pair<int, int>>;

class SampleBuilder: public Builder
{   
private:
    Dungeon* dungeon;
    
    void buildRoom(
        int y, 
        int x
    );
    
    std::pair<int, int> setCurrentPos();
    
    bool** initLockedCellsMap();
    
    void buildInitialLine(
        std::pair<int, int> &currPos,
        CoordVector *line,
        bool** lockedCells
    );
    
    std::pair<int, int> pickPos(
        const CoordVector* line
    );
    
    bool tryExtend(
        std::pair<int, int> &currPos,
        CoordVector* line,
        bool** lockedCells,
        std::vector<CoordVector> &lineContainer,
        int &lineID
    );
    
    int randRange(
        const int distance
    );
    
    int getFreeRight(
        const std::pair<int, int> &currPos
    );
    
    int getFreeLeft(
        const std::pair<int, int> &currPos
    );
    
    int getFreeUp(
        const std::pair<int, int> &currPos
    );
    
    int getFreeDown(
        const std::pair<int, int> &currPos
    );
    
    int lookRight(
        const std::pair<int, int> &currPos,
        bool** lockedCells
    );
    
    bool buildRight(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        bool** lockedCells,
        const int range
    );
    
    bool moveRight(
        const std::pair<int, int> &currPos, 
        CoordVector *line,
        bool** lockedCells
    );
    
    
    int lookLeft(const std::pair<int, int> &currPos,
                 bool** lockedCells
    );
    
    bool buildLeft(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        bool** lockedCells,
        const int range
    );
    
    bool moveLeft(
        const std::pair<int, int> &currPos, 
        CoordVector *line, 
        bool** lockedCells
    );
    

    int lookUp(const std::pair<int, int> &currPos,
               bool** lockedCells
    );
    
    bool buildUp(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        bool** lockedCells,
        const int range
    );
    
    bool moveUp(
        const std::pair<int, int> &currPos, 
        CoordVector *line,
        bool** lockedCells
    );
    
    
    int lookDown(
        const std::pair<int, int> &currPos,
        bool** lockedCells
    );
    
    bool buildDown(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        bool** lockedCells,
        const int range
    );
    
    bool moveDown(
        const std::pair<int, int> &currPos, 
        CoordVector *line,
        bool** lockedCells
    );
    
    
    bool extend(
        const std::pair<int, int> &currPos,
        CoordVector *line,
        bool** lockedCells
    );            
    
    
    bool isVertical(
        const CoordVector *line
    );
    
    bool isHorizontal(
        const CoordVector *line
    );
    
    
    // legacy
    void lockCellsDiagonalToPos(
        bool** lockedCells, 
        const std::pair<int, int> &currPos
    );
    
    void lockCellsHorizontalToPos(
        bool** lockedCells, 
        const std::pair<int, int> &currPos
    );
    
    void lockCellsVerticalToPos(
        bool** lockedCells, 
        const std::pair<int, int> &currPos
    );
    // end legacy
    
    
    void scanAndLockAroundPos(
        bool** lockedCells, 
        const std::pair<int, int> &currPos, 
        RoomGrid& tLayout
    );
    
    
    void displayLockedCellsDB(
        bool** lockedCells, 
        const unsigned int H, 
        const unsigned int W
    );
    
    void displayLineContainerContents(
        std::vector<CoordVector> &lineContainer
    );
    
    void removeDepricatedCells(
        CoordVector *line, 
        bool** lockedCells
    );
    
    bool isInLine(CoordVector *line, std::pair<int, int> pos);
    
    void addLineToContainer(
        std::vector< CoordVector > &container, 
        CoordVector *line
    );
    
    void deleteLineFromContainer(
        std::vector<CoordVector> &lineContainer,
        const int &lineID
    );
    
    int pickLine(
        const std::vector<CoordVector> &lineContainer
    );
    
public:
    SampleBuilder();
    //SampleBuilder(Dungeon* dungeonInConstruction);
    ~SampleBuilder();
    
    void initNewDungeon(
        int threashold, 
        int heightLimit, 
        int widthLimit
    );
    
    void buildEntrance();
    
    void generateLayout();
    
    void placeEnemies() {};
    
    void placeTraps() {};
    
    void placeLoot() {};
    
    Dungeon* returnResult();
};
