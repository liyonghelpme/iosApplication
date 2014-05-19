//
//  Logic.h
//  TestCpp
//
//  Created by dozengame on 14-5-14.
//
//

#ifndef __TestCpp__Logic__
#define __TestCpp__Logic__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpModel.h"

using namespace cocos2d;

class Logic : CCObject {
public:
    static Logic *getInstance();
    Logic();
    void initMatchInfo();
    
    int getUID() {
        return uid;
    }
    void setUID(int i){
        uid = i;
    }
    
    bool initMatchYet;
    bool requestYet;
    rapidjson::Document d;
    
    
    
    string loginName;
    string password;
    string nickname;
    
    //当前选择的比赛信息
    const rapidjson::Value *matchInfo;
private:
    int uid;
    
    void initMatchOver(bool isSuc, string s, void *);
};
#endif /* defined(__TestCpp__Logic__) */
