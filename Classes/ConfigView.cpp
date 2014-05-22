//
//  ConfigView.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-19.
//
//

#include "ConfigView.h"

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
    
    return true;
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
    
}