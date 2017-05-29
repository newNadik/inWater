#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "3d/CCBundle3D.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
//    auto scene = Scene::createWithPhysics();
////    scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
//    scene->getPhysicsWorld()->setGravity( Vec2( 0, -195.0f ) );
//
//    auto layer = GameScene::create();
//    layer->SetPhysicsWorld( scene->getPhysicsWorld() );
    auto scene = Scene::createWithPhysics();
    scene->getPhysics3DWorld()->setDebugDrawEnable(true);
    scene->getPhysics3DWorld()->setGravity(Vec3(0,0,0));

    auto layer = GameScene::create();
    layer->SetPhysics3DWOrld(scene->getPhysics3DWorld());
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
    backgroundSprite->setGlobalZOrder(0);
    this->addChild(backgroundSprite);


    ///corpuse
    corpuseLayer = Layer::create();
    corpuseLayer->setPosition(Vec2::ZERO);
    corpuseLayer->setGlobalZOrder(5);
    this->addChild(corpuseLayer);

    Sprite *bottomPart = Sprite::create("bot.png");
    bottomPart->setPosition(Vec2(bottomPart->getContentSize().width*0.5f,
                                 bottomPart->getContentSize().height*0.5f));
//    auto controllsBody = PhysicsBody::createEdgeBox( bottomPart->getBoundingBox().size, PHYSICSBODY_MATERIAL_DEFAULT);
//    controllsBody->setCategoryBitmask(GAME_BORDER);
//    controllsBody->setCollisionBitmask(ITEM);
//    bottomPart->setPhysicsBody( controllsBody );
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
    controlersLayer->setGlobalZOrder(6);
    this->addChild(controlersLayer);

