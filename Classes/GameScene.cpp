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

    controlersLayer = Layer::create();
    controlersLayer->setPosition(Vec2::ZERO);
    this->addChild(controlersLayer, 5);

    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    edgeBody->setCategoryBitmask(BOUNDARY);
    edgeBody->setCollisionBitmask(BUBBLE | ITEM);
    auto edgeNode = Node::create();
    edgeNode->setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2) );
    edgeNode->setPhysicsBody( edgeBody );
    this->addChild( edgeNode );

    Sprite *rightBtnForeground = Sprite::create("rightBtnForeground.png");
    rightBtnForeground->setPosition(rightButtonPosition);
    controlersLayer->addChild(rightBtnForeground, 2);

    rightButton = Button::create("rightBtn.png", "rightBtn.png","rightBtn.png");
    rightButton->setPosition(rightButtonPosition);
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
                ((Button*)pSender)->runAction(MoveTo::create(0.2f, rightButtonPosition));
                break;}
            case Widget::TouchEventType::ENDED:{
                this->unscheduleUpdate();
                ((Button*)pSender)->setTag(0);
                ((Button*)pSender)->runAction(MoveTo::create(0.2f, rightButtonPosition));
                break;}
            default: break;

        }
    });
    controlersLayer->addChild(rightButton, 1);

    LayerColor *controllsBack = LayerColor::create(lightBlue, winSize.width, rightBtnForeground->getBoundingBox().getMaxY());
    auto controllsBody = PhysicsBody::createEdgeBox( controllsBack->getBoundingBox().size, PHYSICSBODY_MATERIAL_DEFAULT);
    controllsBody->setCategoryBitmask(GAME_BORDER);
    controllsBody->setCollisionBitmask(ITEM);
    controllsBack->setPhysicsBody( controllsBody );
    controlersLayer->addChild(controllsBack, 0);
    addStars(5);
    return true;
}
void GameScene::update(float dt) {
    if((int)dt%1 == 0){
        if(rightButton->getTag() > 0 && rightButton->getTag() < 7){
            rightButton->runAction(MoveBy::create(0.01f, Vec2(-1, 4)));
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
