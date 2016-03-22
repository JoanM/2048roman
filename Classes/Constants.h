//
//  Constants.h
//  roman2048
//
//  Created by Joan Martinez on 19/06/15.
//
//

#ifndef roman2048_Constants_h
#define roman2048_Constants_h

enum ESwapDirection
{
    eSwapUp,
    eSwapDown,
    eSwapLeft,
    eSwapRight
};

enum ECallbackReason
{
    eRestart,
    eEndLost,
    eEndWin
};

struct MoveFinishedData
{
    bool hasBeenMoved {false};
    bool hasArrivedTo2048 {false};
    unsigned int numberOfPoints {0};
};

namespace Animations {
    namespace Duration
    {
        static const float scaleDuration = 0.1f;
        static const float moveDuration = 0.2f;
        static const float creationDuration = 0.2f;
    };
};

namespace SaveData{
    static const char maxPoints[] = "maxPoints";
    static const char gamePoints[] = "gamePoints";
    static const char savedState[] = "savedState";
    static const char numOfGames[] = "numOfGames";
}

namespace Notifications {
    static const char enterBackground[] = "enter_background";
}

namespace Resources
{
    namespace Fonts{
        static const char ArialSCBold[] = "AmaticSC-Bold.ttf";
    };
    namespace Images
    {
        static const char RestartNormal[] = "restartNormal.png";
        static const char RestartSelected[] = "restartSelected.png";
    }
}

namespace Labels {
    static const char Highscore[] = "Highscore";
    static const char Score[] = "Score";
    static const char Title[] = "MMXLVIII";
}

namespace Ads{
    static const char FullScreen[] = "gameover";
}

#endif
