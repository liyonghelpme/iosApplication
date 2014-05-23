//
//  Message.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//


#include "Message.h"
#include "HttpModel.h"
#include "Logic.h"


static MessageService *s_m = NULL;
MessageService *MessageService::getInstance(){
    if (s_m == NULL) {
        s_m = new MessageService();
    }
    return s_m;
}


//, long long startTime, long long endTime
void MessageService::getHistoryMessage(int cid, CCObject *obj, MyHttpResp rp){
    HttpModel *hm = HttpModel::getInstance();
    char buf[512];
    long long startTime, endTime;
    
    //当前时间
    time_t td;
    time(&td);
    //tm *timeinfo;
    //timeinfo = localtime(&(td));
    
    endTime = td+1000;//当期时间向后延续一些
    startTime = endTime-3600*24;
    
    startTime *= 1000;
    endTime *= 1000;
    
    sprintf(buf, "message/%d/%lld/%lld", cid, startTime, endTime);
    std::map<string, string> postData;
    
    hm->addRequest(buf, "GET", postData, obj, rp, NULL);
}
Channel *MessageService::getChannel(int cid){
    
}