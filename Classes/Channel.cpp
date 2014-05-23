//
//  Channel.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#include "Channel.h"
#include "RedisInterface.h"
static ChannelService* cs = NULL;
ChannelService* ChannelService::getInstance() {
    if (cs == NULL) {
        cs = new ChannelService();
    }
    return cs; 
}

void ChannelService::channelSendText(std::string text){
    sendText(text);
}

void ChannelService::listenChannel(){
    startReceiveRedis();
}