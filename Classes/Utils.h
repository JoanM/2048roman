//
//  Utils.h
//  roman2048
//
//  Created by Joan Martinez on 20/05/15.
//
//

#ifndef roman2048_Utils_h
#define roman2048_Utils_h

#include <iostream>
#include <sstream>
#include "cocos2d.h"
#include "Constants.h"

namespace Utils{
    
    namespace Str
    {
        // Use of std::to_string gives an error in android
        template < typename T > std::string to_string( const T& n )
        {
            std::ostringstream stm ;
            stm << n ;
            return stm.str() ;
        }
    }
    
    namespace Position
    {
        static ESwapDirection getSwapDirection(const cocos2d::Point& initPoint, const cocos2d::Point& endPoint)
        {
            auto xDelta = endPoint.x - initPoint.x;
            auto yDelta = endPoint.y - initPoint.y;
            
            ESwapDirection swapDirection;
            if(std::abs(xDelta) > std::abs(yDelta))
            {
                swapDirection = xDelta > 0 ? eSwapRight : eSwapLeft;
            }
            else
            {
                swapDirection = yDelta > 0 ? eSwapUp: eSwapDown;
            }
            
            return swapDirection;
        }
        
        static cocos2d::Vec2 getBottomLeftCorner()
        {
            return cocos2d::Director::getInstance()->getVisibleOrigin();
        }
        
        static cocos2d::Vec2 getBottomRightCorner()
        {
            return { cocos2d::Director::getInstance()->getVisibleOrigin().x + cocos2d::Director::getInstance()->getVisibleSize().width,
                cocos2d::Director::getInstance()->getVisibleOrigin().y};
        }
        
        static cocos2d::Vec2 getTopLeftCorner()
        {
            return { cocos2d::Director::getInstance()->getVisibleOrigin().x,
                cocos2d::Director::getInstance()->getVisibleOrigin().y + cocos2d::Director::getInstance()->getVisibleSize().height};
        }
        
        static cocos2d::Vec2 getTopRightCorner()
        {
            return { cocos2d::Director::getInstance()->getVisibleOrigin().x + cocos2d::Director::getInstance()->getVisibleSize().width,
                cocos2d::Director::getInstance()->getVisibleOrigin().y + cocos2d::Director::getInstance()->getVisibleSize().height};
        }
        
        static cocos2d::Vec2 getCenter()
        {
            return { cocos2d::Director::getInstance()->getVisibleOrigin().x + cocos2d::Director::getInstance()->getVisibleSize().width /2,
                cocos2d::Director::getInstance()->getVisibleOrigin().y + cocos2d::Director::getInstance()->getVisibleSize().height/2};
        }
        
        static cocos2d::Vec2 getCenter(const cocos2d::Node & parent)
        {
            return {parent.getContentSize().width/2, parent.getContentSize().height/2};
        }
    }
    
    namespace Data
    {
        static int getInt(const char *key)
        {
            return cocos2d::UserDefault::getInstance()->getIntegerForKey(key);
        }
        
        static void storeInt(const char *key, int value)
        {
            return cocos2d::UserDefault::getInstance()->setIntegerForKey(key, value);
        }
        
        static void storeBytes(const char *key, const cocos2d::Data &data)
        {
            return cocos2d::UserDefault::getInstance()->setDataForKey(key, data);
        }
        
        static cocos2d::Data getBytes(const char * key)
        {
            return cocos2d::UserDefault::getInstance()->getDataForKey(key);
        }
    }
    
    namespace NodeCreation{
        template<class Func>
        static auto createType(Func f, const cocos2d::Vec2& position) -> decltype(f())
        {
            auto node = f();
            
            node->setPosition(position);
            return node;
        }
        
        template<class Func>
        static auto createType(Func f, const cocos2d::Vec2& position, const cocos2d::Point& anchor, bool ignoreAnchorPoint = false) -> decltype(f())
        {
            auto node = f();
            node->setPosition(position);
            node->setAnchorPoint(anchor);
            return node;
        }
    }
}

#define CREATE_FUNC_ARGS(__TYPE__) \
template <typename ...Args> \
static __TYPE__* create(Args&&... args) \
{ \
__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(std::forward<Args>(args)...)) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

#endif
