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
#include "ConfigView.h"
#include "FriendView.h"
#include <stdlib.h>
#include "writer.h"
#include "stringbuffer.h"

//#include "Match.h"

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
WorldCup::~WorldCup(){
    dict->release();
}

bool WorldCup::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    showYet = false;
    inUpdateData = false;
    oldOrNew = 0;
    lastRefreshTime = 0;
    inRefresh = false;
    lastUpdateIndex = 0;
    dict = CCDictionary::create();
    dict->retain();
    
    
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCLog("winSize %f %f", size.width, size.height);
    
    UILayer *lay = UILayer::create();
    addChild(lay);
    //lay->setPosition(ccp(size.width/2, size.height/2));
    //lay->setContentSize(size);
    
    

    //w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_1.json");
    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_1_0.json");
    lay->addWidget(w);
    CCSize rs = w->getSize();
    CCLog("rootSize %f %f", rs.width, rs.height);
    w->setSizeType(SIZE_ABSOLUTE);
    w->setSize(size);
    
    Button *conf = static_cast<Button*>(UIHelper::seekWidgetByName(w, "selfinfo"));
    conf->addTouchEventListener(this, toucheventselector(WorldCup::onConf));
    /*
    
    */
    /*
    Button *fri = static_cast<Button*>(UIHelper::seekWidgetByName(w, "Button_5"));
    fri->addTouchEventListener(this, toucheventselector(WorldCup::onFri));
    */
    
    lv = static_cast<UIListView*>(UIHelper::seekWidgetByName(w, "listView"));
    lv->addEventListenerListView(this, listvieweventselector(WorldCup::onListview));
    lv->addEventListenerScrollView(this, scrollvieweventselector(WorldCup::onScroll));
    
    
    UIPanel *pan = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "dayPanel"));
    pan->setEnabled(false);
    
    pc = pan->clone();
    pc->setVisible(true);
    pc->setEnabled(true);
    pc->retain();
    
    dlab = static_cast<UILabel*>(UIHelper::seekWidgetByName(pc, "date"));
    
    UIPanel *cupPanel = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "cupPanel"));
    cupPanel->setEnabled(false);
    //整个Panel 大小一样的 按钮
    //Button *full = static_cast<Button*>(UIHelper::seekWidgetByName(cupPanel, "full"));
    //full->setVisible(false);
    
    
    cp = cupPanel->clone();
    cp->setVisible(true);
    cp->setEnabled(true);
    cp->retain();
    
    
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


//当移动到 顶部或者 底部 暂停滑动 更新数据
void WorldCup::onScroll(cocos2d::CCObject *obj, ScrollviewEventType st){
    CCLog("scroll event %d", st);
    switch (st) {
        //取消该功能
        
        //放手后回到顶部开始
        case cocos2d::ui::SCROLLVIEW_EVENT_SCROLL_TO_TOP:
        {
            if (!inUpdateData) {
                inUpdateData = true;
                oldOrNew = 0;
                bool fs = Logic::getInstance()->fetchOldMatchInfo();
                if (!fs) {
                    inUpdateData = false;
                } else {
                    Logic::getInstance()->clearTestNum();
                }
            }
        }
            break;
        //放手后 回到底部开始
        case cocos2d::ui::SCROLLVIEW_EVENT_SCROLL_TO_BOTTOM:
        {
            if (!inUpdateData) {
                inUpdateData = true;
                oldOrNew = 1;
                bool fs = Logic::getInstance()->fetchNewMatchInfo();
                if (!fs) {
                    inUpdateData = false;
                }else {
                    Logic::getInstance()->clearTestNum();
                }
            }
        }
            break;
         
        default:
            break;
    }
}


void WorldCup::onListview(cocos2d::CCObject *obj, ListViewEventType lt) {
    switch (lt) {
        case cocos2d::ui::LISTVIEW_ONSELECTEDITEM_START:
        {
            //CCLog("item number %d %f %f", lv->getItems()->count(), lv->getSize().width, lv->getSize().height);
        }
            break;
        case cocos2d::ui::LISTVIEW_ONSELECTEDITEM_END:
        {
            
        }
            break;
        default:
            break;
    }
}


