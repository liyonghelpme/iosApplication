//
//  ChatView.cpp
//  TestCpp
//
//  Created by dozengame on 14-5-15.
//
//

#include "ChatView.h"
#include "RedisInterface.h"
#include "stringbuffer.h"
#include "writer.h"
#include "base64.h"
#include "string.h"
#include "Logic.h"
#include "ChatInfo.h"


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
    //chatImgId = 1;
    showKeyboard = false;
    inVoice = false;
    //vid = 0;
    selectImgYet = false;
    
    
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
    
    //w->addTouchEventListener(this, toucheventselector(ChatView::onBack));
    //CCLog("back is touch %d %d", w->isTouchEnabled(), w->isEnabled());
    
    
    const rapidjson::Value* minf = Logic::getInstance()->matchInfo;
    
    Label *title = static_cast<Label*>(UIHelper::seekWidgetByName(w, "title"));
    if (!DEBUG) {
        string tname = (*minf)["name1"].GetString();
        tname += "VS";
        tname += (*minf)["name2"].GetString();
        title->setText(tname);
        
        Label *slab = static_cast<Label*>(UIHelper::seekWidgetByName(w, "score"));
        string sc = "比分 ";
        sc += (*minf)["score1"].GetInt();
        sc += ":";
        sc += (*minf)["score2"].GetInt();
        slab->setText(sc);
        
    }
    
    
    
    
    
    tf = static_cast<UITextField*>(UIHelper::seekWidgetByName(w, "word"));
    tf->setMulLine(true);
    
    CCSize bs = tf->getSize();
    tf->ignoreContentAdaptWithSize(false);
    float nw = 400*fs.width/640;
    
    
    
    //输入框高度随着输入内容一起增长即可
    tf->setSize(CCSizeMake(nw, 58));
    
    tf->setTextHorizontalAlignment(kCCTextAlignmentLeft);
    tf->setTextVerticalAlignment(kCCVerticalTextAlignmentBottom);
    tf->setAnchorPoint(ccp(0, 0));
    tf->setSizeType(SIZE_ABSOLUTE);
    
    tf->addEventListenerTextField(this, textfieldeventselector(ChatView::onText));
    
    bottom = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "bottom"));
    add = static_cast<UIButton*>(UIHelper::seekWidgetByName(bottom, "add"));
    //add->setEnabled(false);
    add->addTouchEventListener(this, toucheventselector(ChatView::onAdd));
    
    
    send = static_cast<UIButton*>(UIHelper::seekWidgetByName(bottom, "send"));
    send->addTouchEventListener(this, toucheventselector(ChatView::onSend));
    
    
    
    //对方说话的panel
    //panel 下面还可以增加panel么?
    oneWord = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "oneWord"));
    oneWord->setEnabled(false);
    lab = static_cast<UILabel*>(UIHelper::seekWidgetByName(oneWord, "userDialog"));
    lab->ignoreContentAdaptWithSize(false);
    head = static_cast<ImageView*>(UIHelper::seekWidgetByName(oneWord, "head"));
    
    
    //float lwid = fs.width-114-10-20;
    
    //CCLog("label max widht %f", lwid);
    //lab->setSizeType(SIZE_ABSOLUTE);
    //lab->setTextAreaSize(CCSizeMake(100, fs.height));
    
    
    twoWord = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "twoWord"));
    twoWord->setEnabled(false);
    lab2 = static_cast<UILabel*>(UIHelper::seekWidgetByName(twoWord, "userDialog2"));
    lab2->ignoreContentAdaptWithSize(false);
    head2 = static_cast<ImageView*>(UIHelper::seekWidgetByName(twoWord, "head2"));
    //voice2 = static_cast<ImageView*>(UIHelper::seekWidgetByName(twoWord, "voice2"));
    
    speak = static_cast<Button*>(UIHelper::seekWidgetByName(bottom, "speak"));
    speak->setEnabled(false);
    speak->addTouchEventListener(this, toucheventselector(ChatView::onSpeak));
    
    
    
    
    sayWord = static_cast<Button*>(UIHelper::seekWidgetByName(bottom, "Button_9"));
    sayWord->addTouchEventListener(this, toucheventselector(ChatView::onSay));
    myvoice = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "myvoice"));
    myvImg = static_cast<Button*>(UIHelper::seekWidgetByName(myvoice, "voice2"));
    myvoice->setEnabled(false);
    
    
    otherVoice = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "otherVoice"));
    ohead = static_cast<ImageView*>(UIHelper::seekWidgetByName(otherVoice, "ohead"));
    ovoice = static_cast<Button*>(UIHelper::seekWidgetByName(otherVoice, "ovoice"));
    otherVoice->setEnabled(false);
    ovoice->addTouchEventListener(this, toucheventselector(ChatView::onOtherVoice));
    
    Button *cinfo = static_cast<Button*>(UIHelper::seekWidgetByName(w, "Button_3"));
    cinfo->addTouchEventListener(this, toucheventselector(ChatView::onChatInfo));
    
    
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
    lv->addTouchEventListener(this, toucheventselector(ChatView::onBackgroundTouch));
    
    lv->addEventListenerListView(this, listvieweventselector(ChatView::onBackground));
    
    
    scheduleUpdate();
    return true;
}

