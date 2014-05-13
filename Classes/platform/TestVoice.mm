//
//  TestVoice.m
//  HelloCpp
//
//  Created by dozengame on 14-5-13.
//
//

#import "TestVoice.h"

@implementation TestVoice
-(void) test{
    NSArray *pathComponents = [NSArray arrayWithObjects:
                               [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject],
                               @"MyAudioMemo.m4a",
                               nil];
    NSURL *outputFileURL = [NSURL fileURLWithPathComponents:pathComponents];
    AVAudioSession *session = [AVAudioSession sharedInstance];
    NSError *error;
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
    NSMutableDictionary *recordSetting = [[NSMutableDictionary alloc] init];
    [recordSetting setValue:[NSNumber numberWithInt:kAudioFormatMPEG4AAC] forKey:AVFormatIDKey];
    [recordSetting setValue:[NSNumber numberWithFloat:44100.0] forKey:AVSampleRateKey];
    [recordSetting setValue:[NSNumber numberWithInt: 2] forKey:AVNumberOfChannelsKey];
 
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
    
}
TestVoice *tv;
void *startRecord(){
    tv = [[TestVoice alloc] init];
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
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Done"
                                                        message: @"Finish playing the recording!"
                                                       delegate: nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
    
}
const char* getFileName(){
    NSURL *url = tv->recorder.url;
    const char *fn = url.fileSystemRepresentation;
    return  fn;
}
@end
