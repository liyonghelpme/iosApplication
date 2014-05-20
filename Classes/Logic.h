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

extern bool DEBUG;

class Logic : CCObject {
public:
    static Logic *getInstance();
    Logic();
    void initMatchInfo();
    void initChatInfo();
    
    int getUID();
    void setUID(int i);
    //频道编号
    int getCID();
    
    
    bool initMatchYet;
    bool requestYet;
    rapidjson::Document d;
    
    
    
    string loginName;
    string password;
    string nickname;
    
    void storeData();
    
    //当前选择的比赛信息
    const rapidjson::Value *matchInfo;
    
    //本地存储的声音文件的编号
    int getVid();
    int getImgId();
    
    bool  chatInfoYet;
    bool initChatYet;
    rapidjson::Document chatD;
private:
    int vid;
    //本地缓存的图像ID 可以暂时不保存到本地文件系统里面 保存在聊天室里面即可
    int imgId;
    
    double firstDay;
    double lastDay;
    bool searchYet;
    
    int uid;
    
    void initMatchOver(bool isSuc, string s, void *);
    void initChatOver(bool isSuc, string s, void *);
    
    //尝试获得比赛信息
    int testNum;
};
#endif /* defined(__TestCpp__Logic__) */