//点击listView 则 关闭键盘 detachIME
void ChatView::onBackground(cocos2d::CCObject *obj, ListViewEventType tt){
    CCLog("background touch event %d", tt);
    switch (tt) {
        case cocos2d::ui::LISTVIEW_ONSELECTEDITEM_START:
        {
            CCLog("touch background");
            tf->closeIME();
        }
            break;
        case cocos2d::ui::LISTVIEW_ONSELECTEDITEM_END:
        {
            
        }
            break;
        default:
            break;
    }
}
void ChatView::onBackgroundTouch(cocos2d::CCObject *obj, TouchEventType tt){
    switch (tt) {
        case cocos2d::ui::TOUCH_EVENT_BEGAN:
        {
            tf->closeIME();
        }
            break;
        default:
            break;
    }
}

void ChatView::onAdd(cocos2d::CCObject *obj, TouchEventType tt){
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
            //打开了选择图片页面
            selectImgYet = true;
            openImageSelect();
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

void ChatView::onChatInfo(CCObject *obj, TouchEventType tt){
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
            CCDirector::sharedDirector()->pushScene(ChatInfo::scene());
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

void ChatView::onOtherVoice(cocos2d::CCObject *obj,  TouchEventType tt){
    CCLog("on Other Voice");
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
            
            CCNode *n = static_cast<CCNode*>(obj);
            int vd = n->getTag();
            playVoice(vd);
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
void ChatView::onSay(cocos2d::CCObject *obj, TouchEventType tt){
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
            if (!inVoice) {
                inVoice = true;
                sayWord->setTitleText("Word");
                tf->setEnabled(false);
                speak->setEnabled(true);
            }else{
                inVoice = false;
                sayWord->setTitleText("Voice");
                tf->setEnabled(true);
                speak->setEnabled(false);
            }
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
void ChatView::onSpeak(cocos2d::CCObject *obj, TouchEventType tt){
    switch (tt) {
        case cocos2d::ui::TOUCH_EVENT_BEGAN:
        {
            startRecord();
            speak->setTitleText("松开结束");
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_MOVED:
        {
            
        }
            break;
        case cocos2d::ui::TOUCH_EVENT_ENDED:
        {
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            
            CCSize ws = myvImg->getSize();
            CCSize hsz = head2->getSize();
            float height = std::max(ws.height, hsz.height);
            height += 20;
            //voice2->setEnabled(true);
            //lab2->setEnabled(false);
            
            
            UIPanel *pan = static_cast<UIPanel*>(myvoice->clone());
            Button *voice = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "voice2"));
            //声音文件编号 当前录制的音频文件编号
            voice->setTag(getMyRecordVid());
            voice->addTouchEventListener(this, toucheventselector(ChatView::onVoice));
            
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            pan->setVisible(true);
            lv->pushBackCustomItem(pan);
            
            
            stopRecord();
            const char *fn = getFileName();
            CCLog("file name %s", fn);
            //connect redis server
            //connect();
            //send data to server
            //发送语音文件 本地显示相关的语音文件关联
            startSend(fn);
            speak->setTitleText("按住说话");
            
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
//播放自己的声音
void ChatView::onVoice(cocos2d::CCObject *obj, TouchEventType tt){
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
            CCNode *n = static_cast<CCNode*>(obj);
            int vd = n->getTag();
            playVoice(vd);
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
            CCSize ws = lab2->getSize();
            CCSize hsz = head2->getSize();
            float height = std::max(ws.height, hsz.height);
            height += 20;
            
            //voice2->setEnabled(false);
            //lab2->setEnabled(true);
            
            UIPanel *pan = static_cast<UIPanel*>(twoWord->clone());
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            pan->setVisible(true);
            /*
            UIPanel *pan = static_cast<UIPanel*>(oneWord->clone());
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            */
            
            lv->pushBackCustomItem(pan);
            
            //保持指针传递或者传递string更安全
            //connect();
            //startSend(text.c_str());
            
            rapidjson::Document d;
            d.SetObject();
            
            rapidjson::Document::AllocatorType &allocator = d.GetAllocator();
            d.AddMember("type", "text", allocator);
            d.AddMember("content", text.c_str(), allocator);
            d.AddMember("sender", Logic::getInstance()->nickname.c_str(), allocator);
            
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            d.Accept(writer);
            
            sendText(strbuf.GetString());
            
            adjustBut();
            
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
    
    //打开过图片选择 并且 确认 获取图片 数据了
    if (selectImgYet && checkGetYet()) {
        selectImgYet = false;
        int len;
        void *data = getImage(&len);
        CCLog("select Img %x %d", data, len);
        //获取了本地图像数据
        if (data != NULL) {
            //发送出去显示为一张对话的图片
            
            char name[128];
            sprintf(name, "tmp_%d", Logic::getInstance()->getImgId());
            const char *key = name;
            
            CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key);
            CCImage *pImage = new CCImage();
            pImage->initWithImageData(data, len, CCImage::kFmtPng);
            
            pTexture = CCTextureCache::sharedTextureCache()->addUIImage(pImage, key);
            CC_SAFE_RELEASE(pImage);
            
            
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            //图片宽度需要进行缩放到100*100 的尺寸
            CCSize ws = CCSizeMake(100, 100);
            CCSize hsz = head->getSize();
            float height = std::max(ws.height, hsz.height);
            height += 20;
            
            
            UIPanel *pan = static_cast<UIPanel*>(myvoice->clone());
            Button *vimg = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "voice2"));
            CCLog("vimg name %s", vimg->getName());
            vimg->loadTextureNormal(key, UI_TEX_TYPE_LOCAL);
            vimg->setSize(ws);
            vimg->setSizeType(SIZE_ABSOLUTE);
            //使用customSize 而不是图片自动size
            vimg->ignoreContentAdaptWithSize(false);
            
            
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            pan->setVisible(true);
            
            lv->pushBackCustomItem(pan);
            
            //发送图像文件
            sendImage();
            //head->loadTextureNormal(key, UI_TEX_TYPE_LOCAL);
            
        }
    }
    
    if (receive == NULL) {
        startReceiveRedis();
        CCLog("receive %x", receive);
    }else {
        std::string channel;
        std::string content;
        bool res = readSubData(receive, &channel, &content);
        if(res){
            CCLog("read data %s %d", channel.c_str(), content.size());
            
            rapidjson::Document d;
            d.Parse<0>(content.c_str());
            std::string tx = "text";
            std::string ctype = d["type"].GetString();
            
            std::string conText;
            bool isText = false;
            
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            float lwid = fs.width-114-10-30;
            string sender = d["sender"].GetString();
            //自己发送的不要显示
            //显示发送者的头像数据 根据Id 获得相关头像
            //header Cache
            //调试关闭
            /*
            if (sender.compare(Logic::getInstance()->nickname) == 0) {
                return;
            }
             */
            
            //后台只publish 一个索引信息 从服务器拉去 实际的语音讯息
            if (ctype.compare("text") == 0 ){
                conText = d["content"].GetString();
                isText = true;
                
                testLabel->setString(conText.c_str());
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
                lab->setText(conText);
                
                CCSize ws = lab->getSize();
                CCSize hsz = head->getSize();
                float height = std::max(ws.height, hsz.height);
                height += 20;
                
                UIPanel *pan = static_cast<UIPanel*>(oneWord->clone());
                pan->setEnabled(true);
                pan->setSize(CCSizeMake(fs.width, height));
                pan->setSizeType(SIZE_ABSOLUTE);
                pan->setVisible(true);
                
                CCLog("push CutonItem where");
                lv->pushBackCustomItem(pan);
            } else if(ctype.compare("voice") == 0){
                //decode base64
                unsigned char *voice = (unsigned char*)d["content"].GetString();
                unsigned char *out;
                int outLen = cocos2d::base64Decode(voice, strlen((const char*)voice), &out);
                int vid = Logic::getInstance()->getVid();
                
                storeFile(out, outLen, vid);
                free(out);
                
                CCSize ws = ovoice->getSize();
                CCSize hsz = ohead->getSize();
                float height = std::max(ws.height, hsz.height);
                height += 20;
                
                //ovoice->setTag(vid);
                //vid++;
                
                UIPanel *pan = static_cast<UIPanel*>(otherVoice->clone());
                pan->setEnabled(true);
                pan->setSize(CCSizeMake(fs.width, height));
                pan->setSizeType(SIZE_ABSOLUTE);
                pan->setVisible(true);
                Button *newVoice = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "ovoice"));
                newVoice->addTouchEventListener(this, toucheventselector(ChatView::onOtherVoice));
                newVoice->setTag(vid);
                //vid++;
                
                lv->pushBackCustomItem(pan);
                
            }else if(ctype.compare("image") == 0) {
                
                unsigned char *image = (unsigned char*)d["content"].GetString();
                unsigned char *out;
                int outLen = cocos2d::base64Decode(image, strlen((const char*)image), &out);
                
                /*
                bool compareData = true;
                int llen;
                unsigned char *ldata = (unsigned char*)getImage(&llen);
                //decode 的数据是错误的
                if (ldata != NULL) {
                    CCLog("data lenght %d %d", llen, outLen);
                    //长度相同但是 解压缩后的数据不同为什么呢？
                    for (int i = 0 ; i < 100 && i < llen; i++) {
                        CCLog("%x %c %x %c", (unsigned char)ldata[i], ldata[i], (unsigned char)out[i], out[i]);
                    }
                    for (int i = 0; i < llen; i++) {
                        if (ldata[i] != out[i]) {
                            compareData = false;
                            break;
                        }
                    }
                    if (!compareData) {
                        CCLog("data different!!");
                    }
                    
                }
                */
                
                char name[128];
                int imgId = Logic::getInstance()->getImgId();
                sprintf(name, "tmp_%d", imgId);
                const char*key = name;
                
                CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key);
                CCImage *pImage = new CCImage();
                pImage->initWithImageData(out, outLen, CCImage::kFmtPng);
                
                pTexture = CCTextureCache::sharedTextureCache()->addUIImage(pImage, key);
                CC_SAFE_RELEASE(pImage);
                
                //int vid = Logic::getInstance()->getVid();
                //storeFile(out, outLen, vid);
                free(out);
                
                CCSize ws = CCSizeMake(100, 100);
                CCSize hsz = ohead->getSize();
                float height = std::max(ws.height, hsz.height);
                height += 20;
                
                //ovoice->setTag(vid);
                //vid++;
                
                UIPanel *pan = static_cast<UIPanel*>(otherVoice->clone());
                pan->setEnabled(true);
                pan->setSize(CCSizeMake(fs.width, height));
                pan->setSizeType(SIZE_ABSOLUTE);
                pan->setVisible(true);
                Button *newVoice = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "ovoice"));
                newVoice->setTag(imgId);
                newVoice->ignoreContentAdaptWithSize(false);
                newVoice->setSizeType(SIZE_ABSOLUTE);
                newVoice->setSize(ws);
                //加载的发送的图像数据不对为什么呢？
                newVoice->loadTextureNormal(key, UI_TEX_TYPE_LOCAL);
                //newVoice->addTouchEventListener(this, toucheventselector(ChatView::onOtherVoice));
                //newVoice->setTag(vid);
                //vid++;
                
                lv->pushBackCustomItem(pan);
            }
            
            
            
            
        }
    }
}

