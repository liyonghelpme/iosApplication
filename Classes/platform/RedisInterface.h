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
void sendText(std::string);
void sendImage();

const char *getFileName();



void *createRedis();
void connectRedis(void *);
void runSubscribe(void *);
bool readSubData(void *, std::string *, std::string *);
//limit data Size
//void receiveData(const char*, unsigned int length);
//void conCallback(redisAsyncContext *, void *);

extern void *receive;

void storeFile(const unsigned char*con, int len, int vid);
void playVoice(int vid);

void startReceiveRedis();
void openImageSelect();

void *getImage(int *length);

bool checkGetYet();

int getMyRecordVid();
int getVoiceLength(const char *);


#endif
