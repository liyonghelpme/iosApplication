//
//  LoginScene.cpp
//  BallParty
//
//  Created by dozengame on 14-5-13.
//
//

#include "LoginScene.h"
//#include "md5.h"
#include "Md5.h"


UIWidget* loginWidget;

CCScene* LoginScene::scene()
{
	CCScene* scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		LoginScene* layer = LoginScene::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}
bool LoginScene::init(){
    CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pCache->addSpriteFramesWithFile("Scene/LoginLayer/LoginSence0.plist");
    
    return true;
}
void LoginScene::onEnter(){
    CCLayer::onEnter();
	TouchGroup* ui = TouchGroup::create();
	loginWidget = GUIReader::shareReader()->widgetFromJsonFile("Scene/LoginSence/LoginSence.json");
	ui->addWidget(loginWidget);
	ui->setAnchorPoint(CCPointZero);
	this->addChild(ui);
    
    //登陆区域
    UIPanel* login_panel = (UIPanel*)loginWidget->getChildByTag(4);
    
    //用户名
    m_userName = CCEditBox::create(CCSize(320,40), CCScale9Sprite::createWithSpriteFrameName("button.png"));
	m_userName->setFontSize(23);
	m_userName->setAnchorPoint(CCPointZero);
	m_userName->setPlaceHolder("请输入用户名");
    m_userName->setPlaceholderFontColor(ccc3(248,252,255));
	m_userName->setInputMode(kEditBoxInputModeAny);
	m_userName->setReturnType(kKeyboardReturnTypeDone);
	m_userName->setPosition(ccp(153,378));
	m_userName->setDelegate(this);
	m_userName->setTouchPriority(0);
    //密码
    m_passWord = CCEditBox::create(CCSize(320,40), CCScale9Sprite::createWithSpriteFrameName("button.png"));
	m_passWord->setFontSize(23);
	m_passWord->setAnchorPoint(CCPointZero);
	m_passWord->setPlaceHolder("请输入密码");
    m_passWord->setPlaceholderFontColor(ccc3(248,252,255));
	m_passWord->setInputMode(kEditBoxInputModeAny);
    m_passWord->setInputFlag(kEditBoxInputFlagPassword);
	m_passWord->setReturnType(kKeyboardReturnTypeDone);
	m_passWord->setPosition(ccp(153,310));
	m_passWord->setDelegate(this);
	m_passWord->setTouchPriority(0);

    UIButton* loginBtn = (UIButton*)login_panel->getChildByTag(8);
    loginBtn->addTouchEventListener(this,toucheventselector(LoginScene::loginPress));
    
    UIButton* registerBtn = (UIButton*)login_panel->getChildByTag(44);
    registerBtn->addTouchEventListener(this, toucheventselector(LoginScene::registerPress));
    
    login_panel->addNode(m_userName);
    login_panel->addNode(m_passWord);
    
    //更新显示
    refurbishScene();

    
}

void LoginScene::refurbishScene(){
    //本地有存储用户名
    string loginNmaeKey = CCUserDefault::sharedUserDefault()->getStringForKey("loginName");
    UIPanel* login_panel = (UIPanel*)loginWidget->getChildByTag(4);
    UIImageView* userIcon = (UIImageView*)login_panel->getChildByName("userIcon");
    UIImageView* usernameLable = (UIImageView*)login_panel->getChildByName("usernameLiImage");
    
    if (loginNmaeKey != "") {
        //隐藏用户输入框
        m_userName->setText(loginNmaeKey.c_str());
        m_userName->setVisible(false);
        
        usernameLable->setVisible(false);
        
        UILabel* iconNameLabel = (UILabel*)userIcon->getChildByTag(47);
        iconNameLabel->setText(loginNmaeKey.c_str());
    }else{
        m_userName->setVisible(true);
        usernameLable->setVisible(true);
        userIcon->setVisible(false);
    }
}

void LoginScene::loginEnd(bool suc, std::string s, void*param)
{
    
    if(UserService::shareUserService()->analyzeLoginRect(s)){
    //登陆成功
        CCLog("loginEnd finish");
        CCDirector* pDirector = CCDirector::sharedDirector();
        
        //TODO 跳转登录成功后 进入赛事页面
        //CCScene* pScene = ChatRoomScene::scene();
        //pDirector->replaceScene(pScene);
        
        //保存本地用户名
        CCUserDefault::sharedUserDefault()->setStringForKey("loginName", m_userName->getText());
        CCUserDefault::sharedUserDefault()->flush();
    }else{
    //登陆失败
        CCLog("Login fail!");
        CCMessageBox("账号或密码错误！", "提示");
        //删除本地用户名
        CCUserDefault::sharedUserDefault()->setStringForKey("loginName", "");
        CCUserDefault::sharedUserDefault()->flush();
        
        refurbishScene();
    }
}

void LoginScene::registerPress(CCObject *pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED){
        
        CCDirector* pDirector = CCDirector::sharedDirector();
        
        //进入注册页面
        //CCScene* pScene = RegistScene::scene();
        //pDirector->replaceScene(pScene);

	}
    
}

void LoginScene::loginPress(CCObject *pSender,TouchEventType type)
{
	switch (type)
	{
        case TOUCH_EVENT_BEGAN:
            break;
        case TOUCH_EVENT_ENDED:
            //直接发送 暂时忽略验证
            
            UserService::shareUserService()->login(m_userName->getText(),md5(m_passWord->getText()),this,MyHttpResp(&LoginScene::loginEnd));
            break;
        case TOUCH_EVENT_CANCELED:
            break;
        case TOUCH_EVENT_MOVED: 
            break;
	}

}
void LoginScene::editBoxEditingDidBegin( cocos2d::extension::CCEditBox* editBox )
{
}

void LoginScene::editBoxEditingDidEnd( cocos2d::extension::CCEditBox* editBox )
{
    
}

void LoginScene::editBoxTextChanged( cocos2d::extension::CCEditBox* editBox, const std::string& text )
{
    
}

void LoginScene::editBoxReturn( cocos2d::extension::CCEditBox* editBox )
{
    
}

void LoginScene::onExit(){
    
    

}