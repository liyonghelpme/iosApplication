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

class ChannelService : public CCObject {
public:
    static ChannelService* getInstance();
    
    void sendMessage();
    void onReceiveMessage();

};
#endif /* defined(__TestCpp__Channel__) */
