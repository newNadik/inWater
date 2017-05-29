#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "physics3d/CCPhysics3D.h"
#include "Constants.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class GameScene : public Layer
{
    Layer *controlersLayer;
    Layer *corpuseLayer;
    Physics3DWorld *sceneWorld;

    Button *rightButton;
    Sprite *rightBtnFront;

    Button *leftButton;
    Sprite *leftBtnFront;

    Button *soundButton;
    Button *vibrButton;

    void SetPhysics3DWOrld( cocos2d::Physics3DWorld *world ) { sceneWorld = world; };
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

    Size gameScreenSize;
    int screendepth = 500;
    int gravityInt = 300;
    void initPhysicsBounds();
public:
    static Scene* createScene();

    virtual void update(float dt);
    virtual bool init();
    CREATE_FUNC(GameScene);
};

#endif // _GAME_SCENE_H__
