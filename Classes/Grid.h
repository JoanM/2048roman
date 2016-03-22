//
//  Grid.h
//  roman2048
//
//  Created by Joan Martinez on 17/04/15.
//
//

#ifndef roman2048_Grid_h
#define roman2048_Grid_h

#include "cocos2d.h"
#include "2d/CCNode.h"
#include "Utils.h"
#include <iostream>
using namespace cocos2d;

class MoveFinishedData;
class NumberCardSprite;

class Grid : public cocos2d::Node
{
public:
    CREATE_FUNC(Grid);
    
    ~Grid() = default;
    bool init();
    Vector<FiniteTimeAction *> move(ESwapDirection swapDirection, unsigned int &numberOfPoints);
    bool move(ESwapDirection swapDirection, std::function<void (const MoveFinishedData &)> onMoveFinished);
    bool isMovementPossible();
    void spawnCell();
    cocos2d::Data serialize();
    void restore(const cocos2d::Data &data);
    
private:
    void createBackground();
    void createCellAtSpecificPosition(unsigned int row, unsigned int col, unsigned short value);
    Vector<FiniteTimeAction*> move(unsigned int edge, short increase, bool swap, unsigned int &numberOfPoints);
    unsigned int getXPosition(unsigned int x);
    unsigned int getYPosition(unsigned int y);
    
    unsigned int m_tileWidth {0};
    unsigned int m_tileHeight {0};
    NumberCardSprite* m_cardArray[4][4] {};
    unsigned short m_numberFullCells {0};
    bool m_hasArrivedTo2048 {false};
};

#endif
