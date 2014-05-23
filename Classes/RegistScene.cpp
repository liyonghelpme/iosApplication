//
//  RegistScene.cpp
//  BallParty
//
//  Created by dozengame on 14-5-13.
//
//

#include "RegistScene.h"
#include "Md5.h"

UIButton* m_registBtn;
UIWidget* addshow;
CCScene* RegistScene::scene()
{
	CCScene* scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		RegistScene* layer = RegistScene::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}
bool RegistScene::init(){
    CCLayer::init();
    
    v_phoneNum = "";
    v_password = "";
    v_itrsPhoneNum = "";
    
    return true;
}
void RegistScene::onEnter(){
    CCLayer::onEnter();
     
	TouchGroup* ui = TouchGroup::create();
	addshow = GUIReader::shareReader()->widgetFromJsonFile("Scene/RegistScene/RegistScene.json");
	ui->addWidget(addshow);
	ui->setAnchorPoint(CCPointZero);
	this->addChild(ui);
    
    //可视区域
    UIPanel* setup1 = (UIPanel*)addshow->getChildByTag(20);
    UIPanel* setup2 = (UIPanel*)addshow->getChildByTag(44);
    
    //32强
    UILabel* likeLabel = (UILabel*)addshow->getChildByTag(44)->getChildByTag(89)->getChildByTag(146);
    likeLabel->addTouchEventListener(this, toucheventselector(RegistScene::worldCupStrongPress));
    
    UIPanel* listPanel = (UIPanel*)addshow->getChildByTag(102);
    UIListView* listView = (UIListView*)listPanel->getChildByTag(110);
    //listPanel->setEnabled(false);
    Layout* default_item = Layout::create();
    default_item->setTouchEnabled(true);
    //default_item->setSize(CCSizeMake(640, 30*75));
    
    listView->setItemModel(default_item);
    
    for(int i=0; i<30; i++){
        UIButton* m_option = UIButton::create();
        m_option->setTouchEnabled(true);
        m_option->loadTextures("Scene/RegistScene/bottom_line.png", "Scene/RegistScene/bottom_line.png", "Scene/RegistScene/bottom_line.png");
        //default_item->addChild(m_option);
        UILabel* m_label = UILabel::create();
        if(i % 2 == 0){
            m_label->setText("中华人民共和国");
        }else{
            m_label->setText("中国");
        }
        
        m_label->setFontSize(26);
        m_label->setColor(ccc3(0,0,0));
        m_label->setAnchorPoint(ccp(0, 0.5));
        m_label->setPosition(ccp(-310,0));
        m_option->addChild(m_label);
        
        char buffer[10];
        sprintf(buffer, "%d", i);
        m_option->setName(buffer);
        
        m_option->addTouchEventListener(this, toucheventselector(RegistScene::listviewPress));
        
        listView->pushBackCustomItem(m_option);
    }
    
    
    setup2->setEnabled(false);
    
    //注册按钮
    m_registBtn = (UIButton*)addshow->getChildByTag(20)->getChildByTag(14);
    m_registBtn->addTouchEventListener(this,toucheventselector(RegistScene::registPress));
    //完成注册按钮
    UIButton* m_finishBtn = (UIButton*)addshow->getChildByTag(44)->getChildByTag(94);
    m_finishBtn->addTouchEventListener(this, toucheventselector(RegistScene::finishPress));
    
    //手机号码
    m_phoneNum = CCEditBox::create(CCSize(300,40), CCScale9Sprite::createWithSpriteFrameName("button.png"));
	m_phoneNum->setFontSize(23);
	m_phoneNum->setAnchorPoint(CCPointZero);
	m_phoneNum->setPlaceHolder("请输入手机号");
	m_phoneNum->setInputMode(kEditBoxInputModePhoneNumber);
	m_phoneNum->setReturnType(kKeyboardReturnTypeDone);
    m_phoneNum->setFontColor(ccc3(80,80,80));
	m_phoneNum->setPosition(ccp(180,750));
	m_phoneNum->setDelegate(this);
	m_phoneNum->setTouchPriority(0);

    //密码
    m_password = CCEditBox::create(CCSize(300,40),CCScale9Sprite::createWithSpriteFrameName("button.png"));
	m_password->setFontSize(23);
	m_password->setAnchorPoint(CCPointZero);
	m_password->setPlaceHolder("请输入密码");
	m_password->setInputMode(kEditBoxInputModeAny);
	m_password->setInputFlag(kEditBoxInputFlagPassword);
	m_password->setReturnType(kKeyboardReturnTypeDone);
    m_password->setFontColor(ccc3(80,80,80));
	m_password->setPosition(ccp(180,635));
	m_password->setDelegate(this);
	m_password->setTouchPriority(0);
    
    //确认密码
    m_cfmPassWord = CCEditBox::create(CCSize(300,40),CCScale9Sprite::createWithSpriteFrameName("button.png"));
	m_cfmPassWord->setFontSize(23);
	m_cfmPassWord->setAnchorPoint(CCPointZero);
	m_cfmPassWord->setPlaceHolder("请确认密码");
	m_cfmPassWord->setInputMode(kEditBoxInputModeAny);
	m_cfmPassWord->setInputFlag(kEditBoxInputFlagPassword);
	m_cfmPassWord->setReturnType(kKeyboardReturnTypeDone);
    m_cfmPassWord->setFontColor(ccc3(80,80,80));
	m_cfmPassWord->setPosition(ccp(180,514));
	m_cfmPassWord->setDelegate(this);
	m_cfmPassWord->setTouchPriority(0);

     //真实姓名
     m_realName = CCEditBox::create(CCSize(300,40),CCScale9Sprite::createWithSpriteFrameName("button.png"));
     m_realName->setFontSize(23);
     m_realName->setAnchorPoint(CCPointZero);
     m_realName->setPlaceHolder("请输入姓名");
     m_realName->setInputMode(kEditBoxInputModeAny);
     m_realName->setReturnType(kKeyboardReturnTypeDone);
     m_realName->setFontColor(ccc3(80,80,80));
     m_realName->setPosition(ccp(180,522));
     m_realName->setDelegate(this);
     m_realName->setTouchPriority(0);
    
    //推荐人手机号
    m_itrsPhoneNum = CCEditBox::create(CCSize(300,40),CCScale9Sprite::createWithSpriteFrameName("button.png"));
	m_itrsPhoneNum->setFontSize(23);
	m_itrsPhoneNum->setAnchorPoint(CCPointZero);
	m_itrsPhoneNum->setPlaceHolder("请输入推荐人手机号");
	m_itrsPhoneNum->setInputMode(kEditBoxInputModePhoneNumber);
	m_itrsPhoneNum->setReturnType(kKeyboardReturnTypeDone);
    m_itrsPhoneNum->setFontColor(ccc3(80,80,80));
	m_itrsPhoneNum->setPosition(ccp(180,441));
	m_itrsPhoneNum->setDelegate(this);
	m_itrsPhoneNum->setTouchPriority(0);
    

    
    setup1->addNode(m_phoneNum);
    setup1->addNode(m_password);
    setup1->addNode(m_cfmPassWord);
    
    setup2->addNode(m_realName);
    setup2->addNode(m_itrsPhoneNum);
    m_realName->setEnabled(false);
    m_itrsPhoneNum->setEnabled(false);
    
    listPanel->setEnabled(false);
}


