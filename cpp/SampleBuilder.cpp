
#include "SampleBuilder.hh"

SampleBuilder::SampleBuilder() {}


SampleBuilder::~SampleBuilder() 
{
    std::cout << "Builder has finished his work." << std::endl;
}

void SampleBuilder::initNewDungeon(
    int threashold, 
    int heightLimit, 
    int widthLimit
)
{
    int height = (rand() % heightLimit) + threashold;
    int width = (rand() % widthLimit) + threashold;
    Dungeon* temp = new Dungeon(height, width);
    this->dungeon = temp;
}


void SampleBuilder::buildEntrance()
{
    int height = this->dungeon->getHeight();
    int width = this->dungeon->getWidth();
    const int entry = rand() % ( (height + width) * 2 );
    int x, y;
    
    if (entry <= width) {
        x = entry; 
        y = 0;
    }
    else if (entry <= height + width) {
        x = width - 1; 
        y = entry - width - 1; 
    }
    else if (entry <= height + width * 2) {
        x = height + width * 2 - entry - 1; 
        y = height - 1; 
    }
    else { 
        x = 0; 
        y = (height + width) * 2 - entry - 1; 
    }
    
    this->dungeon->setEntrance(y, x);
    // this->buildRoom(y, x);
}


void SampleBuilder::buildRoom(
    int y, 
    int x
) 
{
    RoomGrid& temp = this->dungeon->getLayout();
    temp[y][x].reset( new Room() );
    std::cout << "New room was built at: " << y << ", " << x << std::endl;
}


std::pair<int, int> SampleBuilder::setCurrentPos()
{
    std::pair<int, int> currPos;
    currPos.first = this->dungeon->getEntryY();
    currPos.second = this->dungeon->getEntryX();
    return currPos;
}


bool** SampleBuilder::initLockedCellsMap()
{
    const unsigned int H = this->dungeon->getHeight();
    const unsigned int W = this->dungeon->getWidth();
    
    bool** lockedCells = new bool*[H];
    for (unsigned int i = 0; i < H; i++)
        lockedCells[i] = new bool[W];
    
    for (unsigned int i = 0; i < H; i++)
        for (unsigned int j = 0; j < W; j++)
            lockedCells[i][j] = false;

    return lockedCells;
}


void SampleBuilder::buildInitialLine(
    std::pair<int, int> &currPos,
    CoordVector *line,
    bool** lockedCells
)
{
    if (currPos.second == 0)
        moveRight(currPos, line, lockedCells);
    else if (currPos.second == this->dungeon->getWidth() - 1)
        moveLeft(currPos, line, lockedCells);
    else if (currPos.first == 0)
        moveUp(currPos, line, lockedCells);
    else
        moveDown(currPos, line, lockedCells);
}


void SampleBuilder::deleteLineFromContainer(
    std::vector<CoordVector> &lineContainer,
    const int &lineID
)
{
    CoordVector().swap(lineContainer.at(lineID));
    lineContainer.erase(lineContainer.begin() + lineID);
}


std::pair<int, int> SampleBuilder::pickPos(
    const CoordVector* line
)
{
    return line->at( rand() % line->size() );
}


bool SampleBuilder::tryExtend(
    std::pair<int, int> &currPos,
    CoordVector* line,
    bool** lockedCells,
    std::vector<CoordVector> &lineContainer,
    int &lineID
)
{
    switch(line->size())
    {
        case 0:
            deleteLineFromContainer(lineContainer, lineID);
        case 1:
        {
            currPos = line->at(0); 
            if (extend(currPos, line, lockedCells)) 
                return true;
            else
            {
                deleteLineFromContainer(lineContainer, lineID);
                return false;
            }
        }
    }
}