void ChatView::onText(cocos2d::CCObject *obj, TextFiledEventType tt) {
    switch (tt) {
        case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME:
        {
            //出现键盘之后用户就不能点击了
            
            tf->setTouchEnabled(false);
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            float nw = 400*fs.width/640;
            tf->setSize(CCSizeMake(nw, 400));
            
            
            showKeyboard = true;
            CCLog("setSizeYet %d",setSizeYet);
            //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, screenSize.height/2)));
            if (setSizeYet) {
                bottom->runAction(CCMoveTo::create(0.225, ccp(0, ksize.height)));
            }
            //CCEGLView *pegl = CCDirector::sharedDirector()->getOpenGLView();
            //pegl->setIMEKeyboardState(true);
            
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
            float nw = 400*fs.width/640;
            tf->setSize(CCSizeMake(nw, 58));
            //关闭键盘
            CCLog("event detach with ime");
            bottom->runAction(CCMoveTo::create(0.225, ccp(0, 0)));
            //CCEGLView *pegl = CCDirector::sharedDirector()->getOpenGLView();
            //pegl->setIMEKeyboardState(false);
            //
            tf->setTouchEnabled(true);
            
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT:
        {
            adjustBut();
        }
            break;
        case cocos2d::ui::TEXTFIELD_EVENT_DELETE_BACKWARD:
        {
            adjustBut();
        }
            break;
        default:
            break;
    }
}

void ChatView::adjustBut() {
    string con = tf->getStringValue();
    if (con.length() > 0) {
        add->setEnabled(false);
        send->setEnabled(true);
    } else {
        add->setEnabled(true);
        send->setEnabled(false);
        
    }
}