#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "EndGameScene.h"
class Grid;

class HelloWorld : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    void onMoveFinished(const MoveFinishedData &moveFinishedData);
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
private:
    cocos2d::Vec2 _initPoint {};
    Grid *m_grid{nullptr};
    Node * m_menu{nullptr};
    cocos2d::Label *m_scoreValueLabel{nullptr};
    cocos2d::Label *m_maxScoreValueLabel{nullptr};
    cocos2d::Menu *m_restartMenu{nullptr};
    unsigned int m_numberOfGames{0};
    unsigned int m_numberOfPoints{0};
    unsigned int m_maxPoints{0};
    void showDialog(const ECallbackReason& callbackReason,Ref *);
    void enterBackground(EventCustom *);
    void restartCallback(const CloseReason &reason);
    
    void createGrid();
    void setTitleLabel();
    void createRestartMenu();
    void setScoreLabels();
    void enableTouch();
    void saveState(bool restart);
    void restoreState();
};

#endif // __HELLOWORLD_SCENE_H__