//    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
//    edgeBody->setCategoryBitmask(BOUNDARY);
//    edgeBody->setCollisionBitmask(BUBBLE | ITEM);
//    auto edgeNode = Node::create();
//    edgeNode->setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2) );
//    edgeNode->setPhysicsBody( edgeBody );
//    this->addChild( edgeNode );

    ///buttons
    leftBtnFront = Sprite::create("left_big_buton_topshadow.png");
    leftBtnFront->setPosition(Vec2(buttonsCenterPosition, buttonsCenterPosition));
    controlersLayer->addChild(leftBtnFront, 2);

    leftButton = Button::create("left_big_buton_normal.png", "left_big_buton_pressed.png", "left_big_buton_pressed.png");
    leftButton->setPosition(leftBtnFront->getPosition());
    leftButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        Button *senderButton = (Button*)pSender;
        switch (type){
            case Widget::TouchEventType::BEGAN:{
                senderButton->setTag(1);
                this->scheduleUpdate();
                break;
            }
            case Widget::TouchEventType::CANCELED:{
                this->unscheduleUpdate();
                senderButton->setTag(0);
                leftBtnFront->runAction(FadeTo::create(0.2f, 255));
                break;}
            case Widget::TouchEventType::ENDED:{
                this->unscheduleUpdate();
                senderButton->setTag(0);
                leftBtnFront->runAction(FadeTo::create(0.2f, 255));
                break;}
            default: break;

        }
    });
    controlersLayer->addChild(leftButton, 1);

    rightBtnFront = Sprite::create("right_big_buton_topshadow.png");
    rightBtnFront->setPosition(Vec2(winSize.width - buttonsCenterPosition, buttonsCenterPosition));
    controlersLayer->addChild(rightBtnFront, 2);

    rightButton = Button::create("right_big_buton_normal.png", "right_big_buton_pressed.png","right_big_buton_pressed.png");
    rightButton->setPosition(rightBtnFront->getPosition());
    rightButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        Button *senderButton = (Button*)pSender;
        switch (type){
            case Widget::TouchEventType::BEGAN:{
                senderButton->setTag(1);
                this->scheduleUpdate();
                break;
            }
            case Widget::TouchEventType::CANCELED:{
                this->unscheduleUpdate();
                senderButton->setTag(0);
                rightBtnFront->runAction(FadeTo::create(0.2f, 255));
                break;}
            case Widget::TouchEventType::ENDED:{
                this->unscheduleUpdate();
                senderButton->setTag(0);
                rightBtnFront->runAction(FadeTo::create(0.2f, 255));
                break;}
            default: break;

        }
    });
    controlersLayer->addChild(rightButton, 1);

    vibrButton = Button::create("vibron_normal.png", "vibron_pressed.png","vibron_pressed.png");
    if(!UserDefault::getInstance()->getBoolForKey(VIBRATION_ENABLED, true))
        vibrButton->loadTextures("vibroff_normal.png", "vibroff_pressed.png");
    
    vibrButton->setPosition(vibrButtonPos);
    vibrButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        Button *senderButton = (Button*)pSender;
        switch (type){
            case Widget::TouchEventType::ENDED:{
                if(UserDefault::getInstance()->getBoolForKey(VIBRATION_ENABLED, true)){
                    senderButton->loadTextures("vibroff_normal.png", "vibroff_pressed.png");
                    UserDefault::getInstance()->setBoolForKey(VIBRATION_ENABLED, false);
                } else {
                    senderButton->loadTextures("vibron_normal.png", "vibron_pressed.png");
                    UserDefault::getInstance()->setBoolForKey(VIBRATION_ENABLED, true);
                }
                break;}
            default: break;
        }
    });
    controlersLayer->addChild(vibrButton, 1);

    soundButton = Button::create("soundon_normal.png", "soundon_pressed.png","soundon_pressed.png");
    if(!UserDefault::getInstance()->getBoolForKey(SOUND_ENABLED, true))
        soundButton->loadTextures("soundoff_normal.png", "soundoff_pressed.png");
    soundButton->setPosition(soundButtonPos);
    soundButton->addTouchEventListener( [this](Ref* pSender, Widget::TouchEventType type) {
        Button *senderButton = (Button*)pSender;
        switch (type){
            case Widget::TouchEventType::ENDED:{
                if(UserDefault::getInstance()->getBoolForKey(SOUND_ENABLED, true)){
                    senderButton->loadTextures("soundoff_normal.png", "soundoff_pressed.png");
                    UserDefault::getInstance()->setBoolForKey(SOUND_ENABLED, false);
                } else {
                    senderButton->loadTextures("soundon_normal.png", "soundon_pressed.png");
                    UserDefault::getInstance()->setBoolForKey(SOUND_ENABLED, true);
                }
                break;}
            default: break;
        }
    });
    controlersLayer->addChild(soundButton, 1);

    gameScreenSize.width = winSize.width - 100;
    gameScreenSize.height = (topPart->getBoundingBox().getMinY()+190) - 470;

    ///physics3D
    this->initPhysicsBounds();
    addStars(3);
    return true;
}
void GameScene::update(float dt) {
    if((int)dt%1 == 0){
        if(rightButton->getTag() > 0 && rightButton->getTag() < 7){
//            rightBtnFront->runAction(FadeTo::create(0.1f, 255 - (rightButton->getTag()*42)));
            rightButton->setTag(rightButton->getTag()+1);
            int fadeTo = 255 - (rightButton->getTag()*36);
            rightBtnFront->runAction(FadeTo::create(0.1, fadeTo));
            makeBubbles(rightButton);
            if(UserDefault::getInstance()->getBoolForKey(VIBRATION_ENABLED, true))
                Device::vibrate(0.1f);
        }
        if(leftButton->getTag() > 0 && leftButton->getTag() < 7){
            leftButton->setTag(leftButton->getTag()+1);
            int fadeTo = 255 - (leftButton->getTag()*36);
            leftBtnFront->runAction(FadeTo::create(0.1, fadeTo));
            makeBubbles(leftButton);
            if(UserDefault::getInstance()->getBoolForKey(VIBRATION_ENABLED, true))
                Device::vibrate(0.1f);
        }
    }
}

