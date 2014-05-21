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


static User *s_su = NULL;
User *User::getInstance() {
    if (s_su == NULL) {
        s_su = new User();
    }
    return s_su;
}
User::User():
loginOk(false)
{
    
}

void User::login(string loginName, string password, CCObject *obj, MyHttpResp rp){
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string>pd;
    pd["loginName"] = loginName;
    pd["password"] = md5(password);
    
    hm->addRequest("user/login", "POST", pd, obj, rp, NULL);
}




void User::registerUser(string loginName, string password, string realName, string phoneNum, string referencePhoneNumber, CCObject*obj, MyHttpResp rp){
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string>pd;
    pd["loginName"] = loginName;
    pd["password"] = md5(password);
    
    //pd["password"] =
    hm->addRequest("user/register", "POST", pd, obj, rp, NULL);
}

void User::saveProfile(){
    
}
void User::changePassword(){
    
}
void User::confirmReference(){
    
}