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
    
    /*
    for (int i=0; i < 5; i++) {
        Layout* rc = (Layout*)row->clone();
        rc->setEnabled(true);
        lv->pushBackCustomItem(rc);
    }
    */
    
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

//登录房间的时候 应该附带有用户的基本讯息
void ChatInfo::update(float dt){
    //获取房间信息
    if (!getYet) {
        if (!Logic::getInstance()->chatInfoYet) {
            Logic::getInstance()->initChatInfo();
        } else if (Logic::getInstance()->initChatYet){
            getYet = true;
            rapidjson::Document &d = Logic::getInstance()->chatD;
            rapidjson::Value &b = d["channel"];
            Layout *rc;
            Button *b1, *b2, *b3;
            for (int i =0; i < b.Size(); i++) {
                if (i%3 == 0) {
                    rc = (Layout*)row->clone();
                    rc->setEnabled(true);
                    lv->pushBackCustomItem(rc);
                    b1 = static_cast<Button*>(UIHelper::seekWidgetByName(rc, "Button_7"));
                    b2 = static_cast<Button*>(UIHelper::seekWidgetByName(rc, "Button_9"));
                    b3 = static_cast<Button*>(UIHelper::seekWidgetByName(rc, "Button_10"));
                    b2->setEnabled(false);
                    b3->setEnabled(false);
                } else if(i % 3 == 1) {
                    b2->setEnabled(true);
                }  else {
                    b3->setEnabled(true);
                }
            }
            
        }
    }
}


