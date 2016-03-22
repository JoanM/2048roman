//
//  Grid.cpp
//  roman2048
//
//  Created by Joan Martinez on 17/04/15.
//
//

#include <stdio.h>
#include <algorithm>
#include <random>
#include "NumberCardSprite.h"
#include "Grid.h"
#include "Constants.h"
#include <vector>
#include <array>

constexpr int NUMBER_OF_COLUMNS_ROWS = 4;
constexpr int NUMBER_OF_CELLS = NUMBER_OF_COLUMNS_ROWS*NUMBER_OF_COLUMNS_ROWS;

static constexpr unsigned int MARGIN_BETWEEN_ROWS = 8;
static constexpr unsigned int MARGIN_BETWEEN_COLUMNS = 8;
static constexpr unsigned int LEFT_MARGIN = 15;
static constexpr unsigned int RIGHT_MARGIN = LEFT_MARGIN;

using namespace cocos2d;
using namespace std;

bool Grid::init()
{
    if(!Node::init())
    {
        return false;
    }
    
    auto maxTileWidth = (Director::getInstance()->getVisibleSize().width - MARGIN_BETWEEN_COLUMNS*3 - LEFT_MARGIN - RIGHT_MARGIN)/4;
    auto maxTileHeight = ((Director::getInstance()->getVisibleSize().height - MARGIN_BETWEEN_COLUMNS*3 - LEFT_MARGIN - RIGHT_MARGIN)*3/5)/4;
    
    m_tileWidth = min(maxTileHeight, maxTileWidth);
    m_tileHeight = m_tileWidth;
    
    createBackground();
    
    return true;
}

void Grid::createBackground(){
    
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            auto cellBackground = LayerColor::create(Color4B(200, 190, 180, 255),m_tileWidth, m_tileHeight);
            Vec2 v(m_tileWidth*i + MARGIN_BETWEEN_COLUMNS*i, m_tileHeight*j + MARGIN_BETWEEN_ROWS*j);
            cellBackground->setPosition(v);
            addChild(cellBackground);
        }
    }
    
    this->setContentSize({static_cast<float>(m_tileWidth * NUMBER_OF_COLUMNS_ROWS + MARGIN_BETWEEN_COLUMNS * (NUMBER_OF_COLUMNS_ROWS - 1)),static_cast<float>(m_tileHeight * NUMBER_OF_COLUMNS_ROWS + MARGIN_BETWEEN_ROWS * (NUMBER_OF_COLUMNS_ROWS - 1))});
}

unsigned int Grid::getXPosition(unsigned int x)
{
    return x*m_tileWidth + x*MARGIN_BETWEEN_COLUMNS;
}

unsigned int Grid::getYPosition(unsigned int y)
{
    return y*m_tileHeight + y*MARGIN_BETWEEN_ROWS;
}

void Grid::spawnCell(){
    
    bool emptyCell = false;
    unsigned int xCell = 0;
    unsigned int yCell = 0;
    while(!emptyCell){
        xCell = arc4random() % NUMBER_OF_COLUMNS_ROWS;
        yCell = arc4random() % NUMBER_OF_COLUMNS_ROWS;
        emptyCell = m_cardArray[xCell][yCell] == nullptr;
    }
    
    unsigned int x = getXPosition(yCell);
    unsigned int y = getYPosition(xCell);
    
    NumberCardSprite::create(x, y, Size(0,0), 1);
    
    m_cardArray[xCell][yCell] = NumberCardSprite::create(x,y, Size(static_cast<float>(m_tileWidth),
                                                                   static_cast<float>(m_tileHeight))
                                                                   , arc4random() % 5 != 0 ? 1 : 2);
    
    this->addChild(m_cardArray[xCell][yCell]);
    m_cardArray[xCell][yCell]->setOpacity(0);
    m_cardArray[xCell][yCell]->setCascadeOpacityEnabled(true);
    m_cardArray[xCell][yCell]->runAction(Sequence::create(ScaleTo::create(0, 0.5),
                                                          Spawn::create(ScaleTo::create(Animations::Duration::creationDuration, 1), FadeIn::create(Animations::Duration::creationDuration), NULL)
                                                          , NULL));
    ++m_numberFullCells;
}

bool Grid::move(ESwapDirection swapDirection, std::function<void (const MoveFinishedData &)> onMoveFinished)
{
    unsigned int numberOfPoints {0};
    Vector<FiniteTimeAction *> actions = this->move(swapDirection, numberOfPoints);
    MoveFinishedData finishedData {};
    finishedData.hasArrivedTo2048 = m_hasArrivedTo2048;
    finishedData.hasBeenMoved = actions.size();
    finishedData.numberOfPoints = numberOfPoints;
    
    auto seq = Sequence::create(DelayTime::create(Animations::Duration::moveDuration + Animations::Duration::scaleDuration), CallFunc::create([=]{onMoveFinished(finishedData);}), NULL);
    actions.pushBack(seq);
    runAction(Spawn::create(actions));
    
    return finishedData.hasBeenMoved;
}

Vector<FiniteTimeAction *> Grid::move(ESwapDirection swapDirection, unsigned int &numberOfPoints)
{
    Vector<FiniteTimeAction *> actions{};
    switch (swapDirection) {
        case eSwapDown:
            actions = move(0,1, true, numberOfPoints);
            break;
        case eSwapRight:
            actions = move(3,-1, false, numberOfPoints);
            break;
        case eSwapLeft:
            actions = move(0,1, false, numberOfPoints);
            break;
        case eSwapUp:
            actions = move(3,-1, true, numberOfPoints);
        default:
            break;
    }
    
    return std::move(actions);
}

