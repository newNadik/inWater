#include "cocos2d.h"
using namespace cocos2d;

#define winSize Director::getInstance()->getWinSize()
#define visibleSize Director::getInstance()->getVisibleSize()
//blue - 0, 174, 240
#define lightBlue Color4B(0, 174, 240, 255)

enum CategoryBitMask {
    BOUNDARY =          0x0001,
    GAME_BORDER =       0x0002,
    BUBBLE =            0x0004,
    ITEM =              0x0008,
    BUCKET =            0x0010,
};