#pragma mark - stars
void GameScene::addStars(int count){
    for(int i=0; i<count; i++){

        Physics3DRigidBodyDes roboxDesxs;
        roboxDesxs.mass = 10.0f;
        roboxDesxs.disableSleep = true;
        float itemScale = 100.0f;
        auto robox = Sprite3D::create("test.obj");
        robox->setScale(itemScale);
        roboxDesxs.shape = Physics3DShape::createBox(Vec3(robox->getContentSize().width * itemScale,
                                                       robox->getContentSize().height * itemScale,
                                                       robox->getContentSize().width * itemScale));
        auto boundsBackBody = Physics3DRigidBody::create(&roboxDesxs);
        auto boundsBackComponent = Physics3DComponent::create(boundsBackBody);
        robox->setPosition3D(Vec3(getRandom(winSize.width*0.3f,winSize.width*0.7f),
                                  getRandom(winSize.height*0.65f,winSize.height*0.8f),
                                  -screendepth));
        robox->addComponent(boundsBackComponent);
        boundsBackComponent->syncNodeToPhysics();
        boundsBackComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
        robox->setGlobalZOrder(3);
        this->addChild(robox);


//        auto robox = Sprite3D::create("test.obj");
//        float itemScale = 100.0f;
//        robox->setScale(itemScale);
//
//        Physics3DRigidBodyDes rbDes;
//        std::vector<Vec3> trianglesList = Bundle3D::getTrianglesList("test.obj");
//        for (auto& it : trianglesList) { it *= itemScale; }
//
//        rbDes.mass = 100.0f;
//        rbDes.shape = Physics3DShape::createMesh(&trianglesList[0], (int)trianglesList.size() / 3);
//        auto rigidBody = Physics3DRigidBody::create(&rbDes);
//        rigidBody->setRestitution(1.0f);
//
//        auto rbcs = Physics3DComponent::create(rigidBody);
//        robox->setPosition3D(Vec3(getRandom(winSize.width*0.3f,winSize.width*0.7f),
//                                  getRandom(winSize.height*0.65f,winSize.height*0.8f),
//                                  -screendepth));
//        robox->addComponent(rbcs);
//        rbcs->syncNodeToPhysics();
//        rbcs->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
//        robox->setGlobalZOrder(3);
//        this->addChild(robox);

        ///2D
//        Sprite *starSprite = Sprite::create("star.png");
//        starSprite->setPosition(Vec2(winSize.width*getRandom(0.25f, 0.75f), winSize.height*getRandom(0.25f, 0.75f)));
//        auto spriteBody = PhysicsBody::createBox(Size(starSprite->getContentSize().width,
//                                                      starSprite->getContentSize().height),
//                                                 PHYSICSBODY_MATERIAL_DEFAULT);//PhysicsMaterial(0.0f, 1, 0));
//        spriteBody->setCategoryBitmask(ITEM);
//        spriteBody->setCollisionBitmask(BOUNDARY | GAME_BORDER | ITEM | BUBBLE | BUCKET);
//        spriteBody->setMass(5); // 200);
//        spriteBody->setRotationEnable(true);
//        spriteBody->setAngularVelocityLimit(3.0f);
//        spriteBody->setDynamic(true);
//        starSprite->setPhysicsBody(spriteBody);
//        this->addChild(starSprite, 1);
    }
}

