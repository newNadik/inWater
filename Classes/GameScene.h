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
    PhysicsWorld *sceneWorld;

    Button *rightButton;

    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    void accelerated(Acceleration *acceleration, Event *event);

    void makeBubbles(Button *sender);
    void addStars(int count);
    int getRandom(int min, int max);
    float getRandom(float min, float max);

    Vec2 rightButtonPosition = Vec2(winSize.width - 85 - 255, 50 + 255);
public:
    static Scene* createScene();

    virtual void update(float dt);
    virtual bool init();
    CREATE_FUNC(GameScene);
};

#endif // _GAME_SCENE_H__
