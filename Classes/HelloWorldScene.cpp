#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PluginAdMob/PluginAdMob.h"
#include "Grid.h"
#include "Utils.h"
#include "EndGameScene.h"
#include <iostream>
#include <array>

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace Utils::Position;


void HelloWorld::saveState(bool restart)
{
    if (restart) {
        Utils::Data::storeInt(SaveData::gamePoints, 0);
        Utils::Data::storeBytes(SaveData::savedState, cocos2d::Data::Null);
    }
    else
    {
        Utils::Data::storeInt(SaveData::gamePoints, m_numberOfPoints);
        Utils::Data::storeBytes(SaveData::savedState, m_grid->serialize());
    }
    
    Utils::Data::storeInt(SaveData::maxPoints, m_maxPoints);
    Utils::Data::storeInt(SaveData::numOfGames, m_numberOfGames);
}

void HelloWorld::restoreState()
{
    m_maxPoints = Utils::Data::getInt(SaveData::maxPoints);
    m_numberOfPoints = Utils::Data::getInt(SaveData::gamePoints);
    m_numberOfGames = Utils::Data::getInt(SaveData::numOfGames);
    m_grid->restore(Utils::Data::getBytes(SaveData::savedState));
}

void createScoreLabel(const char * label, unsigned int value, Node & parent, int verticalPadding, Point initPosition, Label **valueLabel, Label **textLabel, const Color3B & color){
    auto textL = Label::createWithTTF(label, Resources::Fonts::ArialSCBold, 35);
    auto valueL = Label::createWithTTF(Utils::Str::to_string(value), Resources::Fonts::ArialSCBold, 35);
    textL->setColor(color);
    valueL->setColor(color);
    
    textL->setPosition(initPosition);
    valueL->setPosition({initPosition.x, initPosition.y - verticalPadding});
    parent.addChild(textL);
    parent.addChild(valueL);
    
    if(valueLabel)
    {
        *valueLabel = valueL;
    }
    
    if(textLabel)
    {
        *textLabel = textL;
    }
}

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    
    scene->addChild(layer);
    
    return scene;
}

void HelloWorld::createGrid()
{
    m_grid = Utils::NodeCreation::createType(
                                             [](){return Grid::create();},
                                             {Utils::Position::getCenter().x, Utils::Position::getCenter().y - 30},
                                             {0.5,0.5}
                                             );
    this->addChild(m_grid);
}

void HelloWorld::setTitleLabel()
{
    auto valueL = Label::createWithTTF(Labels::Title, Resources::Fonts::ArialSCBold, 85);
    valueL->setColor(Color3B(0,140, 140));
    valueL->setPosition(Utils::Position::getCenter().x, Utils::Position::getTopLeftCorner().y - 81);
    this->addChild(valueL);
}

void HelloWorld::createRestartMenu()
{
    auto menuItem = MenuItemImage::create(Resources::Images::RestartNormal,
                                          Resources::Images::RestartSelected,
                                          std::bind(&HelloWorld::showDialog, this, eRestart, std::placeholders::_1));
    
    // Assuming m_grid has anchor point 0.5 , 0.5
    m_menu = Utils::NodeCreation::createType([&](){return Menu::createWithItem(menuItem);}, {
        Utils::Position::getCenter().x,
        (m_grid->getPosition().y - m_grid->getContentSize().height/2)/2,
    }, {0.5, 0.5});
    
    addChild(m_menu);
}

