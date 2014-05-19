//
//  Logic.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-14.
//
//

#include "Logic.h"
#include <string>
#include <map>
static Logic *s_Logic = NULL;
Logic::Logic():
initMatchYet(false)
, requestYet(false)
, uid(0)
{
    
}
Logic *Logic::getInstance(){
    if (s_Logic == NULL) {
        s_Logic = new Logic();
    }
    return s_Logic;
}

void Logic::initMatchInfo(){
    HttpModel *hm = HttpModel::getInstance();
    std::map<string, string> postData;
    hm->addRequest("getMatches", "GET", postData, this, MYHTTP_SEL(Logic::initMatchOver), NULL);
    requestYet = true;
    
}

void Logic::initMatchOver(bool isSuc, string s, void *param) {
    initMatchYet = true;
    //rapidjson::Document d;
    d.Parse<0>(s.c_str());
    if (d["state"].GetInt() == 0) {
        return;
    }
    
    
    /*
    const rapidjson::Value&b = d["matches"];
    for(rapidjson::SizeType i=0; i < b.Size(); i++) {
        const rapidjson::Value &c = b[i];
        
    }
    */
}