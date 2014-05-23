//
//  Channel.h
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#ifndef __TestCpp__Channel__
#define __TestCpp__Channel__

#include <iostream>
#include <string.h>
#include "cocos2d.h"
#include "HttpModel.h"

using namespace std;
class Channel {
public:
    int cid;
    string serHost;
    string serPort;
    
};

//给当前用户所在的频道发送 文本信息  声音信息 和 图片信息
class ChannelService : public CCObject {
public:
    static ChannelService* getInstance();
    
    //基于同步方案设计的 消息接收机制
    //void sendMessage();
    //void onReceiveMessage();

    void channelSendText(std::string text);
    //监听当前用户打开的频道
    void listenChannel();
};
#endif /* defined(__TestCpp__Channel__) */