void Grid::createCellAtSpecificPosition(unsigned int row, unsigned int col, unsigned short value){
    unsigned int x = getXPosition(col);
    unsigned int y = getYPosition(row);
    m_cardArray[row][col] = NumberCardSprite::create(x,y, Size(static_cast<float>(m_tileWidth), static_cast<float>(m_tileHeight)), value);
    this->addChild(m_cardArray[row][col]);
    ++m_numberFullCells;
}

Vector<FiniteTimeAction *> Grid::move(unsigned int edge, short increase, bool isVerticalMovement, unsigned int &numberOfPoints)
{
    Vector<FiniteTimeAction*> actions {};
    numberOfPoints = 0;
    for (unsigned int i = edge; i < NUMBER_OF_COLUMNS_ROWS; i+= increase) {
        unsigned int initialValue = edge;
        NumberCardSprite* lastCard = nullptr;
        
        for (unsigned int j = edge; j < NUMBER_OF_COLUMNS_ROWS ; j+=increase) {
            auto row = isVerticalMovement ? j : i;
            auto col = isVerticalMovement ? i : j;
            auto& cardArray = m_cardArray[row][col];
            bool toRemove = false;
            
            if(cardArray != nullptr){
                
                if(lastCard && lastCard->getScore() == cardArray->getScore())
                {
                    initialValue-=increase;
                    numberOfPoints += lastCard->getScore() * 2;
                    m_hasArrivedTo2048 |= (lastCard->getScore() * 2 == 2048);
                    toRemove = true;
                }
                else
                {
                    toRemove = false;
                    lastCard = cardArray;
                }
                
                auto destRow = isVerticalMovement ? initialValue : i;
                auto destCol = isVerticalMovement ? i : initialValue;
                
                initialValue += increase;
                
                if(destRow == row && destCol == col) continue;
                
                Vector<FiniteTimeAction*> seq {};
                
                auto action = MoveTo::create(Animations::Duration::moveDuration, {static_cast<float>(getXPosition(destCol)), static_cast<float>(getYPosition(destRow))});
                seq.pushBack(TargetedAction::create(cardArray,action));
                
                if(toRemove) {
                    
                    seq.pushBack(CallFunc::create([&, cardArray]{
                        this->removeChild(cardArray);
                    }));
                    
                    auto scale = ScaleBy::create(Animations::Duration::scaleDuration, 1.25f);
                    seq.pushBack(CallFunc::create([=]{lastCard->updateStateOnNewValue();}));
                    seq.pushBack(TargetedAction::create(lastCard,Sequence::create(scale, scale->reverse(), NULL)));
                    lastCard->setNextValue();
                    
                    m_numberFullCells--;
                    cardArray = nullptr;
                    lastCard = nullptr;
                    
                } else {
                    std::swap(cardArray,m_cardArray[destRow][destCol]);
                }
                
                actions.pushBack(Sequence::create(seq));
            }
        }
    }
    
    return std::move(actions);
}

bool Grid::isMovementPossible()
{
    if(m_numberFullCells == NUMBER_OF_CELLS){
        for (int row = 0; row < NUMBER_OF_COLUMNS_ROWS; ++row)
        {
            unsigned short prevValue = m_cardArray[row][0]->getScore();
            for (int col = 1; col < NUMBER_OF_COLUMNS_ROWS; ++col){
                if(prevValue == m_cardArray[row][col]->getScore())
                {
                    return true;
                }
                prevValue = m_cardArray[row][col]->getScore();
            }
        }
        
        for (int col = 0; col < NUMBER_OF_COLUMNS_ROWS; ++col)
        {
            unsigned short prevValue = m_cardArray[0][col]->getScore();
            for (int row = 1; row < NUMBER_OF_COLUMNS_ROWS; ++row){
                if(prevValue == m_cardArray[row][col]->getScore())
                {
                    return true;
                }
                prevValue = m_cardArray[row][col]->getScore();
            }
        }
        
        return false;
    }
    
    return true;
}

cocos2d::Data Grid::serialize()
{
    std::array<unsigned char, NUMBER_OF_CELLS> serializeStr;
    for(size_t i = 0; i < NUMBER_OF_CELLS; ++i)
    {
        const auto &card = m_cardArray[i/NUMBER_OF_COLUMNS_ROWS][i%NUMBER_OF_COLUMNS_ROWS];
        serializeStr[i] = card ? card->serialize() : 0;
    }
    
    cocos2d::Data data;
    data.copy(serializeStr.data(), serializeStr.size());
    
    return data;
}

void Grid::restore(const cocos2d::Data &data)
{
    if(!data.isNull())
    {
        unsigned char* bytes = data.getBytes();
        assert(data.getSize() == NUMBER_OF_CELLS);
        for(size_t i = 0; i < data.getSize(); ++i)
        {
            const auto &byte = bytes[i];
            if(byte)
            {
                createCellAtSpecificPosition(static_cast<unsigned int>(i/4), i%4, byte);
            }
        }
    }
    
    while (m_numberFullCells < 2) {
        spawnCell();
    }
}
