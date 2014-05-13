#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "RedisInterface.h"
#include "cocos-ext.h"
USING_NS_CC;

using namespace ui;
using namespace extension;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    //connect();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    CCMenuItemImage *record = CCMenuItemImage::create("record.png", "record_h.png", this, menu_selector(HelloWorld::record));
    CCMenuItemImage *stop = CCMenuItemImage::create("stop.png", "stop_h.png", this, menu_selector(HelloWorld::stop));
    CCMenuItemImage *play = CCMenuItemImage::create("play.png", "play_h.png", this, menu_selector(HelloWorld::play));
    CCMenuItemLabel *send = CCMenuItemLabel::create(CCLabelTTF::create("send", "", 100), this, menu_selector(HelloWorld::send));
    //start receive thread
    CCMenuItemLabel *receive = CCMenuItemLabel::create(CCLabelTTF::create("receive", "", 100), this, menu_selector(HelloWorld::receive));
                                                       
    
    CCMenu *menu = CCMenu::create(record, stop, play, send, receive, NULL);
    addChild(menu);
    menu->setPosition(ccp(300, 300));
    menu->setScale(0.25);
    menu->setPosition(ccp(0, 0));
    menu->alignItemsHorizontally();
    testR = NULL;
    /*
    TouchGroup *ui = TouchGroup::create();
    UIWidget* addshow = GUIReader::shareReader()->widgetFromJsonFile("RegisterScene.ExportJson");
	ui->addWidget(addshow);
	//addshow->setSize(DGloabData::shareGloabData()->m_winSize);
	ui->setAnchorPoint(CCPointZero);
	this->addChild(ui);
    */
    
    scheduleUpdate();
    return true;
}
void HelloWorld::update(float time) {
    //return;
    if(testR != NULL) {
        std::string channel;
        std::string content;
        bool res = readSubData(testR, &channel, &content);
        if(res){
            CCLog("read data %s %s", channel.c_str(), content.c_str());
        }
    }
}
void HelloWorld::receive(cocos2d::CCObject *pSender){
    testR = createRedis();
    connectRedis(testR);
    runSubscribe(testR);
    
}
void HelloWorld::record(CCObject *pSender) {
    startRecord();
}
void HelloWorld::stop(CCObject *pSender){
    stopRecord();
}
void HelloWorld::play(CCObject *pSender){
    startPlay();
}
void HelloWorld::send(CCObject *pSender){
    const char *fn = getFileName();
    CCLog("file name %s", fn);
    //connect redis server
    connect();
    //send data to server
    startSend(fn);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