void HelloWorld::setScoreLabels()
{
    Label *textLabel = NULL;
    float leftGridPosition = m_grid->getPosition().x - m_grid->getContentSize().width / 2.0;
    float rightGridPosition = m_grid->getPosition().x + m_grid->getContentSize().width / 2.0;
    
    createScoreLabel( Labels::Highscore,
                     m_maxPoints,
                     *this,
                     40,
                     {rightGridPosition, Utils::Position::getTopLeftCorner().y - 170},
                     &m_maxScoreValueLabel,
                     &textLabel,
                     Color3B(50,155,50) );
    
    textLabel->setAnchorPoint({1.0, 0.0});
    m_maxScoreValueLabel->setAnchorPoint({1.0, 0.0});
    
    Label *scoreTextLabel = NULL;
    createScoreLabel( Labels::Score,
                     m_numberOfPoints,
                     *this,
                     40,
                     {leftGridPosition, Utils::Position::getTopLeftCorner().y  - 170},
                     &m_scoreValueLabel,
                     &scoreTextLabel,
                     Color3B(50,155,50));
    
    scoreTextLabel->setAnchorPoint({0,0});
    m_scoreValueLabel->setAnchorPoint({0,0});
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    this->_initPoint = touch->getLocation();
    return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
    ESwapDirection swapDirection = getSwapDirection(_initPoint, touch->getLocation());
    
    this->pause();
    m_grid->move(swapDirection, std::bind(&HelloWorld::onMoveFinished, this, std::placeholders::_1));
}

void HelloWorld::enableTouch()
{
    auto* touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchEvent->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(cocos2d::Color4B(240,237,233,255)) )
    {
        return false;
    }
    
    createGrid();
    restoreState();
    
    setTitleLabel();
    createRestartMenu();
    setScoreLabels();
    enableTouch();
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(Notifications::enterBackground,
                                                                          std::bind(&HelloWorld::enterBackground, this, std::placeholders::_1));
        
    return true;
}

void HelloWorld::enterBackground(EventCustom *)
{
    saveState(false);
}

void HelloWorld::showDialog(const ECallbackReason& callbackReason, Ref *)
{
    this->pause();
    m_menu->pause();
    const CloseCallback callback = std::bind(&HelloWorld::restartCallback, this, std::placeholders::_1);
    auto endGame = EndGame::create(callback, Director::getInstance()->getVisibleSize().width - 50,Director::getInstance()->getVisibleSize().height - 50, m_numberOfPoints, m_maxPoints, callbackReason);
    endGame->setPosition(Utils::Position::getCenter().x, Utils::Position::getBottomLeftCorner().y);
    addChild(endGame);
    endGame->setAnchorPoint({0.5, 0.5});
    endGame->ignoreAnchorPointForPosition(false);
    endGame->setPosition(Utils::Position::getCenter().x, Utils::Position::getBottomLeftCorner().y);
    auto move = MoveTo::create(1, Vec2(Utils::Position::getCenter()));
    
    auto move_ease_in = EaseBounceOut::create(move->clone());
    endGame->runAction(move_ease_in);
}

void HelloWorld::restartCallback(const CloseReason &reason)
{
    switch (reason) {
        case CloseCancel:
            m_menu->resume();
            this->resume();
            break;
        case CloseRestart:
            ++m_numberOfGames;
            if(m_numberOfGames % 5 == 0)
            {
#ifdef SDKBOX_ENABLED
                sdkbox::PluginAdMob::show(Ads::FullScreen);
                sdkbox::PluginAdMob::cache(Ads::FullScreen);
#endif
            }
            
            saveState(true);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(Notifications::enterBackground);
            Director::getInstance()->replaceScene(HelloWorld::createScene());
            break;
    }
}

void HelloWorld::onMoveFinished(const MoveFinishedData &moveFinishedData)
{
    this->resume();
    if(moveFinishedData.hasBeenMoved)
    {
        m_numberOfPoints += moveFinishedData.numberOfPoints;
        
        m_scoreValueLabel->setString(Utils::Str::to_string(m_numberOfPoints));
        
        if(m_numberOfPoints > m_maxPoints){
            m_maxPoints = m_numberOfPoints;
            m_maxScoreValueLabel->setString(Utils::Str::to_string(m_maxPoints));
        }
        
        if(moveFinishedData.hasArrivedTo2048)
        {
            showDialog(eEndWin, nullptr);
        }
        else
        {
            m_grid->spawnCell();
            if(!m_grid->isMovementPossible())
            {
                showDialog(eEndLost, nullptr);
            }
        }
    }
}
