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
#include "MyBase64.h"
#include "Channel.h"
#include "Message.h"


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
    enterChatYet = false;
    lastUpdateTime = 0;
    state = 0;
    
    
    CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
    
    setSizeYet = false;
    testLabel = CCLabelTTF::create("", "", 30);
    testLabel->retain();
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    UILayer *lay = UILayer::create();
    addChild(lay);
    
    w = GUIReader::shareReader()->widgetFromJsonFile("gui/ballUI_3_0.json");
    lay->addWidget(w);
    w->setSize(size);
    
    
    //正在录音
    inRecord = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "Panel_40"));
    inRecord->setEnabled(false);
    
    
    //w->addTouchEventListener(this, toucheventselector(ChatView::onBack));
    //CCLog("back is touch %d %d", w->isTouchEnabled(), w->isEnabled());
    
    
    Match *minf = Logic::getInstance()->matchInfo;
    
    Label *title = static_cast<Label*>(UIHelper::seekWidgetByName(w, "title"));
    //if (!DEBUG) {
    
    string tname = minf->host_name;
    tname += "VS";
    tname += minf->guest_name;
    title->setText(tname);
    
    curTime = static_cast<Label*>(UIHelper::seekWidgetByName(w, "time"));
    
   
    
    
    
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
    send->setEnabled(false);
    
    
    
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
    
    backbnt = static_cast<Button*>(UIHelper::seekWidgetByName(w, "back"));
    backbnt->addTouchEventListener(this, toucheventselector(ChatView::onBack));
    
    
    
    sayWord = static_cast<Button*>(UIHelper::seekWidgetByName(bottom, "voice"));
    sayWord->addTouchEventListener(this, toucheventselector(ChatView::onSay));
    myvoice = static_cast<UIPanel*>(UIHelper::seekWidgetByName(w, "myvoice"));
    myvImg = static_cast<Button*>(UIHelper::seekWidgetByName(myvoice, "voice2"));
    myvoice->setEnabled(false);
    
    
    myImg = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "myImg"));
    myImg->setEnabled(false);
    otherImg = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "otherImg"));
    otherImg->setEnabled(false);
    
    
    otherVoice = static_cast<Layout*>(UIHelper::seekWidgetByName(w, "otherVoice"));
    ohead = static_cast<ImageView*>(UIHelper::seekWidgetByName(otherVoice, "ohead"));
    ovoice = static_cast<Button*>(UIHelper::seekWidgetByName(otherVoice, "ovoice"));
    otherVoice->setEnabled(false);
    ovoice->addTouchEventListener(this, toucheventselector(ChatView::onOtherVoice));
    
    Button *cinfo = static_cast<Button*>(UIHelper::seekWidgetByName(w, "selfConfig"));
    cinfo->addTouchEventListener(this, toucheventselector(ChatView::onChatInfo));
    

    
    CCLog("initial listView");
    
    lv = static_cast<ListView*>(UIHelper::seekWidgetByName(w, "listView"));
    lv->addTouchEventListener(this, toucheventselector(ChatView::onBackgroundTouch));
    
    lv->addEventListenerListView(this, listvieweventselector(ChatView::onBackground));
    
    
    scheduleUpdate();
    return true;
}


