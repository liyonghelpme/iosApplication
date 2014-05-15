//
//  TestRedis.m
//  HelloCpp
//
//  Created by dozengame on 14-5-12.
//
//

#import "TestRedis.h"
#include "stdio.h"
void *receive = NULL;
void startReceiveRedis(){
    receive = createRedis();
    connectRedis(receive);
    runSubscribe(receive);
}

@implementation TestRedis
+(id)sharedRedis{
    static TestRedis *sh = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sh = [[self alloc] init];
        
    });
    return sh;
}
-(id)init{
    //connectYet = false;
}

void *createRedis() {
    NSLog(@"create Redis");
    TestRedis *cd = [[TestRedis alloc] init];
    return cd;
}
void connectRedis(void *cd) {
    NSLog(@"connect Redis");
    
    //TestRedis *tr = (TestRedis *)cd;
    //[tr connect];
}
//使用nonblock socket 来接收数据即可
void receiveData(const char *data, unsigned int length){
    
}

/*
-(void)connectAsync{
    context = redisAsyncConnect("127.0.0.1", 6379);
    if(context->err) {
        printf("Error %s\n", context->errstr);
    }
    //redisAsyncSetConnectCallback(context, (redisConnectCallback*)conCallback);
    
}
*/
void runSubscribe(void *tc){
    NSLog(@"run subscribe");
    TestRedis *tr = (TestRedis *)tc;
    [tr createThread];
}
-(void)createThread{
    NSThread *myThread = [[NSThread alloc] initWithTarget:self selector:@selector(myThreadMainMethod:) object:nil];
    [myThread start];
    
}
-(void)myThreadMainMethod:(id)obj{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSLog(@"run thread");
    //Warning connect must in thread itself
    chatInfo = [[NSMutableArray alloc] init];
    [self connect];
    [redis command:@"subscribe chat"];
    while (true) {
        id retVal = [redis getReply];
        NSLog(@"replay is");
        NSLog([NSString stringWithFormat:@"%@", retVal]);
        
        @synchronized(chatInfo){
            [chatInfo addObject:retVal];
        }
        NSLog(@"finish reply");
        
    }
    [chatInfo release];
    [pool release];
}
bool readSubData(void *c, std::string* cha, std::string *con){
    TestRedis *tr = (TestRedis*)c;
    const char*ca, *cn;
    BOOL has = [tr readData:&ca c:&cn];
    if (has) {
        cha->assign(ca);
        con->assign(cn);
    }
    if (has == YES) {
        return true;
    }else
        return false;
    
}
-(BOOL) readData:(const char**)cha c:(const char**)con{
    id msg = nil;
    @synchronized(chatInfo){
        if ([chatInfo count] > 0) {
            msg = [[chatInfo objectAtIndex:0] retain];
            [chatInfo removeObjectAtIndex:0];
        }
    }
    if(msg != nil) {
        NSString *channel = [msg objectAtIndex:1];
        NSString *content = [msg objectAtIndex:2];
        *cha = [channel UTF8String];
        *con = [content UTF8String];
        [msg release];
        return YES;
    }
    return NO;
}

//当未连接的时候连接服务器
//当连接失败 返回NULL
void *connect(){
    TestRedis *tr = [TestRedis sharedRedis];
    if (tr->redis != nil) {
        return tr;
    }
    [tr connect];
    if (tr->redis == nil) {
        return NULL;
    }
    return tr;
}


//阻塞连接 会block掉整个程序 需要使用 Aynchronized 来做连接 或者 采用多线程 来做连接
-(void) connect{
    redis = [ObjCHiredis redis:@"127.0.0.1" on:[NSNumber numberWithInt:6379]];
    
    /*
    id key = [redis command:@"set testKey 123"];
    [redis command:@"subscribe chat"];
    id retVal = [redis getReply];
    NSLog(@"testRedis" );
    NSLog([NSString stringWithFormat:@"%@", key]);
    NSLog([NSString stringWithFormat:@"%@", retVal]);
    
    
    key = [redis command:@"get testKey"];
    retVal = [redis getReply];
    NSLog(@"key is");
    NSLog([NSString stringWithFormat:@"%@", key]);
    NSLog([NSString stringWithFormat:@"%@", retVal]);
    */
}
-(void)sendVoice:(const char *)fn{
    NSLog(@"send voice file");
    //NSLog([NSString stringWithFormat:@"%@", url ]);
    //const char *fn = [url fileSystemRepresentation];
    NSLog([NSString stringWithFormat:@"filename %s", fn ]);
    //FILE *f = fopen(fn, "r");
    //fread(<#void *#>, <#size_t#>, <#size_t#>, <#FILE *#>)
    
    NSFileManager *fmr = [NSFileManager defaultManager];
    NSData *db = [fmr contentsAtPath:[NSString stringWithFormat:@"%s", fn]];
    //data and length different
    //const void* data = db.bytes;
    //NSInteger len = db.length;
    NSString *b64 = db.base64Encoding;
    
    NSString *cmd = [NSString stringWithFormat:@"publish chat %@", b64];
    NSLog(@"send cmd");
    //NSLog(cmd);
    NSLog([NSString stringWithFormat:@"%d cmd %d", (int)b64.length, (int)cmd.length]);
    id retVal = [redis command:cmd];
    //id retVal = [redis getReply];
    NSLog([NSString stringWithFormat:@"retval %@", retVal]);
    NSLog(@"finish read");
}

void startSend(const char *fn){
    id tr = [TestRedis sharedRedis];
    [tr sendVoice:fn];
}

void sendText(std::string text){
    id tr = [TestRedis sharedRedis];
    [tr redisSendText:text.c_str()];
}

//发送命令也要一个异步的线程来处理才行
-(void)redisSendText:(const char *)text{
    NSString *cmd = [NSString stringWithFormat:@"publish chat %s", text];
    id retVal = [redis command:cmd];
    NSLog([NSString stringWithFormat:@"retval %@", retVal]);
}

@end
