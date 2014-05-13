//
//  RedisInterface.h
//  HelloCpp
//
//  Created by dozengame on 14-5-12.
//
//

#ifndef HelloCpp_RedisInterface_h
#define HelloCpp_RedisInterface_h
#include "async.h"
#include "string"
void *connect();

void *startRecord();
void *stopRecord();
void startPlay();
void startSend(const char*);
const char *getFileName();



void *createRedis();
void connectRedis(void *);
void runSubscribe(void *);
bool readSubData(void *, std::string *, std::string *);
//limit data Size
//void receiveData(const char*, unsigned int length);
//void conCallback(redisAsyncContext *, void *);

#endif
