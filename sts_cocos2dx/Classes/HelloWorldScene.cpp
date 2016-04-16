#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto camera = Camera::createPerspective(60, float(visibleSize.width) / visibleSize.height, 1, 1000);
	camera->setPosition3D(Vec3(0.0f, 100.0f, -1.0f));
	camera->lookAt(Vec3(0.0f, 0.0f, 0.0f));
	scene->addChild(camera);

	auto shipSprite = Sprite3D::create("airship.c3b");
	shipSprite->setScale(10.0f);
	shipSprite->setPosition3D(Vec3(0.0f, -200.0f, 100.0f));
	//shipSprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height * 0.6 + origin.y));
	scene->addChild(shipSprite, -1);

	auto light = AmbientLight::create (Color3B::WHITE);
	light->setPosition3D(camera->getPosition3D());
	scene->addChild(light);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	setColor(ccc3(255, 0, 255));
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add your codes below...
    
    return true;
}
