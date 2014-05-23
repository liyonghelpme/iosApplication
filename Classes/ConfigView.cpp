//
//  ConfigView.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-19.
//
//

#include "ConfigView.h"
#include "Logic.h"

CCScene *ConfigView::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ConfigView *layer = ConfigView::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ConfigView::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    getYet = false;
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCLog("winSize %f %f", size.width, size.height);
    
    UILayer *lay = UILayer::create();
    addChild(lay);
    //lay->setPosition(ccp(size.width/2, size.height/2));
    //lay->setContentSize(size);
    
    
    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_7_0.json");
    lay->addWidget(w);
    w->setSize(size);

    Button *back = static_cast<Button*>(UIHelper::seekWidgetByName(w, "back"));
    back->addTouchEventListener(this, toucheventselector(ConfigView::onBack));
    
    scheduleUpdate();
    return true;
}

void ConfigView::initView() {
    CCLog("init View");
    Button *head = static_cast<Button*>(UIHelper::seekWidgetByName(w, "head"));
    char buf[512];
    sprintf(buf, "flags/%d.png", Logic::getInstance()->getFlagId());
    head->loadTextureNormal(buf);
    
    TextField  *nn = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "nickname"));
    nn->setTouchEnabled(false);
    nn->setText(Logic::getInstance()->getNickName());
    
    TextField *account = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "account"));
    account->setTouchEnabled(false);
    
    account->setText(Logic::getInstance()->getPhoneNumber());
    
    TextField *team = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "team"));
    team->setTouchEnabled(false);
    team->setText(Logic::getInstance()->getBio());
    
    
    TextField *gender = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "gender"));
    gender->setTouchEnabled(false);
    gender->setText(Logic::getInstance()->getGender());
    
    
    TextField *local = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "local"));
    local->setTouchEnabled(false);
    local->setText(Logic::getInstance()->getLocal());
}

void ConfigView::onBack(cocos2d::CCObject *obj, TouchEventType tt){
    switch (tt) {
        case cocos2d::ui::TOUCH_EVENT_BEGAN:
        {
            
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_MOVED:
        {
            
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_ENDED:
        {
            CCDirector::sharedDirector()->popScene();
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_CANCELED:
        {
            
        }
            break;
        default:
            break;
    }
}

void ConfigView::update(float dt){
    if (!getYet) {
        //getYet = true;
        if (Logic::getInstance()->fetchInfoState == 0) {
            Logic::getInstance()->fetchInfo();
        } else if(Logic::getInstance()->fetchInfoState == 2) {
            getYet = true;
            initView();
            
        }
    }
    
}