//
//  LoginView.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-16.
//
//

#include "LoginView.h"
#include "HttpModel.h"

CCScene *LoginView::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoginView *layer = LoginView::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LoginView::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    setSizeYet = false;
    inLogin = false;
    
    
    CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    UILayer *lay = UILayer::create();
    addChild(lay);
    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_4.json");
    lay->addWidget(w);
    w->setSize(size);
    
    password = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "password"));
    password->addEventListenerTextField(this, textfieldeventselector(LoginView::onPassword));
    
    login = static_cast<Button*>(UIHelper::seekWidgetByName(w, "login"));
    login->addTouchEventListener(this, toucheventselector(LoginView::onLogin));
    
    reg = static_cast<Button*>(UIHelper::seekWidgetByName(w, "register"));
    reg->addTouchEventListener(this, toucheventselector(LoginView::onReg));
    
    
    phoneNum = static_cast<TextField*>(UIHelper::seekWidgetByName(w, "TextField_9"));
    phoneNum->addEventListenerTextField(this, textfieldeventselector(LoginView::onPhonenum));
    
    error = static_cast<Label*>(UIHelper::seekWidgetByName(w, "error"));
    error->setEnabled(false);
    
    
    scheduleUpdate();
    
    CCUserDefault *u = CCUserDefault::sharedUserDefault();
    std::string uname = u->getStringForKey("username");
    
    //登录 或者注册
    if (uname.compare("") == 0) {
        
    //用户登录过 直接进入应用
    }else {
        
    }
    
    return true;
    
}
void LoginView::onPassword(CCObject *obj, TextFiledEventType tt) {
    switch (tt) {
        case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME:
        {
            //showKeyboard = true;
            //CCLog("setSizeYet %d",setSizeYet);
            //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, screenSize.height/2)));
            /*
            if (setSizeYet) {
                CCPoint wp = phoneNum->convertToWorldSpace(ccp(0, 0));
                CCSize ws = phoneNum->getSize();
                float offY = ksize.height+ws.height/2-wp.y;
                
                if (offY  > 0) {
                    w->runAction(CCMoveTo::create(0.225, ccp(0, offY)));
                }
                //bottom->runAction(CCMoveTo::create(0.225, ccp(0, ksize.height)));
                
            }
             */
            //CCEGLView *pegl = CCDirector::sharedDirector()->getOpenGLView();
            //pegl->setIMEKeyboardState(true);
            
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            //关闭键盘
            //CCLog("event detach with ime");
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, 0)));
            //CCEGLView *pegl = CCDirector::sharedDirector()->getOpenGLView();
            //pegl->setIMEKeyboardState(false);
            //w->runAction(CCMoveTo::create(0.225, ccp(0, 0)));
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT:
            break;
        default:
            break;
    }
}
void LoginView::onPhonenum(CCObject *obj, TextFiledEventType tt){
    switch (tt) {
        case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME:
        {
            //showKeyboard = true;
            //CCLog("setSizeYet %d",setSizeYet);
            //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, screenSize.height/2)));
            if (setSizeYet) {
                CCPoint wp = phoneNum->convertToWorldSpace(ccp(0, 0));
                CCSize ws = phoneNum->getSize();
                float offY = ksize.height+ws.height/2-wp.y;
                if (offY  > 0) {
                //bottom->runAction(CCMoveTo::create(0.225, ccp(0, ksize.height)));
                    w->runAction(CCMoveTo::create(0.225, ccp(0, offY)));
                }
            }
            //CCEGLView *pegl = CCDirector::sharedDirector()->getOpenGLView();
            //pegl->setIMEKeyboardState(true);
            
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            //关闭键盘
            //CCLog("event detach with ime");
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, 0)));
            //CCEGLView *pegl = CCDirector::sharedDirector()->getOpenGLView();
            //pegl->setIMEKeyboardState(false);
            w->runAction(CCMoveTo::create(0.225, ccp(0, 0)));
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT:
            break;
        default:
            break;
    }
}

void LoginView::onLogin(CCObject *obj, TouchEventType tt){
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
            if (inLogin) {
                return;
            }
            inLogin = true;
            HttpModel *hm = HttpModel::getInstance();
            std::map<string, string>pd;
            hm->addRequest("login", "GET", pd, this, MYHTTP_SEL(LoginView::loginOver), NULL);
            
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
void LoginView::loginOver(bool suc, std::string s, void *param) {
    rapidjson::Document d;
    d.Parse<0>(s.c_str());
    inLogin = false;
    if(d["state"].GetInt() == 0) {
        error->setEnabled(true);
        error->runAction(CCSequence::create(CCScaleTo::create(0.1, 1.2, 1.2), CCScaleTo::create(0.1, 1, 1), NULL));
    }else {
        
    }
    
}
void LoginView::onReg(CCObject *obj, TouchEventType tt) {
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


void LoginView::update(float diff){
    
}
