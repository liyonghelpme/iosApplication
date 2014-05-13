//
//  HttpModel.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-13.
//
//

#include "HttpModel.h"
using namespace std;

static HttpModel *s_http = NULL;

HttpModel *HttpModel::getInstance(){
    if (s_http == NULL) {
        s_http = new HttpModel();
    }
    return s_http;
}
HttpModel::HttpModel(){
    baseUrl = "http://localhost:5000/";
}
struct TempData {
    CCObject *object;
    MyHttpResp psel;
    void *param;
};
void HttpModel::addRequest(string url, std::map<string, string> postData, CCObject *object, MyHttpResp psel, void *param){
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl((baseUrl+url).c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(HttpModel::handleHttp));
    request->setTag("test request");
    TempData *td = new TempData();
    td->object = object;
    td->psel = psel;
    td->param = param;
    request->setUserData(td);
    
    
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}
//返回一个 json 对象
void HttpModel::handleHttp(CCHttpClient *client, CCHttpResponse *resp){
    if (!resp) {
        return;
    }
    if (0 != strlen(resp->getHttpRequest()->getTag())) {
        CCLog("%s finish", resp->getHttpRequest()->getTag());
    }
    int statusCode = resp->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, resp->getHttpRequest()->getTag());
    if(!resp->isSucceed()){
        CCLog("response failed");
        CCLog("error buffer %s", resp->getErrorBuffer());
        return;
    }
    
    std::vector<char> *buffer = resp->getResponseData();
    for (unsigned int i = 0; i < buffer->size(); i++) {
        printf("%c", (*buffer)[i]);
    }
    printf("\n");
    string ch = string(buffer->begin(), buffer->end());
    
    /*
    rapidjson::Document d;
    d.Parse<0>(ch.c_str());
    printf("name is %s\n", d["loginName"].GetString());
    */
    TempData *td = (TempData*)resp->getHttpRequest()->getUserData();
    MyHttpResp cb = td->psel;
    //cb();
    ((*(td->object)).*cb)(true, ch, td->param);
    
    //(td->object->(*cb))(true, d, td->param);
    //(&cb)(td->object, true, d, td->param);
    delete td;
    
}