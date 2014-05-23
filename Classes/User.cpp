//
//  User.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#include "User.h"
#include "HttpModel.h"
#include "Md5.h"

/*
static UserService *s_su = NULL;
UserService *UserService::getInstance() {
    if (s_su == NULL) {
        s_su = new UserService();
    }
    return s_su;
}
UserService::UserService()
{
    
}

void UserService::login(string loginName, string password, CCObject *obj, MyHttpResp rp){
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string>pd;
    pd["loginName"] = loginName;
    pd["password"] = md5(password);
    
    hm->addRequest("user/login", "POST", pd, obj, rp, NULL);
}




void UserService::registerUser(string loginName, string password, string realName, string phoneNum, string referencePhoneNumber, CCObject*obj, MyHttpResp rp){
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string>pd;
    pd["loginName"] = loginName;
    pd["password"] = md5(password);
    
    //pd["password"] =
    hm->addRequest("user/register", "POST", pd, obj, rp, NULL);
}

void UserService::saveProfile(){
    
}
void UserService::changePassword(){
    
}
void UserService::confirmReference(){
    
}
*/