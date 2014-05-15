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
class WorldCup : public CCLayer{
public:
    virtual bool init();
    virtual void update(float);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    CREATE_FUNC(WorldCup);
private:
    bool showYet;
    cocos2d::ui::Widget *w;
    
};
#endif /* defined(__TestCpp__WorldCup__) */
