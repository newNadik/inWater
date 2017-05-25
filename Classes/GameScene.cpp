#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
    scene->getPhysicsWorld()->setGravity( Vec2( 0, -195.0f ) );

    auto layer = GameScene::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld() );
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    srand( time( NULL ) );
    if ( !Layer::init() ) { return false; }
    auto touchListener = EventListenerKeyboard::create();
    touchListener->onKeyReleased = [](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {  Director::getInstance()->end(); } };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    Device::setAccelerometerEnabled(true);
    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::accelerated, this));
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this);

    Sprite *backgroundSprite = Sprite::create("underwater.png");
    backgroundSprite->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
    backgroundSprite->setScale(MAX(winSize.width / backgroundSprite->getContentSize().width,
                                   winSize.height / backgroundSprite->getContentSize().height));
    this->addChild(backgroundSprite);

    ///corpuse
    corpuseLayer = Layer::create();
    corpuseLayer->setPosition(Vec2::ZERO);
    this->addChild(corpuseLayer, 5);

    Sprite *bottomPart = Sprite::create("bot.png");
    bottomPart->setPosition(Vec2(bottomPart->getContentSize().width*0.5f,
                                 bottomPart->getContentSize().height*0.5f));
    auto controllsBody = PhysicsBody::createEdgeBox( bottomPart->getBoundingBox().size, PHYSICSBODY_MATERIAL_DEFAULT);
    controllsBody->setCategoryBitmask(GAME_BORDER);
    controllsBody->setCollisionBitmask(ITEM);
    bottomPart->setPhysicsBody( controllsBody );
    corpuseLayer->addChild(bottomPart);

    Sprite *topPart = Sprite::create("top.png");
    topPart->setPosition(Vec2(topPart->getContentSize().width*0.5f,
                              winSize.height - topPart->getContentSize().height*0.5f));
    corpuseLayer->addChild(topPart);

    Sprite *midPart = Sprite::create("mid.png");
    midPart->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    midPart->setScaleY((topPart->getBoundingBox().getMinY() -
            bottomPart->getBoundingBox().getMaxY()) / midPart->getContentSize().height);
    midPart->setPosition(Vec2(topPart->getContentSize().width*0.5f,
                              winSize.height - topPart->getContentSize().height));
    corpuseLayer->addChild(midPart);

    Sprite *logo = Sprite::create("logo.png");
    logo->setPosition(Vec2(winSize.width*0.5f, inWaterLogoYpos));
    corpuseLayer->addChild(logo, 1);

    ///Controllers
    controlersLayer = Layer::create();
    controlersLayer->setPosition(Vec2::ZERO);
    this->addChild(controlersLayer, 6);

    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    edgeBody->setCategoryBitmask(BOUNDARY);
    edgeBody->setCollisionBitmask(BUBBLE | ITEM);
    auto edgeNode = Node::create();
    edgeNode->setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2) );
    edgeNode->setPhysicsBody( edgeBody );
    this->addChild( edgeNode );

    ///buttons
    Sprite *leftBtnBack = Sprite::create("bigButtonShadow.png");
    leftBtnBack->setPosition(Vec2(buttonsCenterPosition, buttonsCenterPosition));
    controlersLayer->addChild(leftBtnBack, 1);

    leftButton = Button::create("bigButton.png", "bigButton.png","bigButton.png");
    leftButton->setPosition(leftBtnBack->getPosition());
    leftButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        switch (type){
            case Widget::TouchEventType::BEGAN:{
                ((Button*)pSender)->setTag(1);
                this->scheduleUpdate();
                break;
            }
            case Widget::TouchEventType::CANCELED:{
                this->unscheduleUpdate();
                ((Button*)pSender)->setTag(0);
                ((Button*)pSender)->runAction(ScaleTo::create(0.2f, 1.0f));
                break;}
            case Widget::TouchEventType::ENDED:{
                this->unscheduleUpdate();
                ((Button*)pSender)->setTag(0);
                ((Button*)pSender)->runAction(ScaleTo::create(0.2f, 1.0f));
                break;}
            default: break;

        }
    });
    controlersLayer->addChild(leftButton, 1);

    Sprite *rightBtnBack = Sprite::create("bigButtonShadow.png");
    rightBtnBack->setPosition(Vec2(winSize.width - buttonsCenterPosition, buttonsCenterPosition));
    controlersLayer->addChild(rightBtnBack, 1);

    rightButton = Button::create("bigButton.png", "bigButton.png","bigButton.png");
    rightButton->setPosition(rightBtnBack->getPosition());
    rightButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        switch (type){
            case Widget::TouchEventType::BEGAN:{
                ((Button*)pSender)->setTag(1);
                this->scheduleUpdate();
                break;
            }
            case Widget::TouchEventType::CANCELED:{
                this->unscheduleUpdate();
                ((Button*)pSender)->setTag(0);
                ((Button*)pSender)->runAction(ScaleTo::create(0.2f, 1.0f));
                break;}
            case Widget::TouchEventType::ENDED:{
                this->unscheduleUpdate();
                ((Button*)pSender)->setTag(0);
                ((Button*)pSender)->runAction(ScaleTo::create(0.2f, 1.0f));
                break;}
            default: break;

        }
    });
    controlersLayer->addChild(rightButton, 1);

    vibrButton = Button::create("vibroff_normal.png", "vibroff_pressed.png","vibroff_pressed.png");
    vibrButton->setPosition(vibrButtonPos);
    vibrButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        switch (type){
            case Widget::TouchEventType::ENDED:{
                break;}
            default: break;
        }
    });
    controlersLayer->addChild(vibrButton, 1);

    soundButton = Button::create("soundoff_normal.png", "soundoff_pressed.png","soundoff_pressed.png");
    soundButton->setPosition(soundButtonPos);
    soundButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        switch (type){
            case Widget::TouchEventType::ENDED:{
                break;}
            default: break;
        }
    });
    controlersLayer->addChild(soundButton, 1);

    addStars(5);
    return true;
}
void GameScene::update(float dt) {
    if((int)dt%1 == 0){
        if(rightButton->getTag() > 0 && rightButton->getTag() < 7){
            rightButton->runAction(ScaleBy::create(0.01f, -0.01f));// MoveBy::create(0.01f, Vec2(-1, 4)));
            makeBubbles(rightButton);
            Device::vibrate(0.1f);
            rightButton->setTag(rightButton->getTag()+1);
        }
    }
}

