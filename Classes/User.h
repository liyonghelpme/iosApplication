//
//  User.h
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#ifndef __TestCpp__User__
#define __TestCpp__User__

#include <iostream>
#include <string.h>
#include "cocos2d.h"
#include "HttpModel.h"

using namespace std;
class User : public cocos2d::CCObject{
public:
    static User *getInstance();
    
    int uid;
    string loginName;
    string password;
    string realName;
    string phoneNumber;
    string bio;
    long long registerTime;
    
    
    void login(string loginName, string password, CCObject *, MyHttpResp rp);
    
    void registerUser(string loginName, string password, string realName, string phoneNum, string referencePhoneNumber, CCObject*, MyHttpResp rp);
    void saveProfile();
    void changePassword();
    void confirmReference();
    
    
    User();
private:
    bool loginOk;
};

#endif /* defined(__TestCpp__User__) */
