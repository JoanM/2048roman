//
//  EndGameScene.h
//  roman2048
//
//  Created by Joan Martinez on 03/06/15.
//
//

#ifndef __roman2048__EndGameScene__
#define __roman2048__EndGameScene__

#include <stdio.h>
#include "Grid.h"
#include "Utils.h"

using namespace cocos2d;

enum CloseReason
{
    CloseRestart,
    CloseCancel
};

typedef std::function<void(CloseReason)> CloseCallback;

class EndGame : public cocos2d::LayerColor{
public:
    CREATE_FUNC_ARGS(EndGame);
private:
    bool init(const CloseCallback& callback, unsigned int width, unsigned int height, unsigned int score, unsigned int maxScore, const ECallbackReason &callbackReason);
};

#endif /* defined(__roman2048__EndGameScene__) */
