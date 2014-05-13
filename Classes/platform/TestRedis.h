//
//  TestRedis.h
//  HelloCpp
//
//  Created by dozengame on 14-5-12.
//
//

#import <Foundation/Foundation.h>
#include "RedisInterface.h"
#import "ObjCHiredis.h"
//#import "async.h"

@interface TestRedis : NSObject {
    ObjCHiredis *redis;
    
    //redisAsyncContext *context;
    NSMutableArray *chatInfo;
}
-(void) connect;
-(void) sendVoice:(const char *)fn;
+(id)sharedRedis;

//-(void) connectAsync;

-(void) createThread;

-(void) myThreadMainMethod:(id )obj;
-(BOOL) readData:(const char**)cha c:(const char**)con;

@end
