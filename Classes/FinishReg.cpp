//
//  FinishReg.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-19.
//
//

#include "FinishReg.h"
#include "HttpModel.h"
#include "Logic.h"
#include "WorldCup.h"


CCScene *FinishReg::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FinishReg *layer = FinishReg::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool FinishReg::init() {
    if ( !CCLayer::init() )
    {
        return false;
    }
    inReg = false;
    CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    UILayer *lay = UILayer::create();
    addChild(lay);

    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_6.json");
    lay->addWidget(w);
    w->setSize(size);
    
    finBut = static_cast<Button*>(UIHelper::seekWidgetByName(w, "finishRegister"));
    finBut->addTouchEventListener(this, toucheventselector(FinishReg::onFin));
    
    name = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "name"));
    error = static_cast<Label*>(UIHelper::seekWidgetByName(w, "error"));
    error->setEnabled(false);
    
    
    netLay = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "Panel_8"));
    netLay->setEnabled(false);
    
    return true;
}

void FinishReg::onFin(cocos2d::CCObject *obj, TouchEventType tt){
    
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
            if (inReg) {
                return;
            }
            string n = name->getStringValue();
            if (n.length() == 0) {
                error->setEnabled(true);
                error->setText("昵称不合法");
                error->runAction(CCSequence::create(CCScaleTo::create(0.1, 1.2, 1.2), CCScaleTo::create(0.1, 1), NULL));
                return;
            }
            
            
            inReg = true;
            HttpModel *hm = HttpModel::getInstance();
            std::map<string, string>pd;
            char uid[128];
            sprintf(uid, "%d", Logic::getInstance()->getUID());
            pd["uid"] = uid;
            pd["nickname"] = n;
            netLay->setEnabled(true);
            
            //pd["password"] =
            hm->addRequest("finReg", "POST", pd, this, MYHTTP_SEL(FinishReg::registerOver), NULL);
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

void FinishReg::registerOver(bool suc, std::string s, void *param) {
    if (!suc) {
        return;
    }
    rapidjson::Document d;
    d.Parse<0>(s.c_str());
    if(d["state"].GetInt() == 0){
        
    }else {
        //下次不用注册自动登录即可
        string n = name->getStringValue();
        Logic::getInstance()->nickname = n;
        
        
        CCUserDefault *u = CCUserDefault::sharedUserDefault();
        u->setStringForKey("loginName", Logic::getInstance()->loginName);
        u->setIntegerForKey("uid", Logic::getInstance()->getUID());
        u->setStringForKey("nickname", Logic::getInstance()->nickname);
        
        
        
        CCDirector::sharedDirector()->replaceScene(WorldCup::scene());
    }
}


void FinishReg::update(float df){
    
}