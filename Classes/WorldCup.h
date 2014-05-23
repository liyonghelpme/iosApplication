//
//  WorldCup.h
//  TestCpp
//
//  Created by dozengame on 14-5-14.
//
//

#ifndef __TestCpp__WorldCup__
#define __TestCpp__WorldCup__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;
using namespace ui;

class WorldCup : public CCLayer{
public:
    virtual bool init();
    virtual void update(float);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    CREATE_FUNC(WorldCup);
    
    ~WorldCup();
    
private:
    void onChat(CCObject*, TouchEventType);
    void onConf(CCObject*, TouchEventType);
    void onFri(CCObject*, TouchEventType);
    void onListview(CCObject*, ListViewEventType);
    void onScroll(CCObject*, ScrollviewEventType);
    void onRefresh(bool isSuc, std::string s, void* p);
    
    void showBasic();
    void showScroll();
    
    
    void refreshOnlineNum(float);
    
    float lastRefreshTime;
    bool inRefresh;
    int lastUpdateIndex;
    
    
    bool showYet;
    cocos2d::ui::Widget *w;
    
    ListView *lv;
    Label *dlab;
    UIWidget *pc, *cp;
    
    bool inUpdateData;
    int oldOrNew;
    
    CCDictionary *dict;
};
#endif /* defined(__TestCpp__WorldCup__) */