void ChatView::onBack(cocos2d::CCObject *obj, TouchEventType tt){
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
            CCDirector::sharedDirector()->popScene();
            //退出聊天室就无法接受回调了可以交给Logic来处理
            
            HttpModel *hm = HttpModel::getInstance();
            std::map<string, string> postData;
            char buf[256];
            sprintf(buf, "%d", Logic::getInstance()->getUID());
            postData["userid"] = buf;
            
            //sprintf(buf, "%d", Logic::getInstance()->getCID());
            //postData["cid"] = buf;
            
            //postData["uid"] = Logic::getInstance()->getUID();
            //postData["cid"] = Logic::getInstance()->getCID();
            
            sprintf(buf, "match/%d", Logic::getInstance()->getCID());
            hm->addRequest(buf, "PUT", postData, NULL, NULL, NULL);
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
                //sayWord->setTitleText("Word");
                sayWord->loadTextures("gui/btn_Chat_Keyboard_Nor.png", "gui/btn_Chat_Keyboard_Press.png", "");
                
                tf->setEnabled(false);
                speak->setEnabled(true);
            }else{
                inVoice = false;
                //sayWord->setTitleText("Voice");
                sayWord->loadTextures("gui/btn_Chat_Voice_Nor.png", "gui/btn_Chat_Voice_Press.png", "");
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


string ChatView::getVoiceTime(int vl) {
    char buf[512];
    int smh[3];
    smh[0] = vl%60;
    vl /= 60;
    smh[1] = vl%60;
    vl /= 60;
    smh[2] = vl%60;
    
    bool notZero = false;
    string rt;
    for (int i = 2; i > -1; i--) {
        if (notZero || smh[i] > 0 || i == 0) {
            notZero = true;
            sprintf(buf, "%d\"", smh[i]);
            rt += buf;
        }
    }
    CCLog("length voice %s", rt.c_str());
    return rt;
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
            
            
            stopRecord();
            
            
            Label *vt = static_cast<Label*>(UIHelper::seekWidgetByName(pan, "voiceTime"));
            
            const char *fn = getFileName();
            CCLog("file name %s", fn);
            int vl = getVoiceLength(fn);
            CCLog("getVoiceLength %d", vl);
            
            char buf[512];
            int smh[3];
            smh[0] = vl%60;
            vl /= 60;
            smh[1] = vl%60;
            vl /= 60;
            smh[2] = vl%60;
            
            bool notZero = false;
            string rt;
            for (int i = 2; i > -1; i--) {
                if (notZero || smh[i] > 0 || i == 0) {
                    notZero = true;
                    sprintf(buf, "%d\"", smh[i]);
                    rt += buf;
                }
            }
            CCLog("length voice %s", rt.c_str());
            vt->setText(rt);
            
            
            
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            pan->setVisible(true);
            lv->pushBackCustomItem(pan);
            
            
            
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
            
            CCSize ws = lab2->getSize();
            CCSize hsz = head2->getSize();
            float height = std::max(ws.height, hsz.height);
            height += 20;
   
            
            UIPanel *pan = static_cast<UIPanel*>(twoWord->clone());
            ImageView *head2 = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "head2"));
            ImageView *chatBack = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "chatBack"));
            chatBack->setSize(CCSizeMake(ws.width+40, ws.height+20));
            char buf[512];
            sprintf(buf, "flags/%d.png", Logic::getInstance()->getFlagId());
            head2->loadTexture(buf);
            
            
            pan->setEnabled(true);
            pan->setSize(CCSizeMake(fs.width, height));
            pan->setSizeType(SIZE_ABSOLUTE);
            pan->setVisible(true);
            
            lv->pushBackCustomItem(pan);
            
            //保持指针传递或者传递string更安全
            //connect();
            //startSend(text.c_str());
            
            rapidjson::Document d;
            d.SetObject();
            
            rapidjson::Document::AllocatorType &allocator = d.GetAllocator();
            d.AddMember("type", "text", allocator);
            CCLog("send text content %s", text.c_str());
            
            //base64 encode
            int olen;
            char *odata = base64_encode((const unsigned char*)text.c_str(), (size_t)text.size(), (size_t*)&olen);
            std::string od = string(odata, olen);
            free(odata);
            
            d.AddMember("content", od.c_str(), allocator);
            
            //id number
            //d.AddMember("sender", Logic::getInstance()->getSenderId().c_str(), allocator);
            d.AddMember("sender", Logic::getInstance()->getSenderId(), allocator);
            d.AddMember("like_team", Logic::getInstance()->getFlagId(), allocator);
            
            
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            //没有对中文做utf8编码操作
            d.Accept(writer);
            
            CCLog("rapidjson encode content %s", strbuf.GetString());
            
            //int olen;
            //
            //sendText(strbuf.GetString());
            ChannelService::getInstance()->channelSendText(strbuf.GetString());
            
            
            //sendText(odata, olen);
            
            
            //free(odata);
            
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

