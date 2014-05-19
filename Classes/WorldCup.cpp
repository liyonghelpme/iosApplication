//
//  WorldCup.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-14.
//
//

#include "WorldCup.h"
#include "Logic.h"
#include "ChatView.h"

using namespace extension;
using namespace ui;


CCScene *WorldCup::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WorldCup *layer = WorldCup::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
bool WorldCup::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    showYet = false;
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCLog("winSize %f %f", size.width, size.height);
    
    UILayer *lay = UILayer::create();
    addChild(lay);
    //lay->setPosition(ccp(size.width/2, size.height/2));
    //lay->setContentSize(size);
    
    

    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_1.json");
    lay->addWidget(w);
    CCSize rs = w->getSize();
    CCLog("rootSize %f %f", rs.width, rs.height);
    w->setSizeType(SIZE_ABSOLUTE);
    w->setSize(size);
    
    /*
    UIPanel *pan = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "dayPanel"));
    UIListView *lv = static_cast<UIListView*>(UIHelper::seekWidgetByName(w, "listView"));
    
    UIWidget *pc = pan->clone();
    pc->setVisible(true);
    
    lv->pushBackCustomItem(pc);
    
    UIPanel *cupPanel = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "cupPanel"));
    UIWidget *cp = cupPanel->clone();
    cp->setVisible(true);
    lv->pushBackCustomItem(cp);
    
    lv->pushBackCustomItem(pc->clone());
    lv->pushBackCustomItem(cp->clone());
    lv->pushBackCustomItem(cp->clone());
    lv->pushBackCustomItem(cp->clone());
    */
    scheduleUpdate();
    
    return true;
    
}
void WorldCup::update(float diff){
    if (!showYet) {
        if (!Logic::getInstance()->requestYet) {
            Logic::getInstance()->initMatchInfo();
        }
        if (Logic::getInstance()->initMatchYet) {
            showYet = true;
            rapidjson::Document &d = Logic::getInstance()->d;
            const rapidjson::Value &b = d["matches"];
            UIPanel *pan = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "dayPanel"));
            pan->setEnabled(false);
            
            UIWidget *pc = pan->clone();
            pc->setVisible(true);
            pc->setEnabled(true);
            
            UILabel *dlab = static_cast<UILabel*>(UIHelper::seekWidgetByName(pc, "date"));
            
            UIPanel *cupPanel = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "cupPanel"));
            cupPanel->setEnabled(false);
            
            UIWidget *cp = cupPanel->clone();
            cp->setVisible(true);
            cp->setEnabled(true);
            
            
            UILabel *title = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "Label_11"));
            UILabel *online = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "Label_16"));
            UILabel *host = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "host"));
            UILabel *client = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "client"));
            
            UIListView *lv = static_cast<UIListView*>(UIHelper::seekWidgetByName(w, "listView"));
            string date;
            char on[1024];
            
            string emp = " ";
            for (rapidjson::SizeType i=0; i < b.Size(); i++) {
                const rapidjson::Value &c = b[i];
                string d = c["date"].GetString();
                //new Data push dayPanel
                //不同天
                
                if (d.compare(date) != 0) {
                    date = d;
                    string fd = c["week"].GetString()+emp+date;
                    dlab->setText(fd);
                    
                    lv->pushBackCustomItem(pc->clone());
                    
                    
                    /*
                    string tit = c["title"].GetString();
                    tit += c["time"].GetString();
                    title->setText(tit);
                    sprintf(on, "在线:%d人", c["online"].GetInt());
                    online->setText(on);
                    lv->pushBackCustomItem(cp->clone());
                    */
                    
                    //同一天
                }else {
                    //dlab->setText(date);
                    //lv->pushBackCustomItem(pc->clone());
                    
                }
                //添加比赛信息
                string tit = c["title"].GetString();
                tit += emp+ c["time"].GetString();
                title->setText(tit);
                
                sprintf(on, "在线:%d人", c["online"].GetInt());
                online->setText(on);
                host->setText(c["name1"].GetString());
                client->setText(c["name2"].GetString());
                
                Layout *ly = static_cast<Layout*>(cp->clone());
                Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "Button_12"));
                ly->setTag(c["id"].GetInt());
                bnt->setTag(c["id"].GetInt());
                bnt->addTouchEventListener(this, toucheventselector(WorldCup::onChat));
                lv->pushBackCustomItem(ly);
                
            }
            
        }
    }
}

void WorldCup::onChat(cocos2d::CCObject *obj, TouchEventType tt){
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
            Button *bnt = static_cast<Button*>(obj);
            int mid = bnt->getTag();
            rapidjson::Document &d = Logic::getInstance()->d;
            const rapidjson::Value &b = d["matches"];
            //const rapidjson::Value &c = b[mid];
            
            Logic::getInstance()->matchInfo = &b[mid];
            
            CCDirector::sharedDirector()->pushScene(ChatView::scene());
            
            
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

