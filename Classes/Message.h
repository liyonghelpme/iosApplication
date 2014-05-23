//
//  Message.h
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#ifndef __TestCpp__Message__
#define __TestCpp__Message__

#include <iostream>
#include <string.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpModel.h"
#include "Channel.h"

using namespace std;
using namespace cocos2d;
using namespace extension;
using namespace ui;

class Message{
public:
    int mid;
    int sender;
    string senderName;
    int mtype;
    string content;
    long long send_time;
};

class MessageService : public CCObject {
public:
    static MessageService *getInstance();
    
    //当前聊天的频道
    Channel *getChannel(int channelId);
    //获取服务器历史消息
    //, long long startTime, long long endTime
    void getHistoryMessage(int cid, CCObject *, MyHttpResp);

private:
    
};


#endif /* defined(__TestCpp__Message__) */
