//
//  ChatView.h
//  TestCpp
//
//  Created by dozengame on 14-5-15.
//
//

#ifndef __TestCpp__ChatView__
#define __TestCpp__ChatView__

#include <iostream>
#include "cocos-ext.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace extension;
using namespace ui;
class ChatView : public CCLayer, public CCIMEDelegate {
public:
    virtual bool init();
    virtual void update(float);
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ChatView);
    
    //获取键盘相关信息
    //
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info)   {
        //CC_UNUSED_PARAM(info);
        CCLog("begin info %f %f %f %f", info.begin.origin.x, info.begin.origin.y, info.begin.size.width, info.begin.size.height);
        CCLog("end info %f %f %f %f", info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);
        CCLog("dura info %f", info.duration);
        ksize = info.end.size;
        setSizeYet = true;
        
    }
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
    
    ~ChatView();
private:
    Widget *w;
    void onText(CCObject *, TextFiledEventType);
    void onSend(CCObject*, TouchEventType);
    
    UIPanel *oneWord;
    //UIButton *content;
    UIPanel *twoWord;
    UILabel *lab2;
    ImageView *head2;
    
    
    UIPanel *bottom;
    CCSize ksize;
    bool setSizeYet;
    TextField *tf;
    ListView *lv;
    
    UILabel *lab;
    ImageView *head;
    
    CCLabelTTF *testLabel;
    
};
#endif /* defined(__TestCpp__ChatView__) */
