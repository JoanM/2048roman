//
//  EndGameScene.cpp
//  roman2048
//
//  Created by Joan Martinez on 03/06/15.
//
//

#include "Utils.h"
#include "HelloWorldScene.h"
#include "EndGameScene.h"
#include "Grid.h"
#include "HelloWorldScene.h"
using namespace cocos2d;

static constexpr char OK_TEXT[] = "Ok";
static constexpr char CANCEL_TEXT[] = "Cancel";
static constexpr char RESTART_TEXT[] = "Restart";
static constexpr char LOST_TEXT[] = "You Lost :(";
static constexpr char WON_TEXT[] = "You Won!! :)";
static constexpr char PLAYAGAIN_TEXT[] = "Play again";

void createScoreLabel(const char * label, unsigned int value, Node & parent, int verticalPadding, Point initPosition, Label **valueLabel, Label **textLabel, const Color3B&color);

bool EndGame::init(const CloseCallback& callback, unsigned int width, unsigned int height, unsigned int score, unsigned int maxScore, const ECallbackReason &callbackReason)
{
    
    if ( !LayerColor::initWithColor({166,166,166,230}, width, height) )
    {
        return false;
    }
    
    auto firstLineY = this->getContentSize().height * 3/ 4;
    auto secondLineY = this->getContentSize().height / 4;
    
    createScoreLabel(Labels::Highscore, maxScore, *this, 50, {getContentSize().width - 100,  secondLineY}, nullptr, nullptr, Color3B::WHITE);
    createScoreLabel(Labels::Score, score, *this, 50, {100,  secondLineY}, nullptr, nullptr, Color3B::WHITE);
    
    const char *messageLabel {nullptr};
    
    switch (callbackReason) {
        case eRestart:
            messageLabel = RESTART_TEXT;
            break;
        case eEndLost:
            messageLabel = LOST_TEXT;
            break;
        case eEndWin:
            messageLabel = WON_TEXT;
            break;
    }
    
    addChild(Utils::NodeCreation::createType([=]{
        return Label::createWithTTF(messageLabel, Resources::Fonts::ArialSCBold, 75);
    }, {Utils::Position::getCenter(*this).x, firstLineY}));
    Menu *menu {nullptr};
    
    
    switch(callbackReason)
    {
        case eRestart:
        {
            auto menuItem = MenuItemFont::create(OK_TEXT, [=](Ref*)
                                                 {
                                                     callback(CloseRestart);
                                                 });
            
            auto menuItemCancel = MenuItemFont::create(CANCEL_TEXT, [=](Ref*)
                                                       {
                                                           if(this->getParent())
                                                           {
                                                               this->getParent()->removeChild(this);
                                                           }
                                                           callback(CloseCancel);
                                                       });
            
            menu = Menu::create(menuItem, menuItemCancel, NULL);
            break;
        }
        case eEndLost:
        {
            auto menuItem = MenuItemFont::create(RESTART_TEXT, [=](Ref*)
                                                 {
                                                     callback(CloseRestart);
                                                 });
            menu = Menu::createWithItem(menuItem);
            break;
        }
        case eEndWin:
        {
            auto menuItem = MenuItemFont::create(PLAYAGAIN_TEXT, [=](Ref*)
                                                 {
                                                     callback(CloseRestart);
                                                 });
            menu = Menu::createWithItem(menuItem);
            break;
        }
    }
    menu->alignItemsHorizontallyWithPadding(100);
    menu->setAnchorPoint({0.5, 0.5});
    menu->setPosition({Utils::Position::getCenter(*this)});
    addChild(menu);
    
    return true;
}



