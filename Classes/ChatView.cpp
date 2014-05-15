//
//  ChatView.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-15.
//
//

#include "ChatView.h"
#include "TestRedis.h"
CCScene *ChatView::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ChatView *layer = ChatView::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
ChatView::~ChatView(){
    testLabel->release();
}

bool ChatView::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
    
    setSizeYet = false;
    testLabel = CCLabelTTF::create("", "", 30);
    testLabel->retain();
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    UILayer *lay = UILayer::create();
    addChild(lay);
    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_3.json");
    lay->addWidget(w);
    w->setSize(size);
    
    tf = static_cast<UITextField*>(UIHelper::seekWidgetByName(w, "word"));
    CCSize bs = tf->getSize();
    tf->ignoreContentAdaptWithSize(false);
    float nw = 400*fs.width/640;
    
    //输入框高度随着输入内容一起增长即可
    tf->setSize(CCSizeMake(nw, 400));
    
    tf->setTextHorizontalAlignment(kCCTextAlignmentLeft);
    tf->setTextVerticalAlignment(kCCVerticalTextAlignmentBottom);
    tf->setAnchorPoint(ccp(0, 0));
    tf->setSizeType(SIZE_ABSOLUTE);
    
    tf->addEventListenerTextField(this, textfieldeventselector(ChatView::onText));
    
    bottom = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "bottom"));
    UIButton *add = static_cast<UIButton*>(UIHelper::seekWidgetByName(bottom, "add"));
    add->setEnabled(false);
    
    UIButton *send = static_cast<UIButton*>(UIHelper::seekWidgetByName(bottom, "send"));
    send->addTouchEventListener(this, toucheventselector(ChatView::onSend));
    
    //panel 下面还可以增加panel么?
    oneWord = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "oneWord"));
    oneWord->setEnabled(false);
    
    lab = static_cast<UILabel*>(UIHelper::seekWidgetByName(oneWord, "userDialog"));
    
    head = static_cast<ImageView*>(UIHelper::seekWidgetByName(oneWord, "head"));
    
    float lwid = fs.width-114-10-20;
    
    CCLog("label max widht %f", lwid);
    //lab->setSizeType(SIZE_ABSOLUTE);
    //lab->setTextAreaSize(CCSizeMake(100, fs.height));
    lab->ignoreContentAdaptWithSize(false);
    
    twoWord = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "twoWord"));
    twoWord->setEnabled(false);
    
    lab2 = static_cast<UILabel*>(UIHelper::seekWidgetByName(twoWord, "userDialog2"));
    lab2->ignoreContentAdaptWithSize(false);
    
    head2 = static_cast<ImageView*>(UIHelper::seekWidgetByName(twoWord, "head2"));
    
    
    //lab->setSize(CCSizeMake(lwid, 768));
    
    
    //lab->setTextAreaSize(CCSizeMake(lwid, 200));
                         
    /*
    UILabel *testLabe = UILabel::create();
    testLabe->setTextAreaSize(CCSizeMake(100,  768));
    testLabe->setAnchorPoint(ccp(0, 1));
    testLabe->setText("what fuck size is it why so big no wrap");
    testLabe->setFontSize(30);
    testLabe->ignoreContentAdaptWithSize(false);
    testLabe->setSize(CCSizeMake(100, 100));
    
    
    UITextField *tw = UITextField::create();
    tw->ignoreContentAdaptWithSize(false);
    tw->setSize(CCSizeMake(100, 100));
    tw->setTextHorizontalAlignment(kCCTextAlignmentLeft);
    tw->setTextVerticalAlignment(kCCVerticalTextAlignmentTop);
    tw->setPlaceHolder("welcome to china");
    tw->setText("come on why so long long long long long long long long ");
    tw->setFontSize(30);
    tw->setPosition(ccp(300 , 600));
    tw->setAnchorPoint(ccp(0.5, 0.5));
    //oneWord->addChild(tw);
    lay->addWidget(tw);
    
    UITextField* textField = UITextField::create();
    textField->ignoreContentAdaptWithSize(false);
    textField->setSize(CCSizeMake(240, 160));
    textField->setTextHorizontalAlignment(kCCTextAlignmentCenter);
    textField->setTextVerticalAlignment(kCCVerticalTextAlignmentCenter);
    textField->setTouchEnabled(true);
    textField->setFontName("Marker Felt");
    textField->setFontSize(30);
    textField->setPlaceHolder("input words here");
    textField->setPosition(ccp(300, 300));
    //textField->setPosition(ccp(widgetSize.width / 2.0f, widgetSize.height / 2.0f));
    //textField->addEventListenerTextField(this, textfieldeventselector(UITextFieldTest_LineWrap::textFieldEvent));
    //m_pUiLayer->addWidget(textField);
    
    lay->addWidget(textField);
    oneWord->addChild(testLabe);
    */
    
    
    /*
    //未知的bug 导致lab设置老出错 用button替换掉  因为另外一个 TextField 也叫做 word 导致重名产生问题了 在上面的 tf = 赋值的时候 出错了
    lab = UILabel::create();
    lab->setText("test Word");
    
    
    UIButton *word = static_cast<UIButton*>(UIHelper::seekWidgetByName(oneWord, "userDialog"));
    lab->setPosition(word->getPosition());
    lab->setAnchorPoint(word->getAnchorPoint());
    lab->setTextHorizontalAlignment(kCCTextAlignmentLeft);
    lab->setTextVerticalAlignment(kCCVerticalTextAlignmentTop);
    lab->setFontSize(word->getTitleFontSize());
    lab->setFontName(word->getTitleFontName());
    
    word->setEnabled(false);
    word->removeFromParent();
    */
    
    
    
    //oneWord->addChild(lab);
    
    
    
    //content = static_cast<UIButton*>(UIHelper::seekWidgetByName(oneWord, "content"));
    
    //word = static_cast<UILabel*>(UIHelper::seekWidgetByName(oneWord, "word"));
    //word->setEnabled(false);
    
    CCLog("initial listView");
    
    lv = static_cast<ListView*>(UIHelper::seekWidgetByName(w, "listView"));
    
    
    scheduleUpdate();
    return true;
}
void ChatView::onSend(cocos2d::CCObject *obj, TouchEventType tt){
    switch (tt) {
        case cocos2d::ui::TOUCH_EVENT_BEGAN:
        {
            
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_MOVED:
        {
            
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_ENDED:
        {
            CCLog("send Msg");
            std::string text = tf->getStringValue();
            tf->setText("");
            //content->setTitleText(text);
            //word->setText(text);
            
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            float lwid = fs.width-114-10-30;
            //调整尺寸
            /*
            CCLabelTTF *lt = static_cast<CCLabelTTF*>(lab->getVirtualRenderer());
            lt->setDimensions(CCSizeMake(lwid, 0));
            CCSize tsz = lt->getContentSize();
            */
            /*
            testLabel->setString(text.c_str());
            testLabel->setDimensions(CCSize(lwid, 0));
            CCSize tsz = testLabel->getContentSize();
            //only when lab text 长度 > 0
            CCLog("testLabel size %f %f", tsz.width, tsz.height);
            lab->setText("");
            lab->setSize(tsz);
            lab->setText(text);
            
            */
            testLabel->setString(text.c_str());
            //first test width
            testLabel->setDimensions(CCSize(0, 0));
            CCSize osz = testLabel->getContentSize();
            //CCSize tsz;
            if(osz.width > lwid){
                testLabel->setDimensions(CCSize(lwid, 0));
            }
            
            //宽度存在一个bug 需要+1 才能显示完整的文字 高度也可能存在这个浮点数bug
            CCSize tsz = testLabel->getContentSize();
            tsz.width++;
            tsz.height++;
            
            lab2->setText("");
            lab2->setSize(tsz);
            //CCLog("setText %s", text.c_str());
            lab2->setText(text);
            
            CCLog("size is what %f %f %f %f",osz.width, osz.height, tsz.width, tsz.height);
            
            //CCLog("label max widht %f", lwid);
            //lab->setSizeType(SIZE_ABSOLUTE);
            //lab->setTextAreaSize(CCSizeMake(100, fs.height));
            //lab->ignoreContentAdaptWithSize(false);
            //lab->setSize(CCSizeMake(lwid, 200));
            //自动计算高度
            //lab->setTextAreaSize(CCSizeMake(lwid, 0));
            
            
            //CCSize csz = lab->getContentSize();
            
            //CCLog("contentSize %f %f", csz.width, csz.height);
            
            //lab->setTextAreaSize(CCSizeMake(100, 768));
            //如何得到实际的文本高度呢？不仅仅是Size得高度
            
            //CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            CCSize ws = lab->getSize();
            CCSize hsz = head->getSize();
            float height = std::max(ws.height, hsz.height);
            height += 20;
            
            UIPanel *pan = static_cast<UIPanel*>(twoWord->clone());
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            
            /*
            UIPanel *pan = static_cast<UIPanel*>(oneWord->clone());
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            */
            
            lv->pushBackCustomItem(pan);
            
            //保持指针传递或者传递string更安全
            connect();
            //startSend(text.c_str());
            sendText(text);
            
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_CANCELED:
        {
            
        }
            break;
        default:
            break;
    }
}
void ChatView::update(float diff){
    if (receive == NULL) {
        startReceiveRedis();
    }else {
        std::string channel;
        std::string content;
        bool res = readSubData(receive, &channel, &content);
        if(res){
            CCLog("read data %s %s", channel.c_str(), content.c_str());
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            float lwid = fs.width-114-10-30;
            
            testLabel->setString(content.c_str());
            //first test width
            testLabel->setDimensions(CCSize(0, 0));
            CCSize osz = testLabel->getContentSize();
            //CCSize tsz;
            if(osz.width > lwid){
                testLabel->setDimensions(CCSize(lwid, 0));
            }
            
            CCSize tsz = testLabel->getContentSize();
            tsz.width++;
            tsz.height++;
            lab->setText("");
            lab->setSize(tsz);
            lab->setText(content);
            
            CCSize ws = lab->getSize();
            CCSize hsz = head->getSize();
            float height = std::max(ws.height, hsz.height);
            height += 20;
            
             UIPanel *pan = static_cast<UIPanel*>(oneWord->clone());
             pan->setEnabled(true);
             pan->setSize(CCSizeMake(fs.width, height));
             pan->setSizeType(SIZE_ABSOLUTE);
            
            lv->pushBackCustomItem(pan);
            
            
        }
    }
}

void ChatView::onText(cocos2d::CCObject *obj, TextFiledEventType tt) {
    switch (tt) {
        case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME:
        {
            CCLog("setSizeYet %d",setSizeYet);
            //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, screenSize.height/2)));
            if (setSizeYet) {
                bottom->runAction(CCMoveTo::create(0.225, ccp(0, ksize.height)));
            }
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            bottom->runAction(CCMoveTo::create(0.225, ccp(0, 0)));
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT:
            break;
        default:
            break;
    }
}