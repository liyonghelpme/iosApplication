//
//  ChatInfo.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-19.
//
//

#include "ChatInfo.h"
#include "Logic.h"
CCScene *ChatInfo::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ChatInfo *layer = ChatInfo::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ChatInfo::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    getYet = false;
    
    
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCLog("winSize %f %f", size.width, size.height);
    
    UILayer *lay = UILayer::create();
    addChild(lay);

    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_8.json");
    lay->addWidget(w);
    w->setSize(size);
    
    row = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "row"));
    row->setEnabled(false);
    
    lv = static_cast<ListView*>(UIHelper::seekWidgetByName(w, "ListView_5"));
    for (int i=0; i < 5; i++) {
        Layout* rc = (Layout*)row->clone();
        rc->setEnabled(true);
        lv->pushBackCustomItem(rc);
    }
    
    Button *back = static_cast<Button*>(UIHelper::seekWidgetByName(w, "Button_4"));
    back->addTouchEventListener(this, toucheventselector(ChatInfo::onBack));
    
    
    scheduleUpdate();
    return true;
}
void ChatInfo::onBack(CCObject*obj, TouchEventType tt){
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

void ChatInfo::update(float dt){
    if (!getYet) {
        if (!Logi) {
            <#statements#>
        }
    }
}


