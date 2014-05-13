//
//  TestVoice.h
//  HelloCpp
//
//  Created by dozengame on 14-5-13.
//
//

#import <Foundation/Foundation.h>
#include "RedisInterface.h"
#import <AVFoundation/AVFoundation.h>
@interface TestVoice : NSObject <AVAudioRecorderDelegate, AVAudioPlayerDelegate> {
    AVAudioRecorder *recorder;
    AVAudioPlayer *player;
}
-(void)test;
-(void)pause;
-(void)stop;
-(void)play;
@end