void WorldCup::onFri(cocos2d::CCObject *obj, TouchEventType tt){
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
            CCDirector::sharedDirector()->pushScene(FriendView::scene());
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
void WorldCup::onConf(cocos2d::CCObject *obj, TouchEventType tt){
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
            CCDirector::sharedDirector()->pushScene(ConfigView::scene());
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
void WorldCup::onRefresh(bool isSuc, std::string s, void *p){
    inRefresh = false;
    if (isSuc) {
        rapidjson::Document d;
        d.Parse<0>(s.c_str());
        int count = d["data"].Size();
        
        //int count = d["count"].GetInt();
        //cid 对应的 Layout对象
        Layout *ly = (Layout*)(dict->objectForKey(*(int*)p));
        free(p);
        
        Label *onlineNum = (Label*)(UIHelper::seekWidgetByName(ly, "Label_16"));
        char onum[128];
        sprintf(onum, "在线: %d人", count);
        onlineNum->setText(onum);
        
        CCLog("online people %d", count);
    }
}

//更新数据所对应的 item的条目是那一条呢？
void WorldCup::refreshOnlineNum(float diff){
    return;
    
    if (!inRefresh) {
        lastRefreshTime = lastRefreshTime-diff;
        if (lastRefreshTime <= 0 ) {
            //inRefresh = true;
            
            HttpModel *hm = HttpModel::getInstance();
            std::map<string, string> postData;
            rapidjson::Document *d = Logic::getInstance()->d;
            //初始化比赛信息结束 之后 才可以 刷新比赛的 在线人数信息
            if ((*d).IsObject()) {
            
                if (!(*d)["data"].IsNull() && lastUpdateIndex < (*d)["data"].Size()) {
                    inRefresh = true;
                    //请求的
                    int mid = (*d)["data"][lastUpdateIndex++]["id"].GetInt();
                    char buf[128];
                    sprintf(buf, "%d", mid);
                    postData["cid"] =  buf;
                    
                
                    int *cid = (int*)malloc(sizeof(int));
                    *cid = mid;
                
                    //hm->addRequest("getUserCount", "GET", postData, this, MYHTTP_SEL(WorldCup::onRefresh), cid);
                    sprintf(buf, "match/%d/user", *cid);
                    hm->addRequest(buf, "GET", postData, this, MYHTTP_SEL(WorldCup::onRefresh), cid);
                                   
                }else {
                    //超过长度 则 处于refresh 状态
                    //inRefresh = true;
                    lastUpdateIndex = 0;
                    //下次更新100s之后
                    lastRefreshTime = 5;
                }
            }
        }
    }
}


void WorldCup::showBasic(){
    if (!Logic::getInstance()->requestYet && !inUpdateData) {
        //inUpdateData = true;
        
        Logic::getInstance()->initMatchInfo();
        //MatchService::getInstance()->getMatches(0, 0);
        
    }
    
    if (Logic::getInstance()->initMatchYet) {
        showYet = true;
        //inUpdateData = false;
        
        //rapidjson::Document *d = MatchService::getInstance()->d;
        rapidjson::Document *d = Logic::getInstance()->d;
        //const rapidjson::Value &b = d["matches"];
        
        const rapidjson::Value &b = (*d)["data"];
        
        
        UILabel *title = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "round"));
        UILabel *online = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "online"));
        UILabel *host = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "host"));
        UILabel *client = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "client"));
        //Button *
        
        //string date;
        char on[1024];
        
        string emp = " ";
        int date = -1;
        string DAY[] = {
            "周日", "周一", "周二", "周三", "周四", "周五", "周六",
        };
        time_t now;
        time(&now);
        
        for (rapidjson::SizeType i=0; i < b.Size(); i++) {
            const rapidjson::Value &c = b[i];
            //string d = c["date"].GetString();
            //毫秒
            long long start_time = c["start_time"].GetUint64();
            start_time /= 1000;
            
            tm *timeinfo;
            timeinfo = localtime((time_t*)&(start_time));
            int wd = timeinfo->tm_wday;
            int day = timeinfo->tm_yday;
            int mon = timeinfo->tm_mon;
            int mday = timeinfo->tm_mday;
            int hour = timeinfo->tm_hour;
            int min = timeinfo->tm_min;
            //new Data push dayPanel
            //不同天
            char minfo[128];
            sprintf(minfo, "%.2d/%.2d", mon+1, mday);
            if (date != day) {
                date = day;
                string fd = DAY[wd]+emp+minfo;
                dlab->setText(fd);
                lv->pushBackCustomItem(pc->clone());
            }
            
            
            //添加比赛信息
            sprintf(minfo, "%.2d:%.2d", hour, min);
            
            string tit = c["cate_name"].GetString();
            if(!c["title"].IsNull()) {
                tit += emp+c["title"].GetString()+emp+minfo;
            }
            //string tit = c["title"].GetString();
            //tit += emp+ c["time"].GetString();
            
            title->setText(tit);
            
            int ol = 0;
            //c["online"].GetInt()
            sprintf(on, "在线:%d人", ol);
            online->setText(on);
            host->setText(c["host_name"].GetString());
            client->setText(c["guest_name"].GetString());
            
            Layout *ly = static_cast<Layout*>(cp->clone());
            
            long long end_time = c["end_time"].GetUint64();
            end_time /= 1000;
            
            
            //Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "Button_12"));
            Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "chatButton"));
            CCLog("chat button init");
            
            Button *realBnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "realBnt"));
            
            Label *state = static_cast<Label*>(UIHelper::seekWidgetByName(ly, "state"));
            Label *onl = static_cast<Label*>(UIHelper::seekWidgetByName(ly, "online"));
            
            if (now >= end_time) {
                onl->setText("完场");
                bnt->setTouchEnabled(false);
                state->setText("");
                //state->setText("已经结束");
                //bnt->setTitleText("已经结束");
            } else if(now >= start_time) {
                state->setText("比赛中");
                //bnt->setTitleText("立即进入");
                
            } else {
                state->setText("尚未开始");
                //bnt->setTitleText("尚未开始");
            }
            
            
            
            
            ly->setTag(c["id"].GetInt());
            //use Match Id
            
            //bnt->setTag();
            //bnt->addTouchEventListener(this, toucheventselector(WorldCup::onChat));
            int cid = c["id"].GetInt();
            
            realBnt->setTag(cid);
            realBnt->addTouchEventListener(this, toucheventselector(WorldCup::onChat));
            //比赛信息对应的 选择Item
            dict->setObject(ly, c["id"].GetInt());
            
            lv->pushBackCustomItem(ly);
            
        }
        
    }
}
void WorldCup::showScroll() {
    if (inUpdateData && Logic::getInstance()->initMatchYet) {
        //return;
        
        inUpdateData = false;
        
        rapidjson::Document *d = Logic::getInstance()->d;
        //const rapidjson::Value &b = d["matches"];
        
        const rapidjson::Value &b = (*d)["data"];
        
        UILabel *title = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "round"));
        UILabel *online = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "online"));
        UILabel *host = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "host"));
        UILabel *client = static_cast<UILabel*>(UIHelper::seekWidgetByName(cp, "client"));
        
        
        //string date;
        char on[1024];
        
        string emp = " ";
        int date = -1;
        string DAY[] = {
            "周日", "周一", "周二", "周三", "周四", "周五", "周六",
        };
        
        //insert at lastPosition or first Position
        int ordInd;
        if (Logic::getInstance()->startInd == 0) {
            ordInd = 0;
        }else {
            ordInd = lv->getItems()->count();
        }
        //int ordInd = Logic::getInstance()->startInd;
        
        
        
        //插到头部 或者插在尾部
        Logic *li = Logic::getInstance();
        CCLog("start end index %d %d %d", li->startInd, li->endInd, b.Size());
        
        time_t now;
        time(&now);
        for (rapidjson::SizeType i=Logic::getInstance()->startInd; i < Logic::getInstance()->endInd; i++) {
            const rapidjson::Value &c = b[i];
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            c.Accept(writer);
            CCLog("c is %s", strbuf.GetString());
            
            //string d = c["date"].GetString();
            //CCLog(c.IsObject());
            CCLog("type %d", c.GetType());
            //毫秒
            //BUG:
            long long start_time = c["start_time"].GetUint64();
            start_time /= 1000;
            
            tm *timeinfo;
            timeinfo = localtime((time_t*)&(start_time));
            int wd = timeinfo->tm_wday;
            int day = timeinfo->tm_yday;
            int mon = timeinfo->tm_mon;
            int mday = timeinfo->tm_mday;
            int hour = timeinfo->tm_hour;
            int min = timeinfo->tm_min;
            //new Data push dayPanel
            //不同天
            char minfo[128];
            sprintf(minfo, "%.2d/%.2d", mon+1, mday);
            if (date != day) {
                date = day;
                string fd = DAY[wd]+emp+minfo;
                dlab->setText(fd);
                lv->insertCustomItem(pc->clone(), ordInd++);
                //lv->pushBackCustomItem(pc->clone());
            }
            
            //添加比赛信息
            sprintf(minfo, "%.2d:%.2d", hour, min);
            
            string tit = c["cate_name"].GetString();
            tit += emp+c["title"].GetString()+emp+minfo;
            
            //string tit = c["title"].GetString();
            //tit += emp+ c["time"].GetString();
            
            title->setText(tit);
            
            int ol = 0;
            //c["online"].GetInt()
            sprintf(on, "在线:%d人", ol);
            online->setText(on);
            host->setText(c["host_name"].GetString());
            client->setText(c["guest_name"].GetString());
            
            Layout *ly = static_cast<Layout*>(cp->clone());
            //Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "Button_12"));
            //Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "full"));
            
            long long end_time = c["end_time"].GetUint64();
            end_time /= 1000;
            /*
            if (now >= end_time) {
                bnt->setTitleText("已经结束");
            } else if(now >= start_time) {
                bnt->setTitleText("立即进入");
            } else {
                bnt->setTitleText("尚未开始");
            }
             */
            
            //Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "Button_12"));
            Button *bnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "chatButton"));
            CCLog("chat button init");
            
            Button *realBnt = static_cast<Button*>(UIHelper::seekWidgetByName(ly, "realBnt"));
            
            Label *state = static_cast<Label*>(UIHelper::seekWidgetByName(ly, "state"));
            Label *onl = static_cast<Label*>(UIHelper::seekWidgetByName(ly, "online"));
            
            if (now >= end_time) {
                onl->setText("完场");
                bnt->setTouchEnabled(false);
                state->setText("");
                //state->setText("已经结束");
                //bnt->setTitleText("已经结束");
            } else if(now >= start_time) {
                state->setText("比赛中");
                //bnt->setTitleText("立即进入");
                
            } else {
                state->setText("尚未开始");
                //bnt->setTitleText("尚未开始");
            }
            
            
            int cid = c["id"].GetInt();
            ly->setTag(cid);
            
            //bnt->setTag(c["id"].GetInt());
            //bnt->addTouchEventListener(this, toucheventselector(WorldCup::onChat));
            
            realBnt->setTag(cid);
            realBnt->addTouchEventListener(this, toucheventselector(WorldCup::onChat));
            
            //lv->pushBackCustomItem(ly);
            //添加 比赛id 对应的 item 映射关系
            dict->setObject(ly, c["id"].GetInt());
            lv->insertCustomItem(ly, ordInd++);
        }
        
    }
}

//停止更新数据
void WorldCup::update(float diff){
    refreshOnlineNum(diff);
    //为显示的时候 首先显示基本页面 数据
    if (!showYet) {
        showBasic();
    } else {
        showScroll();
    }
    
    //暂时不用这个
    //正在等待网络更新比赛数据 向上 向下
    //网络数据已经获得了
    
    
}

void WorldCup::onChat(cocos2d::CCObject *obj, TouchEventType tt){
    CCLog("onChat Button");
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
            rapidjson::Document *d = Logic::getInstance()->d;
            const rapidjson::Value &b = (*d)["data"];
            //const rapidjson::Value &c = b[mid];
            
            int realid = 0;
            for (int i = 0; i < b.Size(); i++) {
                if (b[i]["id"].GetInt() == mid) {
                    realid = i;
                    break;
                }
            }
            Logic::getInstance()->matchInfo = &b[realid];
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