void GameScene::makeBubbles(Button *sender) {

    for(int i=0; i < getRandom(1, 4); i++) {

        Physics3DRigidBodyDes rbDesxs;
        rbDesxs.mass = 100.0f;
        auto robox = Sprite3D::create();
        rbDesxs.shape = Physics3DShape::createSphere(getRandom(10, 20));
        auto rbds = Physics3DRigidBody::create(&rbDesxs);
        rbds->applyCentralImpulse(Vec3((winSize.width - sender->getPositionX()*2.0f)*getRandom(30, 70)
                                      - sceneWorld->getGravity().x,
                                      (winSize.height - sender->getPositionY()*2.0f)*getRandom(30, 70)
                                      - sceneWorld->getGravity().y, getRandom(-screendepth*0.4f, screendepth*0.4f)));
        auto rbcs = Physics3DComponent::create(rbds);
        float widthPos;
        if(sender->getPositionX() < winSize.width*0.5f){
            widthPos = getRandom(50.0f,winSize.width*0.5f);
        } else {
            widthPos = getRandom(winSize.width*0.5f, winSize.width - 50);
        }
        robox->setPosition3D(Vec3(widthPos, //getRandom(sender->getBoundingBox().getMinX(),sender->getBoundingBox().getMaxX()),
                                 getRandom(480,550),
                                  getRandom(-screendepth*0.4f, screendepth*0.4f)));
        robox->addComponent(rbcs);
        rbcs->syncNodeToPhysics();
        rbcs->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
        this->addChild(robox);
        robox->setGlobalZOrder(3);

        robox->runAction(Sequence::create(DelayTime::create(getRandom(0.5f, 1.2f)),
                                           FadeOut::create(getRandom(0.3f, 0.7f)),
                                           RemoveSelf::create(true) , NULL));

//        auto sprite = Sprite::create("bubble.png");
//        sprite->setScale(getRandom(0.2f, 0.6f));
//        sprite->setPosition(Vec2(getRandom(sender->getBoundingBox().getMinX(),sender->getBoundingBox().getMaxX()),
//                                 getRandom(sender->getBoundingBox().getMinY(),sender->getBoundingBox().getMaxY())));
//        auto spriteBody = PhysicsBody::createCircle(sprite->getContentSize().width / 2,
//                                                    PHYSICSBODY_MATERIAL_DEFAULT);//PhysicsMaterial(0.0f, 1, 0));
//        spriteBody->applyImpulse(Vec2((winSize.width - sender->getPositionX()*2.0f)*getRandom(200, 600)//(0.2f, 0.6f)
//                                      - sceneWorld->getGravity().x,
//                                      (winSize.height - sender->getPositionY()*2.0f)*getRandom(200, 600)//(0.2f, 0.6f)
//                                      - sceneWorld->getGravity().y));
//        spriteBody->setCategoryBitmask(BUBBLE);
//        spriteBody->setCollisionBitmask(BOUNDARY | ITEM | BUBBLE | BUCKET);
//
//        spriteBody->setMass(1);//(250);
//        spriteBody->setGravityEnable(false);
//        spriteBody->setDynamic(true);
//
//        sprite->setPhysicsBody(spriteBody);
//        sprite->setGlobalZOrder(4);
//        this->addChild(sprite);
//        sprite->runAction(Sequence::create(DelayTime::create(getRandom(0.3f, 1.0f)),
//                                           FadeOut::create(getRandom(0.2f, 0.5f)),
//                                           RemoveSelf::create(true) , NULL));
    }
}
int GameScene::getRandom(int min, int max) { return min + rand() % max; }
float GameScene::getRandom(float min, float max) {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}
void GameScene::accelerated(Acceleration *acceleration, Event *event){
//    Vec2 gravity(acceleration->x * 98.0,acceleration->y *98.0);
//    sceneWorld->setGravity(gravity);
    Vec3 gravity(acceleration->x * gravityInt,acceleration->y *gravityInt, acceleration->z *gravityInt);
    sceneWorld->setGravity(gravity);
}
void GameScene::initPhysicsBounds(){
    Physics3DRigidBodyDes boundsBackDesxs;
    boundsBackDesxs.mass = 0.0f;
    boundsBackDesxs.shape = Physics3DShape::createBox(Vec3(gameScreenSize.width, gameScreenSize.height, 1));
    auto boundsBackBody = Physics3DRigidBody::create(&boundsBackDesxs);
    auto boundsBackComponent = Physics3DComponent::create(boundsBackBody);
    auto backScreen = Sprite3D::create();
    backScreen->setPosition3D(Vec3(winSize.width*0.5f, 470.0f+gameScreenSize.height*0.5f, -screendepth));
    backScreen->addComponent(boundsBackComponent);
    boundsBackComponent->syncNodeToPhysics();
    boundsBackComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
    backScreen->setGlobalZOrder(2);
    this->addChild(backScreen);

    Physics3DRigidBodyDes boundsRightDesxs;
    boundsRightDesxs.mass = 0.0f;
    boundsRightDesxs.shape = Physics3DShape::createBox(Vec3(1, gameScreenSize.height, screendepth));
    auto boundsRightBody = Physics3DRigidBody::create(&boundsRightDesxs);
    auto boundsRightComponent = Physics3DComponent::create(boundsRightBody);
    auto frontRightScreen = Sprite3D::create();
    frontRightScreen->setPosition3D(Vec3(winSize.width - 50.0f, 470.0f+gameScreenSize.height*0.5f, -screendepth*0.5f));
    frontRightScreen->addComponent(boundsRightComponent);
    boundsRightComponent->syncNodeToPhysics();
    boundsRightComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
    frontRightScreen->setGlobalZOrder(3);
    this->addChild(frontRightScreen);

    Physics3DRigidBodyDes boundsLeftDesxs;
    boundsLeftDesxs.mass = 0.0f;
    boundsLeftDesxs.shape = Physics3DShape::createBox(Vec3(1, gameScreenSize.height, screendepth));
    auto boundsLeftBody = Physics3DRigidBody::create(&boundsLeftDesxs);
    auto boundsLeftComponent = Physics3DComponent::create(boundsLeftBody);
    auto frontLeftScreen = Sprite3D::create();
    frontLeftScreen->setPosition3D(Vec3(50.0f, 470.0f+gameScreenSize.height*0.5f, -screendepth*0.5f));
    frontLeftScreen->addComponent(boundsLeftComponent);
    boundsLeftComponent->syncNodeToPhysics();
    boundsLeftComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
    frontLeftScreen->setGlobalZOrder(3);
    this->addChild(frontLeftScreen);

    Physics3DRigidBodyDes boundsBotDesxs;
    boundsBotDesxs.mass = 0.0f;
    boundsBotDesxs.shape = Physics3DShape::createBox(Vec3(gameScreenSize.width, 1, screendepth));
    auto boundsBotBody = Physics3DRigidBody::create(&boundsBotDesxs);
    auto boundsBotComponent = Physics3DComponent::create(boundsBotBody);
    auto frontBotScreen = Sprite3D::create();
    frontBotScreen->setPosition3D(Vec3(winSize.width*0.5f, 470.0f, -screendepth*0.5f));
    frontBotScreen->addComponent(boundsBotComponent);
    boundsBotComponent->syncNodeToPhysics();
    boundsBotComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
    frontBotScreen->setGlobalZOrder(3);
    this->addChild(frontBotScreen);

    Physics3DRigidBodyDes boundsTopDesxs;
    boundsTopDesxs.mass = 0.0f;
    boundsTopDesxs.shape = Physics3DShape::createBox(Vec3(gameScreenSize.width, 1, screendepth));
    auto boundsTopBody = Physics3DRigidBody::create(&boundsTopDesxs);
    auto boundsTopComponent = Physics3DComponent::create(boundsTopBody);
    auto frontTopScreen = Sprite3D::create();
    frontTopScreen->setPosition3D(Vec3(winSize.width*0.5f, 470.0f+gameScreenSize.height, -screendepth*0.5f));
    frontTopScreen->addComponent(boundsTopComponent);
    boundsTopComponent->syncNodeToPhysics();
    boundsTopComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
    frontTopScreen->setGlobalZOrder(3);
    this->addChild(frontTopScreen);

    Physics3DRigidBodyDes boundsDesxs;
    boundsDesxs.mass = 0.0f;
    boundsDesxs.shape = Physics3DShape::createBox(Vec3(gameScreenSize.width, gameScreenSize.height, 1));
    auto boundsBody = Physics3DRigidBody::create(&boundsDesxs);
    auto boundsComponent = Physics3DComponent::create(boundsBody);
    auto frontScreen = Sprite3D::create();
    frontScreen->setPosition3D(Vec3(winSize.width*0.5f, 470.0f+gameScreenSize.height*0.5f, 0));
    frontScreen->addComponent(boundsComponent);
    boundsComponent->syncNodeToPhysics();
    boundsComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
    frontScreen->setGlobalZOrder(4);
    this->addChild(frontScreen);
}