//使用redis的 超时机制来注册用户的频道怎么样呢？
void ChatView::onEnterChat(bool isSuc, string s, void *param){
    CCLog("enter Chat suc");
}

void ChatView::updateEnter(float diff){
    //更新服务器上面 的用户在线状态
    if (!enterChatYet) {
        lastUpdateTime = 60;
        enterChatYet = true;
        HttpModel *hm = HttpModel::getInstance();
        std::map<string, string> postData;
        char buf[256];
        sprintf(buf, "%d", Logic::getInstance()->getUID());
        postData["userid"] = buf;
        
        //sprintf(buf, "%d", Logic::getInstance()->getCID());
        //postData["cid"] = buf;
        
        
        //hm->addRequest("enterChannel", "POST", postData, this, MYHTTP_SEL(ChatView::onEnterChat), NULL);
        //char buf[256];
        sprintf(buf, "match/%d", Logic::getInstance()->getCID());
        
        hm->addRequest(buf, "POST", postData, this, MYHTTP_SEL(ChatView::onEnterChat), NULL);
    } else {
        //更新数据
        lastUpdateTime -= diff;
        if (lastUpdateTime <= 0) {
            enterChatYet = false;
        }
    }
}

void ChatView::onMsg(bool isSuc, std::string s, void *param) {
    if (isSuc) {
        rapidjson::Document d;
        d.Parse<0>(s.c_str());
        
        
        CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
        float lwid = fs.width-114-10-30;
        
        for(int i =0; i <d["data"].Size(); i++) {
            const rapidjson::Value &b = d["data"][i];
            int uid = b["user_id"].GetInt();
            string ty = b["type"].GetString();
            int flagId = b["like_team"].GetInt();
            
            unsigned char* content = (unsigned char*)b["content"].GetString();
            if (uid == Logic::getInstance()->getUID()) {
                continue;
            }
            
            if (ty.compare("text") == 0 ){
                unsigned char *tword = content;
                unsigned char *out;
                int outLen = cocos2d::base64Decode(tword, strlen((const char*)tword), &out);
                string conText = string((const char*)out, (size_t)outLen);
                free(out);
                
                CCLog("read text is %s", conText.c_str());
                
                testLabel->setString(conText.c_str());
                //first test width
                testLabel->setDimensions(CCSize(0, 0));
                CCSize osz = testLabel->getContentSize();
                //CCSize tsz;
                if(osz.width > lwid){
                    testLabel->setDimensions(CCSize(lwid, 0));
                }
                
                //尺寸大一些才能容下资源
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
                ImageView *head = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "head"));
                ImageView *chatBack = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "chatBack"));
                chatBack->setSize(CCSizeMake(ws.width+40, ws.height+20));
                
                char buf[512];
                sprintf(buf, "flags/%d.png", flagId);
                head->loadTexture(buf);
                
                pan->setEnabled(true);
                pan->setSize(CCSizeMake(fs.width, height));
                pan->setSizeType(SIZE_ABSOLUTE);
                pan->setVisible(true);
                //加入新的用户消息
                CCLog("push CutonItem where");
                lv->pushBackCustomItem(pan);
            }
        }
        
        state = 2;
    }
}