void SampleBuilder::generateLayout()
{
    const unsigned int H = this->dungeon->getHeight();
    const unsigned int W = this->dungeon->getWidth();
    
    std::pair<int, int> currPos = setCurrentPos();
    std::cout << "Entry coords are: " << currPos.first << " " << currPos.second << std::endl;
    
    bool** lockedCells = initLockedCellsMap();        
    
    CoordVector *line = new CoordVector();
    CoordVector *newLine = new CoordVector();
    std::vector<CoordVector> lineContainer;

    buildInitialLine(currPos, line, lockedCells);
    
    lineContainer.push_back(*line);
    
    int seed;
    std::cout << "Please input desired generation seed: "; 
    std::cin >> seed;
    
    for (int i = 0; i < 40; i++)
    {
        std::cout << ">> " << i << "\n";
        
        displayLineContainerContents(lineContainer);
        
        srand(seed + i);
        
        int lineID = pickLine(lineContainer); 
        line = &lineContainer.at(lineID); 
        
        if (line->size() > 1) // pickPos()
            currPos = pickPos(line);
        else
        {
            if ( tryExtend(currPos, line, lockedCells, lineContainer, lineID) )
                currPos = pickPos(line);
            else
            {
                std::cout << "Skipping iteration\n\n\n";
                continue;
            }   
        }
        
        std::cout << "Attempting to generate new line at coordinate " 
            << currPos.first << ", " << currPos.second << std::endl;
        
        newLine = new CoordVector();
        
        if ( isHorizontal(line) )
        {
            moveUp(currPos, newLine, lockedCells);
            if (newLine->size() > 0)
                newLine->erase(newLine->begin());
            moveDown(currPos, newLine, lockedCells);
        }
        else if (isVertical(line))
        {
            moveLeft(currPos, newLine, lockedCells);
            if (newLine->size() > 0)
                newLine->erase(newLine->begin());
            moveRight(currPos, newLine, lockedCells); 
        }

        removeDepricatedCells(newLine, lockedCells); 
        removeDepricatedCells(line, lockedCells); 
        
        if (newLine->size() != 0)
            addLineToContainer(lineContainer, newLine);
        
        if (line->size() == 0)
        {
            deleteLineFromContainer(lineContainer, lineID);
        }
        
        this->dungeon->displayMap();
        std::cout << "\n";
        displayLockedCellsDB(lockedCells, H, W);
        std::cout << "\n";
    }
    
    std::cout << "End result:-----------------------------------------------------" << "\n";
    displayLockedCellsDB(lockedCells, H, W);
}

/* 
 * <Unimportant>
 * TODO: Using ifdef add killswitch for debuging features.
 * TODO: Consider doing something with unjust cell deprication.
 *        NOTE0: When cell is depricated sometimes algorithm only considers one direction it can be 
 *        built towards. 
 *        Example:
 *        --[]----
 *        --[][][]
 *        --[]----
 *        Here (1, 1) will get depricated although we can build from it to the left to (1, 0).
 *        UPD: (1, 1) is the end of horizontal line. Make algorithm consider line endings when depricating;
 * 
 * <Important>
 * TODO: In move methods make use of emplace so that cells coordinates are at correct order.
 * 
 * TODO: Refactoring
 *       1) Moving to private methods that should be there.
 *       4) Simplify the code where possible.
 *       6) Code proper destructor.
 *
 * TODO: Keep debugging this sucker.
 * 
 * <Notes>
 * NOTE: 3 in a block rule is that a square block of 4 cells should have no more than
 *        3 initialized cells. 
 */



bool SampleBuilder::isInLine( // Removes from line coordinates matching given.
    CoordVector *line, 
    std::pair<int, int> pos
)  
{
    for (int i = 0; i < line->size(); i++)
        if (pos == line->at(i))
            line->erase(line->begin() + i);
}


void SampleBuilder::addLineToContainer(
    std::vector<CoordVector> &container, 
    CoordVector *line
)
{
    container.emplace(container.end());
    container.at(container.size() - 1) = *line;
}


int SampleBuilder::pickLine(
    const std::vector<CoordVector> &lineContainer
)
{
    if (lineContainer.size() > 0)
        return rand() % lineContainer.size();
    else
        throw "No lines to be built upon";
}


int SampleBuilder::randRange(
    const int distance
)
{
    switch (distance) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2: 
            return rand() % 2 + 1;
        default:
            return rand() % (distance - 1) + 1;
    }
}


int SampleBuilder::getFreeRight(
    const std::pair<int, int> &currPos
)
{
    return this->dungeon->getWidth() - currPos.second - 1;
}


int SampleBuilder::getFreeLeft(
    const std::pair<int, int> &currPos
) 
{
    return currPos.second;
}


int SampleBuilder::getFreeUp(
    const std::pair<int, int> &currPos
) 
{
    return this->dungeon->getHeight() - currPos.first - 1;
}


