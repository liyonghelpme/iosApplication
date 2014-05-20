//
//  Logic.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-14.
//
//

#include "Logic.h"
#include <string>
#include <map>

bool DEBUG = true;

static Logic *s_Logic = NULL;
Logic::Logic():
initMatchYet(false)
, requestYet(false)
, uid(0)
, vid(1)
, chatInfoYet(false)
, initChatYet(false)
, searchYet(false)
, matchInfo(NULL)
, testNum(0)
{
    
}
int Logic::getImgId(){
    return imgId++;
}
int Logic::getVid(){
    return vid++;
}

int Logic::getUID() {
    return uid;
}
void Logic::setUID(int i){
    uid = i;
}
Logic *Logic::getInstance(){
    if (s_Logic == NULL) {
        s_Logic = new Logic();
    }
    return s_Logic;
}

void Logic::initMatchInfo(){
    requestYet = true;
    testNum++;
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    
    //postData["starttime"] = "0";
    //postData["endtime"] = "99999999";
    double tm, tomorrow;
    if (!searchYet) {
        searchYet = true;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        tm = tv.tv_sec+(tv.tv_usec/1000000.0);
        tomorrow = tm+24*3600*7;
        tm *= 1000;
        tomorrow *= 1000;
        firstDay = tm;
        lastDay = tomorrow;
    } else {
        //向后推迟7天
        firstDay = lastDay;
        lastDay += 24*3600*7*1000;
        tm = firstDay;
        tomorrow = lastDay;
    }
    
    char t1[512];
    sprintf(t1, "match/%lld/%lld", (long long)tm, (long long)tomorrow);
    
    hm->addRequest(t1, "GET", postData, this, MYHTTP_SEL(Logic::initMatchOver), NULL);
}

void Logic::initChatInfo() {
    chatInfoYet = true;
    
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    int cid;
    if (DEBUG) {
        cid = 0;
    }else {
        cid = (*matchInfo)["id"].GetInt();
    }
    char v[128];
    sprintf(v, "%d", cid);
    postData["cid"] = v;
    hm->addRequest("getChatInfo", "GET", postData, this, MYHTTP_SEL(Logic::initChatOver), NULL);
}
void Logic::initChatOver(bool isSuc, string s, void *param){
    initChatYet = true;
    if (isSuc) {
        chatD.Parse<0>(s.c_str());
    }
}

void Logic::initMatchOver(bool isSuc, string s, void *param) {
    //initMatchYet = true;
    //rapidjson::Document d;
    d.Parse<0>(s.c_str());
    //没有获得最后一条数据
    //最多 7*20 = 140 天的数据
    if (d["data"].Size() == 0 && testNum < 20) {
        this->initChatInfo();
    } else {
        CCLog("initMatchOver %d", d["data"].Size());
        initMatchYet = true;
    }
    
    /*
    if (d["state"].GetInt() == 0) {
        return;
    }
    */
    
    
    /*
    const rapidjson::Value&b = d["matches"];
    for(rapidjson::SizeType i=0; i < b.Size(); i++) {
        const rapidjson::Value &c = b[i];
        
    }
    */
}
void Logic::storeData() {
    CCUserDefault *u = CCUserDefault::sharedUserDefault();
    u->setStringForKey("loginName", loginName);
    u->setStringForKey("nickname", nickname);
    u->setIntegerForKey("uid", uid);
}
int Logic::getCID() {
    if (matchInfo != NULL) {
        return (*matchInfo)["cid"].GetInt();
    }
    return 0;
}
