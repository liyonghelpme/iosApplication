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
using namespace std;
class User{
public:
    int uid;
    string loginName;
    string password;
    string realName;
    string phoneNumber;
    string bio;
    long long registerTime;
    
    void login(string loginName, string password);
    void registerUser(string loginName, string password, string realName, string phoneNum, string referencePhoneNumber);
    void saveProfile();
    void changePassword();
    void confirmReference();
    
};

#endif /* defined(__TestCpp__User__) */