int SampleBuilder::getFreeDown(
    const std::pair<int, int> &currPos
) 
{
    return currPos.first;
}


int SampleBuilder::lookRight( 
    const std::pair<int, int> &currPos, 
    bool** lockedCells 
)
{
    const int R = getFreeRight(currPos); 
    if (R == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    int roomsInARow = 0;
    int res = 0;
    
    for (int i = currPos.second; i < this->dungeon->getWidth(); i++)
    {
        if (lockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    
    return res;
}


bool SampleBuilder::buildRight(
    const std::pair<int, int> &currPos,
    CoordVector *line,
    bool** lockedCells,
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout(); 
        
    for (int i = currPos.second; i < range; i++)
    {
        if (lockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            this->buildRoom(currPos.first, i);
        }
        
        std::pair<int, int> pos(currPos.first, i);
        scanAndLockAroundPos(lockedCells, pos, tLayout);
        line->push_back(pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveRight(
    const std::pair<int, int> &currPos, 
    CoordVector *line, 
    bool** lockedCells
) 
{
    isInLine(line, currPos); 
        
    int R = lookRight(currPos, lockedCells);
    int range = randRange(R) + currPos.second;
    
    if (range > this->dungeon->getWidth() - 1)
        range = this->dungeon->getWidth() - 1;
    
    if (buildRight(currPos, line, lockedCells, range))
        return true;
    else
        return false;
}


int SampleBuilder::lookLeft(
    const std::pair<int, int> &currPos, 
    bool** lockedCells
)
{
    const int L = getFreeLeft(currPos); 
    if (L == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    int roomsInARow = 0;
    int res = 0;
    
    for (int i = currPos.second; i >= 0; i--)
    {
        if (lockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    
    return res;
}


bool SampleBuilder::buildLeft(
    const std::pair<int, int>& currPos, 
    CoordVector* line, bool ** lockedCells, 
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout();
    
    for (int i = currPos.second; i >= range; i--)
    {
        if (lockedCells[currPos.first][i])
            break;
        
        if (tLayout[currPos.first][i] == nullptr)
        {
            this->buildRoom(currPos.first, i);
        }
        
        std::pair<int, int> pos(currPos.first, i);
        scanAndLockAroundPos(lockedCells, pos, tLayout);
        line->push_back(pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveLeft(
    const std::pair<int, int> &currPos, 
    CoordVector *line, 
    bool** lockedCells
) 
{
    isInLine(line, currPos);
        
    int L = lookLeft(currPos, lockedCells);
    int range = currPos.second - randRange(L);
    
    if (range < 0)
        range = 0;
    
    if (buildLeft(currPos, line, lockedCells, range))
        return true;
    else
        return false;
}


int SampleBuilder::lookUp(
    const std::pair<int, int> &currPos, 
    bool** lockedCells
)
{
    const int U = getFreeUp(currPos);
    if (U == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    int roomsInARow = 0;
    int res = 0;
    
    for (int j = currPos.first; j < this->dungeon->getHeight(); j++)
    {
        if (lockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    return res;
}


bool SampleBuilder::buildUp(
    const std::pair<int, int>& currPos, 
    CoordVector* line, 
    bool ** lockedCells, 
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout();
    
    isInLine(line, currPos);
    
    for (int j = currPos.first; j < range; j++)
    {
        if (lockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            this->buildRoom(j, currPos.second);
        }
        
        std::pair<int, int> pos(j, currPos.second);
        scanAndLockAroundPos(lockedCells, pos, tLayout);
        line->push_back(pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveUp(
    const std::pair<int, int> &currPos, 
    CoordVector *line, 
    bool** lockedCells 
) 
{
    int U = lookUp(currPos, lockedCells); 
    int range = currPos.first + randRange(U);
    
    if (range > this->dungeon->getHeight() - 1)
        range = this->dungeon->getHeight() - 1;    
    
    if (buildUp(currPos, line, lockedCells, range))
        return true;
    else
        return false;
}


int SampleBuilder::lookDown(
    const std::pair<int, int> &currPos, 
    bool **lockedCells
)
{
    const int D = getFreeDown(currPos);
    if (D == 0)
        return 0;
    
    RoomGrid& tLayout = this->dungeon->getLayout();
    int roomsInARow = 0;
    int res = 0;
    
    for (int j = currPos.first; j >= 0; j--)
    {
        if (lockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            roomsInARow = 0;
        }
        else
        {
            roomsInARow++;
            if (roomsInARow > 1)
                break;
        }
        res++;
    }
    return res;
}


bool SampleBuilder::buildDown(
    const std::pair<int, int>& currPos, 
    CoordVector* line, 
    bool ** lockedCells, 
    const int range
)
{
    RoomGrid& tLayout = this->dungeon->getLayout();
    
    for (int j = currPos.first; j >= range; j--)
    {
        if (lockedCells[j][currPos.second])
            break;
        
        if (tLayout[j][currPos.second] == nullptr)
        {
            this->buildRoom(j, currPos.second);
        }
            
        std::pair<int, int> pos(j, currPos.second);
        scanAndLockAroundPos(lockedCells, pos, tLayout);
        line->push_back(pos);
    }
    
    if (line->size() < 2)
        return false;
    else
        return true;
}


bool SampleBuilder::moveDown(
    const std::pair<int, int> &currPos, 
    CoordVector *line, 
    bool** lockedCells) 
{
    int D = lookDown(currPos, lockedCells);
    int range = currPos.first - randRange(D);
    
    if (range < 0)
        range = 0;
    
    isInLine(line, currPos);
    
    if (buildDown(currPos, line, lockedCells, range))
        return true;
    else
        return false;
}


bool SampleBuilder::extend(
    const std::pair<int, int> &currPos, 
    CoordVector *line, 
    bool** lockedCells
)
{
    std::cout << "Attempting to extend at coordinate " << currPos.first << ", " << currPos.second << std::endl;
    
    const int R = getFreeRight(currPos);
    const int L = getFreeLeft(currPos);
    const int U = getFreeUp(currPos);
    const int D = getFreeDown(currPos);
    
    bool right = false, left = false, up = false, down = false;
    // create copy of line
    RoomGrid& tLayout = this->dungeon->getLayout();
    
    if (R != 0 || L != 0)
    {
        if (R != 0 && tLayout[currPos.first][currPos.second + 1] == nullptr)
            right = moveRight(currPos, line, lockedCells);
        if (L != 0 && tLayout[currPos.first][currPos.second - 1] == nullptr)
            left = moveLeft(currPos, line, lockedCells);
    }
    else
    {
        if (U != 0 && tLayout[currPos.first + 1][currPos.second] == nullptr)
            up = moveUp(currPos, line, lockedCells); 
        if (D != 0 && tLayout[currPos.first - 1][currPos.second] == nullptr)
            down = moveDown(currPos, line, lockedCells);
    }
    
    if ( (right && left) || (up && down) )
        lockedCells[currPos.first][currPos.second] = true;
        
    if (right || left || up || down)
    {
        std::cout << "Extension succesful!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Extension failed!" << std::endl;
        return false;
    }
}


bool SampleBuilder::isHorizontal(
    const CoordVector *line
)
{
    if (line->size() < 2)
    {
        std::cout << "Line's length is less than 2." << std::endl;
        return false;
    }
    
    if (line->at(0).second != line->at(1).second)
        return true;
    return false;
}


bool SampleBuilder::isVertical(
    const CoordVector *line
)
{
    if (line->size() < 2)
    {
        std::cout << "Line's length is less than 2. " << std::endl;
        return false;
    }
    if (line->at(0).first != line->at(1).first)
        return true;
    return false;
}


void SampleBuilder::scanAndLockAroundPos(
    bool** lockedCells, 
    const std::pair<int, int> &currPos, 
    RoomGrid& tLayout
)
{
    const int R = getFreeRight(currPos);
    const int L = getFreeLeft(currPos);
    const int U = getFreeUp(currPos);
    const int D = getFreeDown(currPos);
    
    int globalCount = 0;
    
    if (R != 0)
    {
        if (U != 0)
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first + 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second };
                
            if (tLayout[currPos.first + 1][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second + 1 };
                
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (tLayout[currPos.first][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second + 1 };
                
            if (count == 3)
            {
                lockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
        
        if ( D != 0 )
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (tLayout[currPos.first][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second + 1 };
                
            if (tLayout[currPos.first - 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second };
                
            if (tLayout[currPos.first - 1][currPos.second + 1] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second + 1 };
                
            if (count == 3)
            {
                lockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
    }
    
    if (L != 0)
    {
        if (U != 0)
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first + 1][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second - 1 };
                
            if (tLayout[currPos.first + 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first + 1, currPos.second };
                
            if (tLayout[currPos.first][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second - 1 };
                
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (count == 3)
            {
                lockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
        
        if ( D != 0 )
        {
            std::pair<int, int> empty;
            
            unsigned int count = 0;
            if (tLayout[currPos.first][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second - 1 };
                
            if (tLayout[currPos.first][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first, currPos.second };
                
            if (tLayout[currPos.first - 1][currPos.second - 1] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second - 1 };
                
            if (tLayout[currPos.first - 1][currPos.second] != nullptr)
                count++;
            else
                empty = { currPos.first - 1, currPos.second };
                
            if (count == 3)
            {
                lockedCells[empty.first][empty.second] = true;
                globalCount++;
            }
        }
    }
    
    if (globalCount == 4)
    {
        lockedCells[currPos.first][currPos.second] = true;
        //lockCellsHorizontalToPos(lockedCells, currPos);
        //lockCellsVerticalToPos(lockedCells, currPos);
    }
}


void SampleBuilder::lockCellsDiagonalToPos(
    bool** lockedCells, 
    const std::pair<int, int> &currPos
)
{
    const int R = getFreeRight(currPos);
    const int L = getFreeLeft(currPos);
    const int U = getFreeUp(currPos);
    const int D = getFreeDown(currPos);
    
    if (R != 0)
    {
        if (U != 0)
            lockedCells[currPos.first + 1][currPos.second + 1] = true;
        if (D != 0)
            lockedCells[currPos.first - 1][currPos.second + 1] = true;
    }
    
    if (L != 0)
    {
        if (U != 0)
            lockedCells[currPos.first + 1][currPos.second - 1] = true;
        if (D != 0)
            lockedCells[currPos.first - 1][currPos.second - 1] = true;
    }
    
    lockedCells[currPos.first][currPos.second] = true;
}


void SampleBuilder::lockCellsHorizontalToPos(
    bool** lockedCells, 
    const std::pair<int, int> &currPos
)
{
    const int R = getFreeRight(currPos);
    const int L = getFreeLeft(currPos);
    
    if (R != 0)
    {
        lockedCells[currPos.first][currPos.second + 1] = true;
    }
    
    if (L != 0)
    {
        lockedCells[currPos.first][currPos.second - 1] = true;
    }
}


void SampleBuilder::lockCellsVerticalToPos(
    bool** lockedCells, 
    const std::pair<int, int> &currPos
)
{
    const int U = getFreeUp(currPos);
    const int D = getFreeDown(currPos);
    
    if (U != 0)
        lockedCells[currPos.first + 1][currPos.second] = true;
    
    if (D != 0)
        lockedCells[currPos.first - 1][currPos.second] = true;
}


void SampleBuilder::displayLockedCellsDB(
    bool** lockedCells, 
    const unsigned int H, 
    const unsigned int W
)
{
        for (unsigned int i = 0; i < H; i++)
        {
            for (unsigned int j = 0; j < W; j++)
                if (lockedCells[i][j])
                    std::cout << "L";
                else
                    std::cout << "-";
                    
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
        std::cout << std::endl;
}


void SampleBuilder::displayLineContainerContents(
    std::vector<CoordVector>& lineContainer
)
{
    for (int i = 0; i < lineContainer.size(); i++)
    {
        std::cout << i << ": ";
        for (int j = 0; j < lineContainer[i].size(); j++)
            std::cout << "(" << lineContainer[i].at(j).first << ";" << lineContainer[i].at(j).second << ") ";
        std::cout << "\n";
    }
}





void SampleBuilder::removeDepricatedCells(
    CoordVector *line, 
    bool** lockedCells
)
{
    for (unsigned int i = 0; i < line->size(); i++)
    {
        if (lockedCells[line->at(i).first][line->at(i).second])
            line->erase(line->begin() + i);
    }
}


Dungeon* SampleBuilder::returnResult() { return this->dungeon; }