#pragma mark - stars
void GameScene::addStars(int count){
    for(int i=0; i<count; i++){
        Sprite *starSprite = Sprite::create("star.png");
        starSprite->setPosition(Vec2(winSize.width*getRandom(0.25f, 0.75f), winSize.height*getRandom(0.25f, 0.75f)));
        auto spriteBody = PhysicsBody::createBox(Size(starSprite->getContentSize().width,
                                                      starSprite->getContentSize().height),
                                                 PHYSICSBODY_MATERIAL_DEFAULT);//PhysicsMaterial(0.0f, 1, 0));
        spriteBody->setCategoryBitmask(ITEM);
        spriteBody->setCollisionBitmask(BOUNDARY | GAME_BORDER | ITEM | BUBBLE | BUCKET);
        spriteBody->setMass(5); // 200);
        spriteBody->setRotationEnable(true);
        spriteBody->setAngularVelocityLimit(3.0f);
        spriteBody->setDynamic(true);
        starSprite->setPhysicsBody(spriteBody);
        this->addChild(starSprite, 1);
    }
}

void GameScene::makeBubbles(Button *sender) {

    for(int i=0; i < getRandom(1, 4); i++) {
        auto sprite = Sprite::create("bubble.png");
        sprite->setScale(getRandom(0.2f, 0.6f));
        sprite->setPosition(Vec2(getRandom(sender->getBoundingBox().getMinX(),sender->getBoundingBox().getMaxX()),
                                 getRandom(sender->getBoundingBox().getMinY(),sender->getBoundingBox().getMaxY())));
        auto spriteBody = PhysicsBody::createCircle(sprite->getContentSize().width / 2,
                                                    PHYSICSBODY_MATERIAL_DEFAULT);//PhysicsMaterial(0.0f, 1, 0));
        spriteBody->applyImpulse(Vec2((winSize.width - sender->getPositionX()*2.0f)*getRandom(200, 600)//(0.2f, 0.6f)
                                      - sceneWorld->getGravity().x,
                                      (winSize.height - sender->getPositionY()*2.0f)*getRandom(200, 600)//(0.2f, 0.6f)
                                      - sceneWorld->getGravity().y));
        spriteBody->setCategoryBitmask(BUBBLE);
        spriteBody->setCollisionBitmask(BOUNDARY | ITEM | BUBBLE | BUCKET);
        
        spriteBody->setMass(1);//(250);
        spriteBody->setGravityEnable(false);
        spriteBody->setDynamic(true);

        sprite->setPhysicsBody(spriteBody);
        this->addChild(sprite);
        sprite->runAction(Sequence::create(DelayTime::create(getRandom(0.3f, 1.0f)),
                                           FadeOut::create(getRandom(0.2f, 0.5f)),
                                           RemoveSelf::create(true) , NULL));
    }
}
int GameScene::getRandom(int min, int max) { return min + rand() % max; }
float GameScene::getRandom(float min, float max) {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}
void GameScene::accelerated(Acceleration *acceleration, Event *event){
    Vec2 gravity(acceleration->x * 98.0,acceleration->y *98.0);
    sceneWorld->setGravity(gravity);
}
