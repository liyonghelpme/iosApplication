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
#include "Match.h"
#include <vector>
using namespace std;



using namespace cocos2d;

extern bool DEBUG;

class Logic : CCObject {
public:
    vector<Match> allMatch;
    
    static Logic *getInstance();
    Logic();
    void initMatchInfo();
    
    //获取旧的比赛信息
    bool fetchOldMatchInfo();
    //获取新的比赛信息
    bool fetchNewMatchInfo();
    
    
    void initChatInfo();
    
    
    
    int getUID();
    
    //频道编号
    int getCID();
    
    
    bool initMatchYet;
    bool requestYet;
    //rapidjson::Document *d;
    
    
    
    
    
    void storeData();
    
    //当前选择的比赛信息
    //const rapidjson::Value *matchInfo;
    
    Match *matchInfo;
    
    //本地存储的声音文件的编号
    int getVid();
    int getImgId();
    
    bool  chatInfoYet;
    bool initChatYet;
    rapidjson::Document chatD;
    void clearTestNum(){
        testNum = 0;
    }
    
    //新获取的 数据的开始和结束索引
    int startInd;
    int endInd;
    
    int getSenderId();
    int getFlagId() {
        return flagId;
    }
    
    
    
    
    //登录 或者 注册完成后 需要设定的数据
    void setLoginName(string ln) {
        loginName = ln;
    }
    void setUID(int i);
    void setNickName(string nk) {
        nickname = nk;
    }
    string getNickName(){
        return nickname;
    }
    void setFlagId(int fd) {
        flagId = fd;
    }
    void setRealName(string rn) {
        realName = rn;
    }
    void setPhoneNumber(string pn) {
        phoneNumber = pn;
    }
    string getPhoneNumber() {
        return phoneNumber;
    }
    void setBio(string bi){
        bio = bi;
    }
    string getBio(){
        return bio;
    }
    
    string getGender() {
        if (gender == 1) {
            return "男";
        }else {
            return "女";
        }
    }
    
    string getLocal() {
        return area;
    }
    /*
    string getTeamName() {
        return "巴西";
    }
     */
    int fetchInfoState;
    
    void fetchInfo();
    
private:
    void fetchOver(bool isSuc, string s, void *param);
    int gender;
    string area;
    
    string realName;
    string phoneNumber;
    string bio;
    
    string loginName;
    string password;
    string nickname;
    
    int flagId;
    int vid;
    //本地缓存的图像ID 可以暂时不保存到本地文件系统里面 保存在聊天室里面即可
    int imgId;
    
    double firstDay;
    double lastDay;
    bool searchYet;
    
    int uid;
    
    void initMatchOver(bool isSuc, string s, void *);
    void initChatOver(bool isSuc, string s, void *);
    
    
    void initOldMatchOver(bool isSuc, string s, void *);
    void initNewMatchOver(bool isSuc, string s, void *);
    
    //尝试获得比赛信息
    int testNum;
    bool inRequest;
    
    
    bool realFetchNewMatch();
    bool realFetchOldMatch();
    
    
    bool noOld;
    bool noNew;
    
    
    
    double dayRange;
    
};
#endif /* defined(__TestCpp__Logic__) */
