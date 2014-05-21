//
//  ServiceCenter.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#include "ServiceCenter.h"
static ServiceCenter *s_ser = NULL;
ServiceCenter *ServiceCenter::getInstance(){
    if (s_ser == NULL) {
        s_ser = new ServiceCenter();
    }
    return s_ser;
}