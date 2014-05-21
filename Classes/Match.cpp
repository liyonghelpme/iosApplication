//
//  Match.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#include "Match.h"
static MatchService *ms = NULL;
MatchService *MatchService::getInstance(){
    if (ms == NULL) {
        ms = new MatchService();
    }
    return ms;
}
MatchService::MatchService()
: requestYet(false)
, testNum(0)
, searchYet(false)
, dayRange(100)
, initMatchYet(false)
{
    d = new rapidjson::Document();
}


bool MatchService::getMatches(long long startTime, long long endTime) {
    if (requestYet) {
        return false;
    }
    realRequestMatch();
}

void MatchService::realRequestMatch(){
    //开始请求 未获得结果数据
    requestYet = true;
    initMatchYet = false;
    
    
    testNum++;
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    double today, tomorrow;
    
    if (!searchYet) {
        searchYet = true;
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
    
    hm->addRequest(t1, "GET", postData, this, MYHTTP_SEL(MatchService::initMatchOver), NULL);
    
}

void MatchService::initMatchOver(bool isSuc, string s, void *param){
    d->Parse<0>(s.c_str());
    //没有获得最后一条数据
    //最多 7*20 = 140 天的数据
    //CCLog("initMatchOver %d", (*d)["data"].Size());
    if ((*d)["data"].Size() == 0 && testNum < 20) {
        this->realRequestMatch();
    } else {
        initMatchYet = true;
    }
}




void MatchService::getMatchById(int tid) {
    
}