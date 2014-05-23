//
//  用户服务类
//  BallParty
//
//  Created by dozengame on 14-5-13.
//
//

#include "UserService.h"

UserService* UserService::m_userService = NULL;
UserService* UserService::shareUserService()
{
	if(!m_userService)
	{
		m_userService = new UserService();
		CCEGLView* eglView = CCEGLView::sharedOpenGLView();
		CCSize visibleSize = eglView->getFrameSize();
	}
	return m_userService;
}
void UserService::login(string _loginName,string _password,CCNode* pTagert,MyHttpResp selector)
{
   
    HttpModel *hm = HttpModel::getInstance();
    std::map<std::string, std::string> postData;
    postData["loginName"]=_loginName;
    postData["password"]=_password;
    hm->addRequest(NET_LOGIN,"POST", postData, pTagert, selector, NULL);
}
void UserService::registe(string _loginName,string _password,string _passwordconfirm,string  _referencePhoneNumber,CCNode* pTaget,MyHttpResp selector)
{
    HttpModel *hm = HttpModel::getInstance();
    std::map<std::string, std::string> postData;
    postData["loginName"]=_loginName;
    postData["password"]=_password;
    postData["passwordconfirm"]=_passwordconfirm;
    postData["referrer"]=_referencePhoneNumber;
    //postData["phoneNumber"]=_phoneNumber;
    hm->addRequest(NET_REGIST,"POST", postData, pTaget, selector, NULL);

}


NetRegist UserService::analyzeRegistRect(string s)
{
    NetRegist regData;
    rapidjson::Document d;
    d.Parse<0>(s.c_str());
    regData.status = d["state"].GetInt();
    if(regData.status != 1){
        const rapidjson::Value &a = d["data"];
        int i = 0;
        regData.msg = a[i].GetString();
        //regData.msg = d["data"].GetString();
    }else{
        regData.msg = "";
    }
    return regData;
}

bool UserService::analyzeLoginRect(string s)
{
    rapidjson::Document d;
    d.Parse<0>(s.c_str());
    int re = d["state"].GetInt();
    

    //const rapidjson::Value &a = d["data"];
    
    if(re ==0)
    {
        return false;
    }else{
        return true;
    }
    return false;
}

