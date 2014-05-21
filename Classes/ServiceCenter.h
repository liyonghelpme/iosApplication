//
//  ServiceCenter.h
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#ifndef __TestCpp__ServiceCenter__
#define __TestCpp__ServiceCenter__

#include <iostream>
#include "cocos2d.h"
class ServiceCenter : public cocos2d::CCObject{
public:
    static ServiceCenter* getInstance();
    
};

#endif /* defined(__TestCpp__ServiceCenter__) */
