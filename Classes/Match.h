//
//  Match.h
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#ifndef __TestCpp__Match__
#define __TestCpp__Match__

#include <iostream>
#include <string.h>
#include "cocos2d.h"
#include "HttpModel.h"

using namespace std;

class Match : public CCObject{
public:
    int mid;
    string host;
    string guest;
    string result;
    string comment;
    long long startTime;
    long long endTime;
};


class MatchService : public CCObject {
public:
    bool getMatches(long long startTime, long long endTime);
    void getMatchById(int tid);
    
    
    MatchService();
    
    static MatchService *getInstance();
    
    
    rapidjson::Document *d;
    bool initMatchYet;
    bool requestYet;
private:
    void initMatchOver(bool isSuc, string s, void *param);
    
    bool searchYet;
    int testNum;
    double firstDay, lastDay, dayRange;
    
    
    void realRequestMatch();
};




#endif /* defined(__TestCpp__Match__) */
