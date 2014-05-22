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
        
        pinyinMove();
    }
    
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
    
    ~ChatView();
private:
    Widget *w;
    void onText(CCObject *, TextFiledEventType);
    void onSend(CCObject*, TouchEventType);
    void onSpeak(CCObject*, TouchEventType);
    void onSay(CCObject*   , TouchEventType);
    void onOtherVoice(CCObject *, TouchEventType);
    void onChatInfo(CCObject*, TouchEventType);
    void onAdd(CCObject*, TouchEventType);
    void onEnterChat(bool isSuc, std::string s, void *);
    
    //出现拼音输入法的时候键盘还得再移动一下
    void pinyinMove();
    

    void updateEnter(float);
    float lastUpdateTime;
    
    
    //listView 的两个事件不是同时可用的 有项目的时候这个可用
    void onBackground(CCObject*, ListViewEventType);
    //没有项目的时候 该事件可用
    void onBackgroundTouch(CCObject*, TouchEventType);
    
    void onVoice(CCObject*, TouchEventType);
    void onMsg(bool isSuc, std::string s, void *param);
    void sendImg();
    
    void adjustBut();
    
    void receiveMsg();
    
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
    UIButton *speak;
    //ImageView *voice2;
    
    Button *sayWord;
    
    bool inVoice;
    
    Layout *myvoice;
    //ImageView *myvImg;
    Button *myvImg;
    
    //int vid;
    
    Layout *otherVoice;
    UIImageView *ohead;
    Button *ovoice;
    Button *backbnt;
    
    Layout *inRecord;
    Label *curTime;

    void onBack(CCObject*, TouchEventType);
    
    bool showKeyboard;
    
    
    UIButton *add, *send;
    
    bool selectImgYet;

    
    bool enterChatYet;
    
    int state;
    //int chatImgId;
    
    //float getInputWidth();
};
#endif /* defined(__TestCpp__ChatView__) */
