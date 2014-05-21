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
using namespace std;
class Message{
public:
    int uid;
    int mtype;
    string content;
    long long send_time;
    
    
    int getChannel(int channelId);
    void getHistoryMessage(int cid, long long startTime, long long endTime);
    
};


#endif /* defined(__TestCpp__Message__) */
