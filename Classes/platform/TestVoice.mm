//
//  TestVoice.m
//  HelloCpp
//
//  Created by dozengame on 14-5-13.
//
//

#import "TestVoice.h"
#include "Logic.h"

@implementation TestVoice
+(id)sharedRecord{
    static TestVoice *sh = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sh = [[self alloc] init];
        
    });
    return sh;
}

void storeFile(const unsigned char*con, int len, int vid){
    NSArray *savePath = [NSArray arrayWithObjects:[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject], [NSString stringWithFormat:@"tempAudio%d.m4a", vid], nil] ;
    NSURL *outputFileURL = [NSURL fileURLWithPathComponents:savePath];
    const char*fn = [outputFileURL fileSystemRepresentation];
    NSLog([NSString stringWithFormat:@"filename %s", fn ]);
    FILE *f = fopen(fn, "wb");
    fwrite(con, 1, len, f);
    fclose(f);
    NSLog([NSString stringWithFormat:@"store File %s", outputFileURL.fileSystemRepresentation]);
    
}

int getMyRecordVid(){
    TestVoice *tv = [TestVoice sharedRecord];
    return tv->myVid;
}
int getVoiceLength(const char*fn) {
    return [[TestVoice sharedRecord] getLength:fn];
}

-(int)getLength:(const char *)fn{
    NSURL *fileURL = [NSURL fileURLWithFileSystemRepresentation:fn isDirectory:false relativeToURL:false];
    
    AVURLAsset *audioAsset = [AVURLAsset URLAssetWithURL:fileURL options:nil];
    NSLog([NSString stringWithFormat:@"%@", audioAsset]);
    CMTime audioDuration = audioAsset.duration;
    float audioT = CMTimeGetSeconds(audioDuration);
    NSLog(@"audio time %f", audioT);
    return audioT;
}

-(void) test{
    //保存自己的语音信息到本地数据存储中
    myVid = Logic::getInstance()->getVid();
    
    NSString *myVoiceName = [NSString stringWithFormat:@"tempAudio%d.m4a", myVid];
    
    NSArray *pathComponents = [NSArray arrayWithObjects:
                               [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject],
                               myVoiceName,
                               nil];
    NSURL *outputFileURL = [NSURL fileURLWithPathComponents:pathComponents];
    AVAudioSession *session = [AVAudioSession sharedInstance];
    NSError *error;
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
    NSMutableDictionary *recordSetting = [[NSMutableDictionary alloc] init];
    [recordSetting setValue:[NSNumber numberWithInt:kAudioFormatMPEG4AAC] forKey:AVFormatIDKey];
    [recordSetting setValue:[NSNumber numberWithFloat:44100.0] forKey:AVSampleRateKey];
    [recordSetting setValue:[NSNumber numberWithInt: 2] forKey:AVNumberOfChannelsKey];
    
    if (recorder != nil) {
        [recorder release];
        recorder = nil;
    }
    
    recorder = [[AVAudioRecorder alloc] initWithURL:outputFileURL settings:recordSetting error:NULL];
    recorder.delegate = self;
    recorder.meteringEnabled = YES;
    [recorder prepareToRecord];
    
    
    if(!recorder.recording){
        AVAudioSession *session = [AVAudioSession sharedInstance];
        [session setActive:YES error:nil];
        [recorder record];
        
    }else {
        
    }
}
-(void)pause{
    [recorder pause];
}

-(void)stop {
    [recorder stop];
    AVAudioSession *av = [AVAudioSession sharedInstance];
    [av setActive:NO error:nil];
    //[recorder release];
}

TestVoice *tv;
void *startRecord(){
    tv = [TestVoice sharedRecord];
    [tv test];
    return tv;
}
void *stopRecord(){
    //[tv pause];
    [tv stop];
    return  tv;
}
void startPlay(){
    [tv play];
}

void playVoice(int vid){
    NSLog([NSString stringWithFormat:@"playVoice %d", vid]);
    
    TestVoice *tvv = [TestVoice sharedRecord];
    [tvv playTempVoice: vid];
}
-(void)playTempVoice:(int)vid{
    if (player != nil) {
        [player release];
        player = nil;
    }
    NSArray *savePath = [NSArray arrayWithObjects:[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject], [NSString stringWithFormat:@"tempAudio%d.m4a", vid], nil] ;
    NSURL *outputFileURL = [NSURL fileURLWithPathComponents:savePath];
    NSLog([NSString stringWithFormat:@"out url %s", outputFileURL.fileSystemRepresentation]);
    
    player = [[AVAudioPlayer alloc] initWithContentsOfURL:outputFileURL error:nil];
    [player setDelegate:self];
    BOOL res = [player play];
    NSLog([NSString stringWithFormat:@"play ok %d", res]);
}


-(void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag{
    NSLog(@"finish record");
}

-(void)play{
    NSLog(@"play audio");
    NSLog([NSString stringWithFormat:@"%@", recorder.url]);
    player = [[AVAudioPlayer alloc] initWithContentsOfURL:recorder.url error:nil];
    [player setDelegate:self];
    [player play];
}
-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    /*
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Done"
                                                        message: @"Finish playing the recording!"
                                                       delegate: nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
    */
}
const char* getFileName(){
    NSURL *url = tv->recorder.url;
    const char *fn = url.fileSystemRepresentation;
    return  fn;
}
@end
