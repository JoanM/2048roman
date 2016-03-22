//
//  NumberCardSprite.cpp
//  roman2048
//
//  Created by Joan Martinez on 14/05/15.
//
//

#include "NumberCardSprite.h"
#include "Grid.h"
#include <iostream>


struct CellFormat{
    unsigned short score;
    const char *text;
    Color3B color;
};

static constexpr int DEFAULT_OPACITY = 122;

static const CellFormat romanValues[] = {
    {},
    CellFormat{2, "II", Color3B(150,230, 60)},
    CellFormat{4, "IV", Color3B(100,200, 100)},
    CellFormat{8, "VIII", Color3B(10,200, 200)},
    CellFormat{16, "XVI", Color3B(90,150, 90)},
    CellFormat{32, "XXXII", Color3B(0,140, 140)},
    CellFormat{64, "LXIV", Color3B(40,60, 170)},
    CellFormat{128, "CXXVIII", Color3B(200,60, 150)},
    CellFormat{256, "CCLVI", Color3B(200,10, 200)},
    CellFormat{512, "DXII", Color3B(120,10, 10)},
    CellFormat{1024, "MXXIV", Color3B(190,10, 10)},
    CellFormat{2048, "MMXLVIII", Color3B(255,0,0)}
};

using namespace std;

bool NumberCardSprite::init(float x, float y, const Size &tileSize, unsigned short value){
    
    if(!Node::init())
    {
        return false;
    }
    
    setContentSize({tileSize.width,tileSize.height});
    m_value = value;
    m_score = romanValues[value].score;
    m_innerLabel = Label::createWithTTF(romanValues[value].text, Resources::Fonts::ArialSCBold, 35);
    
    auto contentSize = getContentSize();
    m_innerLabel->setPosition(contentSize.width/2, contentSize.height/2);
    
    m_layer = LayerColor::create(Color4B(romanValues[m_value].color),tileSize.width, tileSize.height);
    m_layer->setOpacity(DEFAULT_OPACITY);
    
    addChild(m_layer);
    addChild(m_innerLabel);
    
    //Positioned like anchor point 0,0 and using the other anchor to other things like scale.
    ignoreAnchorPointForPosition(true);
    setAnchorPoint({0.5,0.5});
    setPosition({x,y});
    
    return true;
};

void NumberCardSprite::setNextValue(){
    ++m_value;
}

unsigned short NumberCardSprite::getScore() const
{
    return m_score;
}

unsigned char NumberCardSprite::serialize() const
{
    return m_value;
}

void NumberCardSprite::updateStateOnNewValue()
{
    m_score = romanValues[m_value].score;
    
    m_innerLabel->setString(romanValues[m_value].text);
    m_layer->setColor(romanValues[m_value].color);
}
