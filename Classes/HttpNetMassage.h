//
//  HttpNetMassage.h
//  BallParty
//  网络传输配置类
//  Created by dozengame on 14-5-14.
//
//

#ifndef BallParty_HttpNetMassage_h
#define BallParty_HttpNetMassage_h

#include "cocos2d.h"
using namespace std;
#define NET_HOST "http://172.17.0.145:91/"//ip地址
#define NET_LOGIN "user/login" //登陆
#define NET_REGIST "user/register"//注册



//消息基本数据结构..供底层使用
//登陆
typedef struct NetLogin
{


}NetLogin;
//注册
typedef struct NetRegist
{
    int status;
    string msg;
    
}NetRegist;
#endif