//注册
void RegistScene::registPress( CCObject *pSender,TouchEventType type )
{
    
	if (type == TOUCH_EVENT_ENDED)
	{


        v_phoneNum = m_phoneNum->getText();
        v_password = m_password->getText();
        v_cfmPassword = m_cfmPassWord->getText();
        
        //选项不为空
        if(v_password=="" || v_phoneNum==""){
            CCMessageBox("所有选项不能为空！", "提示");
            return;
        }
        
        //确认密码是否一致
        if ( strcmp(v_password.c_str(), v_cfmPassword.c_str()) != 0) {
            CCMessageBox("密码不一致！", "提示");
            return;
        }
        
        showPanel(2);
	}
}

//finish
void RegistScene::finishPress( CCObject *pSender,TouchEventType type )
{
    
	if (type == TOUCH_EVENT_ENDED)
	{
        UserService::shareUserService()->registe(v_phoneNum, md5(v_password), md5(v_cfmPassword), m_itrsPhoneNum->getText(),this,MyHttpResp(&RegistScene::registEnd));
        CCLOG("finish");
	}
}

//listview press
void RegistScene::listviewPress( CCObject *pSender,TouchEventType type )
{
    UIButton* m_btn = dynamic_cast<UIButton*>(pSender);
    
	if (type == TOUCH_EVENT_ENDED)
	{
        CCMessageBox(m_btn->getName(), "提示");
        CCLOG(m_btn->getName());
        UIPanel* listPanel = (UIPanel*)addshow->getChildByTag(102);
        listPanel->setEnabled(false);
        
        UILabel* likeLabel = (UILabel*)addshow->getChildByTag(44)->getChildByTag(89)->getChildByTag(146);
        likeLabel->setText(m_btn->getName());
	}
}

//32强
void RegistScene::worldCupStrongPress( CCObject *pSender,TouchEventType type )
{
    
	if (type == TOUCH_EVENT_ENDED)
	{
        UIPanel* listPanel = (UIPanel*)addshow->getChildByTag(102);
        listPanel->setEnabled(true);
	}
}


void RegistScene::registEnd(bool suc, std::string s, void*param)
{
    NetRegist regData = UserService::shareUserService()->analyzeRegistRect(s);
    CCLog("net regist %d", regData.status);
    if(regData.status == 1){
        CCDirector* pDirector = CCDirector::sharedDirector();
        //CCScene* pScene = ChatRoomScene::scene();
        //pDirector->replaceScene(pScene);
        
        //存储数据到Logic里面即可
        
    }else{
        CCMessageBox(regData.msg.c_str(), "提示");
    }
    //缓存到内存中
    //GloabData::shareGloabData()->self_User = user;
    
}



void RegistScene::showPanel(int setup){
    //可视区域
    UIPanel* setup1 = (UIPanel*)addshow->getChildByTag(20);
    UIPanel* setup2 = (UIPanel*)addshow->getChildByTag(44);
    
    if(setup == 2){
        //转到setup2
        setup1->setPosition(ccp(0, 9999));
        setup1->setEnabled(false);
        
        setup2->setEnabled(true);
        m_realName->setEnabled(true);
        m_itrsPhoneNum->setEnabled(true);
    }
    //CCFileUtils
    //CCDictionary
}

void RegistScene::editBoxEditingDidBegin( cocos2d::extension::CCEditBox* editBox )
{
}

void RegistScene::editBoxEditingDidEnd( cocos2d::extension::CCEditBox* editBox )
{
    
}

void RegistScene::editBoxTextChanged( cocos2d::extension::CCEditBox* editBox, const std::string& text )
{
    
}

void RegistScene::editBoxReturn( cocos2d::extension::CCEditBox* editBox )
{
    
}
void RegistScene::onExit(){
    CCLayer::onExit();
    
    
}