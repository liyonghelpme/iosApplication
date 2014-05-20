//
//  ImgSelect.m
//  TestCpp
//
//  Created by dozengame on 14-5-19.
//
//

#import "ImgSelect.h"
#import "EAGLView.h"

@implementation ImgSelect
+(id)sharedImgSelect{
    static ImgSelect *sh = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sh = [[self alloc] init];
        
    });
    return sh;
}
-(void)dealloc{
    if (imgdata != NULL) {
        free(imgdata);
        imgdata = NULL;
    }
    [super dealloc];
    
}
-(id)init{
    self = [super init];
    if (self) {
        self->imgdata = NULL;
        self->len = 0;
        self->getYet = false;
        
        NSLog(@"init imgdata");
    }
    
    return self;
}
void openImageSelect(){
    ImgSelect *ig = [ImgSelect sharedImgSelect];
    [ig showImgSelect];
}
-(void) showImgSelect {
    getYet = false;
    UIImagePickerController *ipc = [[UIImagePickerController alloc] init];
    ipc.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    ipc.delegate = self;
    
    [[EAGLView sharedEGLView] addSubview:ipc.view];
    [ipc viewWillAppear:YES];
    [ipc viewDidAppear:YES];
    
    
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker{
    NSLog(@"cancel");
    [picker.view removeFromSuperview];
}
-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    [picker.view removeFromSuperview];
    NSLog(@"select info %@", info);
    UIImage *img = [info objectForKey:@"UIImagePickerControllerOriginalImage"];
    NSData *data = UIImagePNGRepresentation(img);
    //NSString *b64 = data.base64Encoding;
    //[b64 UTF8String]
    NSLog(@"getImageData use data to initial Cocos2dx CCSprite then sendData to network");
    if (imgdata != NULL) {
        free(imgdata);
        imgdata = NULL;
    }
    imgdata = malloc(data.length*sizeof(char));
    memcpy(imgdata, data.bytes, data.length);
    len = data.length;
    NSLog([NSString stringWithFormat:@"image size %d", len]);
    getYet = true;
    NSLog(@"getYet");
}
void *getImage(int *length){
    ImgSelect *ig = [ImgSelect sharedImgSelect];
    NSLog([NSString stringWithFormat:@"getImage %x %d", ig->imgdata, ig->len]);
    if (ig->imgdata != NULL) {
        *length = ig->len;
        return ig->imgdata;
    }
    *length = 0;
    return NULL;
}

bool checkGetYet(){
    ImgSelect *ig = [ImgSelect sharedImgSelect];
    return ig->getYet;
}
@end
