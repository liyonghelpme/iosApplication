//
//  RegisterView.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-16.
//
//

#include "RegisterView.h"
#include "HttpModel.h"
#include "FinishReg.h"
#include "Logic.h"

CCScene *RegisterView::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RegisterView *layer = RegisterView::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool RegisterView::init() {
    if ( !CCLayer::init() )
    {
        return false;
    }
    inReg = false;
    
    CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    UILayer *lay = UILayer::create();
    addChild(lay);
    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_5.json");
    lay->addWidget(w);
    w->setSize(size);

    Button *reg = static_cast<Button*>(UIHelper::seekWidgetByName(w, "register"));
    reg->addTouchEventListener(this, toucheventselector(RegisterView::onReg));
    
    error = static_cast<Label*>(UIHelper::seekWidgetByName(w, "error"));
    password = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "password"));
    password1 = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "password1"));
    phonenum = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "phoneNum"));
    error->setEnabled(false);
    
    
    return true;
}

void RegisterView::onReg(cocos2d::CCObject *obj, TouchEventType tt){
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
            string pn = phonenum->getStringValue();
            string p1 = password->getStringValue();
            string p2 = password1->getStringValue();
            if (p1.compare(p2) != 0) {
                error->setEnabled(true);
                error->setText("两次输入的密码不一致");
                error->runAction(CCSequence::create(CCScaleTo::create(0.1, 1.2, 1.2), CCScaleTo::create(0.1, 1), NULL));
                return;
 
            }
            if (p1.length() == 0) {
                error->setEnabled(true);
                error->setText("密码不能为空");
                error->runAction(CCSequence::create(CCScaleTo::create(0.1, 1.2, 1.2), CCScaleTo::create(0.1, 1), NULL));
                return;
            }
            if (pn.length() == 0) {
                error->setEnabled(true);
                error->setText("手机号不能为空");
                error->runAction(CCSequence::create(CCScaleTo::create(0.1, 1.2, 1.2), CCScaleTo::create(0.1, 1), NULL));
                return;
            }
            
            
            inReg = true;
            HttpModel *hm = HttpModel::getInstance();
            std::map<string, string>pd;
            pd["loginName"] = pn;
            pd["password"] = p1;
            //pd["password"] =
            hm->addRequest("register", "POST", pd, this, MYHTTP_SEL(RegisterView::registerOver), NULL);
            
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


void RegisterView::registerOver(bool suc, std::string s, void *param){
    rapidjson::Document d;
    d.Parse<0>(s.c_str());
    inReg = false;
    if (d["state"].GetInt() == 0) {
        error->setEnabled(true);
        error->setText(d["err"].GetString());
        error->runAction(CCSequence::create(CCScaleTo::create(0.1, 1.2, 1.2), CCScaleTo::create(0.1, 1),  NULL));
    }else {
        
        string pn = phonenum->getStringValue();
        Logic::getInstance()->loginName = pn;
        Logic::getInstance()->setUID(d["uid"].GetInt());
        
        CCDirector::sharedDirector()->replaceScene(FinishReg::scene());
    }
}

void RegisterView::update(float diff){
    
}