//网络请求的时候要保存对象 直到请求结束才摧毁对象 从redis服务器接收请求
void ChatView::receiveMsg(){
    //接收该频道的消息
    if (state == 0) {
        state = 1;
        int cid = Logic::getInstance()->getCID();
        MessageService::getInstance()->getHistoryMessage(cid, this, MYHTTP_SEL(ChatView::onMsg));
        
    //从redis 服务器接收数据
    }else if (state == 2) {
        if (receive == NULL) {
            ChannelService::getInstance()->listenChannel();
            //startReceiveRedis();
            
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
                int flagId = d["like_team"].GetInt();
                
                std::string conText;
                bool isText = false;
                
                CCSize fs = CCDirector::sharedDirector()->getVisibleSize();
                float lwid = fs.width-114-10-30;
                int sender = d["sender"].GetInt();
                //自己发送的不要显示
                //显示发送者的头像数据 根据Id 获得相关头像
                //header Cache
                //调试关闭
                
                 //if (sender.compare(Logic::getInstance()->getSenderId()) == 0) {
                 //    return;
                 //}
                
                //后台只publish 一个索引信息 从服务器拉去 实际的语音讯息
                if (ctype.compare("text") == 0 ){
                    unsigned char *tword = (unsigned char*)d["content"].GetString();
                    unsigned char *out;
                    int outLen = cocos2d::base64Decode(tword, strlen((const char*)tword), &out);
                    conText = string((const char*)out, (size_t)outLen);
                    free(out);
                    
                    CCLog("read text is %s", conText.c_str());
                    
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
                    
                    char buf[512];
                    sprintf(buf, "flags/%d.png", flagId);
                    ImageView *head = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "head"));
                    head->loadTexture(buf);
                    ImageView *chatBack = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "chatBack"));
                    chatBack->setSize(CCSizeMake(ws.width+40, ws.height+20));
                    
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
                    
                    
                    Label *vt = static_cast<Label*>(UIHelper::seekWidgetByName(pan, "voiceTime"));
                    string rt = getVoiceTime(d["length"].GetInt());
                    vt->setText(rt);
                    
                    
                    Button *newVoice = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "ovoice"));
                    newVoice->addTouchEventListener(this, toucheventselector(ChatView::onOtherVoice));
                    newVoice->setTag(vid);
                    //vid++;
                    
                    lv->pushBackCustomItem(pan);
                    
                }else if(ctype.compare("image") == 0) {
                    
                    unsigned char *image = (unsigned char*)d["content"].GetString();
                    unsigned char *out;
                    int outLen = cocos2d::base64Decode(image, strlen((const char*)image), &out);
                    
                    
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
                    
                    UIPanel *pan = static_cast<UIPanel*>(otherImg->clone());
                    pan->setEnabled(true);
                    pan->setSize(CCSizeMake(fs.width, height));
                    pan->setSizeType(SIZE_ABSOLUTE);
                    pan->setVisible(true);
                    ImageView *img = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "img"));
                    img->loadTexture(key, UI_TEX_TYPE_LOCAL);
                    
                    
                    /*
                    Button *newVoice = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "ovoice"));
                    newVoice->setTag(imgId);
                    newVoice->ignoreContentAdaptWithSize(false);
                    newVoice->setSizeType(SIZE_ABSOLUTE);
                    newVoice->setSize(ws);
                    //加载的发送的图像数据不对为什么呢？
                    newVoice->loadTextureNormal(key, UI_TEX_TYPE_LOCAL);
                    */
                    //newVoice->addTouchEventListener(this, toucheventselector(ChatView::onOtherVoice));
                    //newVoice->setTag(vid);
                    //vid++;
                    
                    lv->pushBackCustomItem(pan);
                }
                
                
                
                
            }
        }
    }
}
void ChatView::sendImg(){
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
            
            
            UIPanel *pan = static_cast<UIPanel*>(myImg->clone());
            //Button *vimg = static_cast<Button*>(UIHelper::seekWidgetByName(pan, "voice2"));
            ImageView *img = static_cast<ImageView*>(UIHelper::seekWidgetByName(pan, "img"));
            img->loadTexture(key, UI_TEX_TYPE_LOCAL);
            
            /*
            CCLog("vimg name %s", vimg->getName());
            vimg->loadTextureNormal(key, UI_TEX_TYPE_LOCAL);
            vimg->setSize(ws);
            vimg->setSizeType(SIZE_ABSOLUTE);
            */
            //使用customSize 而不是图片自动size
            img->ignoreContentAdaptWithSize(false);
            
            
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
}
void ChatView::update(float diff){
    updateEnter(diff);
    sendImg();
    
    receiveMsg();
}

void ChatView::pinyinMove(){
    bottom->stopAllActions();
    bottom->runAction(CCMoveTo::create(0.2, ccp(0, ksize.height)));
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
            /*
            //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            //bottom->runAction(CCMoveTo::create(0.225, ccp(0, screenSize.height/2)));
            if (setSizeYet) {
                bottom->runAction(CCMoveTo::create(0.225, ccp(0, ksize.height)));
            }
             */
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