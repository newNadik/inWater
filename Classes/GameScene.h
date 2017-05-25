#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Constants.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class GameScene : public Layer
{
    Layer *controlersLayer;
    Layer *corpuseLayer;
    PhysicsWorld *sceneWorld;

    Button *rightButton;
    Button *leftButton;

    Button *soundButton;
    Button *vibrButton;

    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    void accelerated(Acceleration *acceleration, Event *event);

    void makeBubbles(Button *sender);
    void addStars(int count);
    int getRandom(int min, int max);
    float getRandom(float min, float max);
/// ui
    int buttonsCenterPosition = 287;
    int inWaterLogoYpos = 185;
    Vec2 vibrButtonPos = Vec2(901, 373);
    Vec2 soundButtonPos = Vec2(635, 373);

public:
    static Scene* createScene();

    virtual void update(float dt);
    virtual bool init();
    CREATE_FUNC(GameScene);
};

#endif // _GAME_SCENE_H__
