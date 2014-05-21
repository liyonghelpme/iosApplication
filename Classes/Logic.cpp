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
#include "writer.h"
#include "stringbuffer.h"

bool DEBUG = true;

static Logic *s_Logic = NULL;
Logic::Logic():
initMatchYet(false)
, requestYet(false)
, uid(1)
, vid(1)
, chatInfoYet(false)
, initChatYet(false)
, searchYet(false)
, matchInfo(NULL)
, testNum(0)
, inRequest(false)
, noOld(false)
, noNew(false)
, dayRange(30.0)
{
    d = new rapidjson::Document();
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
bool Logic::realFetchNewMatch(){
    testNum++;
    initMatchYet = false;
    inRequest = true;
    
    
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    double aday, bday;
    aday = lastDay;
    bday = lastDay+24*3600*dayRange*1000;
    lastDay = bday;
    
    CCLog("fetch new match %llf %llf", aday, bday);
    char t1[512];
    sprintf(t1, "match/%lld/%lld", (long long)aday, (long long)bday);
    
    hm->addRequest(t1, "GET", postData, this, MYHTTP_SEL(Logic::initNewMatchOver), NULL);
    
    return true;
}
bool Logic::fetchNewMatchInfo() {
    if (inRequest) {
        return false;
    }
    return realFetchNewMatch();
    
}

bool Logic::realFetchOldMatch(){
    testNum++;
    initMatchYet = false;
    inRequest = true;
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    //实际的显示的时间范围
    //lastDay = firstDay;
    
    double aday, bday;
    aday = firstDay-24*3600*dayRange*1000;
    bday = firstDay;
    firstDay = aday;
    
    //firstDay = firstDay-24*3600*7*1000;
    
    char t1[512];
    sprintf(t1, "match/%lld/%lld", (long long)aday, (long long)bday);
    
    
    hm->addRequest(t1, "GET", postData, this, MYHTTP_SEL(Logic::initOldMatchOver), NULL);
    return true;
}

//没有在请求状态才能获取比赛信息
//如果连着7天都没有比赛则无法获取到比赛信息了
//获取旧的比赛信息
bool Logic::fetchOldMatchInfo(){
    if (inRequest) {
        return false;
    }
    return realFetchOldMatch();
}


void Logic::initNewMatchOver(bool isSuc, string s, void *param){
    if (isSuc) {
        rapidjson::Document *d2 = new rapidjson::Document();
        
        (*d2).Parse<0>(s.c_str());
        //没有获得最后一条数据
        //最多 7*4 = 140 天的数据
        CCLog("initNewMatchOver %d %d %s", (*d2)["data"].Size(), testNum, s.c_str());
        
        int gsize = (*d2)["data"].Size();
        if (gsize == 0 && testNum < 3) {
            delete d2;
            realFetchNewMatch();
        } else {
            if (gsize == 0) {
                noNew = true;
            }
            
            initMatchYet = true;
            inRequest = false;
            
            startInd = (*d)["data"].Size();
            endInd = startInd+(*d2)["data"].Size();
            
            rapidjson::Document::AllocatorType &alloc = d->GetAllocator();
            
            int size =(*d2)["data"].Size();
            CCLog("old data %d", (*d2)["data"].Size());
            //popBack 会导致整个数组 被销毁
            //将d2 拼接到 d 上面
            for (int i = 0; i < size; i++) {
                rapidjson::Value &v = (*d2)["data"][i];
                (*d)["data"].PushBack(v, alloc);
            }
            /*
            while (size > 0) {
                rapidjson::Value &v = (*d2)["data"].PopBack();
                CCLog("value type %d", v.GetType());
                //v.Accept(<#Handler &handler#>)
                rapidjson::StringBuffer strbuf;
                rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
                v.Accept(writer);
                
                CCLog("v value %s", strbuf.GetString());
                
                (*d)["data"].PushBack(v, alloc);
                size--;
            }
             */
            
            /*
             for (int i = 0; i < (*d)["data"].Size(); i++) {
             rapidjson::Value &v = (*d)["data"].PopBack();
             (*d2)["data"].PushBack(v, alloc);
             }
             */
            //d = d2;
            delete d2;
        }
        
    }
}

void Logic::initOldMatchOver(bool isSuc, string s, void *param){
    if (isSuc) {
        
        rapidjson::Document *d2 = new rapidjson::Document();
        
        (*d2).Parse<0>(s.c_str());
        //没有获得最后一条数据
        //最多 7*4 = 140 天的数据
        CCLog("initOldMatchOver %d %d", (*d2)["data"].Size(), testNum);
        if ((*d2)["data"].Size() == 0 && testNum < 3) {
            delete d2;
            realFetchOldMatch();
        } else {
            initMatchYet = true;
            inRequest = false;
            
            
            startInd = 0;
            endInd = (*d2)["data"].Size();
            rapidjson::Document::AllocatorType &alloc = d2->GetAllocator();
            CCLog("old data %d", (*d)["data"].Size());
            int size =(*d)["data"].Size();
            
            
            for (int i = 0; i < size; i++) {
                rapidjson::Value &v = (*d)["data"][i];
                (*d2)["data"].PushBack(v, alloc);
            }
            //popBack 会导致整个数组 被销毁
            /*
            while (size > 0) {
                rapidjson::Value &v = (*d)["data"].PopBack();
                (*d2)["data"].PushBack(v, alloc);
                size--;
            }
             */
            
            /*
            for (int i = 0; i < (*d)["data"].Size(); i++) {
                rapidjson::Value &v = (*d)["data"].PopBack();
                (*d2)["data"].PushBack(v, alloc);
            }
             */
            delete d;
            d = d2;
        }
    }
}








void Logic::initMatchInfo(){
    requestYet = true;
    testNum++;
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    
    //postData["starttime"] = "0";
    //postData["endtime"] = "99999999";
    double today, tomorrow;
    
    if (!searchYet) {
        searchYet = true;
        //struct timeval tv;
        //gettimeofday(&tv, NULL);
        //today = tv.tv_sec+(tv.tv_usec/1000000.0);
        time_t td;
        time(&td);
        
        //假设 time_t 对应的是秒
        tm *timeinfo;
        timeinfo = localtime(&(td));
        
        //修改为本天开始的时间
        timeinfo->tm_min = 0;
        timeinfo->tm_sec = 0;
        timeinfo->tm_hour = 0;
        
        today = mktime(timeinfo);
        
        CCLog("today time %ld %lf", td, today);
        
        tomorrow = today+24*3600*dayRange;
        today *= 1000;
        tomorrow *= 1000;
        firstDay = today;
        lastDay = tomorrow;
    } else {
        //向后推迟7天
        //firstDay = lastDay;
        today = lastDay;
        tomorrow = lastDay+24*3600*dayRange*1000;
        lastDay = tomorrow;
        
        //lastDay += 24*3600*7*1000;
        
        //today = firstDay;
        //tomorrow = lastDay;
    }
    
    //从今天开始的比赛
    char t1[512];
    sprintf(t1, "match/%lld/%lld", (long long)today, (long long)tomorrow);
    
    hm->addRequest(t1, "GET", postData, this, MYHTTP_SEL(Logic::initMatchOver), NULL);
}

void Logic::initChatInfo() {
    chatInfoYet = true;
    
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    int cid;
    //if (DEBUG) {
    //    cid = 0;
    //}else {
        cid = (*matchInfo)["id"].GetInt();
    //}
    char buf[128];
    //sprintf(v, "%d", cid);
    //postData["cid"] = v;
    
    sprintf(buf, "match/%d/user", cid);
    //hm->addRequest("getChatInfo", "GET", postData, this, MYHTTP_SEL(Logic::initChatOver), NULL);
    hm->addRequest(buf, "GET", postData, this, MYHTTP_SEL(Logic::initChatOver), NULL);
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
    d->Parse<0>(s.c_str());
    //没有获得最后一条数据
    //最多 7*20 = 140 天的数据
    CCLog("initMatchOver %d", (*d)["data"].Size());
    if ((*d)["data"].Size() == 0 && testNum < 20) {
        this->initMatchInfo();
    } else {
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
//比赛本身的id 作为频道ID
int Logic::getCID() {
    if (matchInfo != NULL) {
        return (*matchInfo)["id"].GetInt();
    }
    return 0;
}
