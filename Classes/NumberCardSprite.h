//
//  NumberCardSprite.h
//  roman2048
//
//  Created by Joan Martinez on 14/05/15.
//
//

#ifndef __roman2048__NumberCardSprite__
#define __roman2048__NumberCardSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "Utils.h"
#include <utility>


using namespace cocos2d;

class NumberCardSprite : public cocos2d::Node{
public:
    NumberCardSprite() = default;
    unsigned short getScore() const;
    unsigned char serialize() const;
    
    void updateStateOnNewValue();
    void setNextValue();
    
    CREATE_FUNC_ARGS(NumberCardSprite);
    
private:
       
    unsigned short m_value{1};
    unsigned short m_score{0};
    Label *m_innerLabel {};
    LayerColor *m_layer{};
    
    bool init(float x, float y, const Size& tileSize, unsigned short value);
    
};


#endif /* defined(__roman2048__NumberCardSprite__) */
