//
//  TestRedis.m
//  HelloCpp
//
//  Created by dozengame on 14-5-12.
//
//

#import "TestRedis.h"
#include "stdio.h"
#include "document.h";
#include "stringbuffer.h"
#include "writer.h"
#include "Logic.h"
#import "ImgSelect.h"
#include "MyBase64.h"
#import <AVFoundation/AVFoundation.h>




void *receive = NULL;
void startReceiveRedis(){
    //释放上一个聊天室的数据 新建一个 新的 接收器
    if (receive != NULL) {
        [receive dealloc];
        receive = NULL;
    }
    
    receive = createRedis();
    NSLog([NSString stringWithFormat:@"receive is %@", receive ]);
    
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
    self = [super init];
    
    if (self != nil) {
        self->connectSuc = false;
        self->subchannelId = -1;
        self->channelName = [NSString stringWithString:@"match"];
    }
    
    return self;
}

void *createRedis() {
    NSLog(@"create Redis");
    id cd = [[TestRedis alloc] init];
    //NSLog([NSString stringWithFormat:@"TestRedis %@", cd]);
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
    //connect false
    if (redis == nil) {
        return;
    }
    //已经订阅了其它频道 首先 取消订阅接着 再订阅新的频道
    /*
    if (self->subchannelId != -1) {
        [redis command:@"unsubscribe"];
        self->subchannelId = -1;
    }
    */
    
    //重新启动新的订阅 关闭旧的订阅 接着 打开新的订阅频道
    self->subchannelId = Logic::getInstance()->getCID();
    //NSLog(@"su")
    NSString *cmd = [NSString stringWithFormat:@"subscribe %@_%d", self->channelName, self->subchannelId];
    [redis command:cmd];
    
    
    while (true) {
        id retVal = [redis getReply];
        //服务器关闭的消息是什么 格式的 retVal不是这样
        NSLog(@"replay is");
        if ([retVal class] == [NSArray class]) {
            NSLog(@"array");
        }else if([retVal class] == [NSNull class]) {
            NSLog(@"null retval");
        }else {
            NSLog([NSString stringWithFormat:@"%@", [retVal class]]);
        }
        //NSLog([NSString stringWithFormat:@"%@", retVal ]);
        
        //NSLog(typeof(retVal));
        //NSLog([NSString stringWithFormat:@"%@", retVal]);
        
        //连接断开了
        if (retVal == nil) {
            NSLog(@"和聊天服务器连接断开了");
        } else {
        
            @synchronized(chatInfo){
                [chatInfo addObject:retVal];
            }
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
    //NSLog([NSString stringWithFormat:@"readSubData %d", has]);
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
    NSString *h = [NSString stringWithCString:HttpModel::getInstance()->baseRedisHost.c_str()];
    redis = [ObjCHiredis redis:h on:[NSNumber numberWithInt:6379]];
    [redis retain];
    if (redis == nil) {
        NSLog([NSString stringWithFormat:@"redis connection fail %@", h]);
        self->connectSuc = false;
    }else {
        NSLog([NSString stringWithFormat:@"redis connection success %@", h]);
        self->connectSuc = true;
    }
    
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
    //connection Fail or connection lost
    if (redis == nil) {
        return;
    }
    
    int vl = getVoiceLength(fn);
    
    NSFileManager *fmr = [NSFileManager defaultManager];
    NSData *db = [fmr contentsAtPath:[NSString stringWithFormat:@"%s", fn]];
    //data and length different
    //const void* data = db.bytes;
    //NSInteger len = db.length;
    NSString *b64 = db.base64Encoding;
    rapidjson::Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType &allocator = d.GetAllocator();
    d.AddMember("type", "voice", allocator);
    d.AddMember("content", [b64 UTF8String], allocator);
    
    d.AddMember("sender", Logic::getInstance()->getSenderId(), allocator);
    d.AddMember("like_team", Logic::getInstance()->getFlagId(), allocator);
    d.AddMember("length", int(vl), allocator);
    
    
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    d.Accept(writer);
    
    int mid = Logic::getInstance()->getCID();
    
    NSString *cmd = [NSString stringWithFormat:@"publish %@_%d %s", self->channelName,  mid, strbuf.GetString()];
    NSLog(@"send cmd");
    //NSLog(cmd);
    NSLog([NSString stringWithFormat:@"%d cmd %d", (int)b64.length, (int)cmd.length]);
    id retVal = [redis command:cmd];
    //id retVal = [redis getReply];
    NSLog([NSString stringWithFormat:@"retval %@", retVal]);
    NSLog(@"finish read");
}


-(void) sendImg{
    if (redis == NULL) {
        return;
    }
    NSLog(@"send Img to Server");
    int len;
    void *data = getImage(&len);
    if (data != NULL) {
        //NSData *data = [NSData dataWithBytes:data length:len];
        //NSString *b64 = data.base64Encoding;
        int olen;
        char *odata = base64_encode((unsigned char*)data, (size_t)len, (size_t*)&olen);
        std::string rd = string(odata, olen);
        free(odata);
        
        //NSString *b64 = [data base64EncodedDataWithOptions:nil];
        
        rapidjson::Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType  &allocator = d.GetAllocator();
        d.AddMember("type", "image", allocator);
        d.AddMember("content", rd.c_str(), allocator);
        
        d.AddMember("sender", Logic::getInstance()->getSenderId(), allocator);
        d.AddMember("like_team", Logic::getInstance()->getFlagId(), allocator);
        
        
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        d.Accept(writer);
        
        int mid = Logic::getInstance()->getCID();
        NSString *cmd = [NSString stringWithFormat:@"publish %@_%d %s", self->channelName, mid, strbuf.GetString()];
        NSLog(@"send cmd");
        //NSLog(cmd);
        NSLog([NSString stringWithFormat:@"%d cmd %d", (int)rd.size(), (int)cmd.length]);
        id retVal = [redis command:cmd];
        //id retVal = [redis getReply];
        NSLog([NSString stringWithFormat:@"retval %@", retVal]);
        NSLog(@"finish read");
        
    }
}

void startSend(const char *fn){
    TestRedis *tr = [TestRedis sharedRedis];
    if (tr->redis == nil) {
        [tr connect];
    }
    [tr sendVoice:fn];
}

void sendImage(){
    TestRedis *tr = [TestRedis sharedRedis];
    if(tr->redis == nil) {
        [tr connect];
    }
    [tr sendImg];
}


void sendText(std::string text){
    TestRedis *tr = [TestRedis sharedRedis];
    if (tr->redis == nil) {
        [tr connect];
    }
    //rapidjson::Document d;
    
    [tr redisSendText:text.c_str()];
}

//发送命令也要一个异步的线程来处理才行
-(void)redisSendText:(const char *)text{
    if (redis == nil) {
        connectSuc = false;
        return;
    }
    //NSString *s = [NSString stringWithFormat:@""];
    Logic *log = Logic::getInstance();
    int mid = log->getCID();
    
    NSLog([NSString stringWithFormat:@"channel is %d  text ", mid]);
    NSString *cmd = [NSString stringWithFormat:@"publish %@_%d %s", self->channelName, mid, text];
    NSLog(cmd);
    //NSLog(cmd);
    //[redis retainCount];
    
    NSLog([NSString stringWithFormat:@"%d", [redis retainCount]]);
    id retVal = [redis command:cmd];
    NSLog([NSString stringWithFormat:@"retval %@", retVal]);
    
    NSLog(@"send Text suc");
}

@end
