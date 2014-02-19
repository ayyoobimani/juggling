#include "JG_Tutorial_Handler.h"


CCString JG_Tutorial_Handler::haveSeenTutorial = "HAVE_SEEN_TUTORIAL";

JG_Tutorial_Handler::JG_Tutorial_Handler(void)
{
	currentShowingPictureNumber = 1;
	playTutorial();
}


JG_Tutorial_Handler::~JG_Tutorial_Handler(void)
{

}

void JG_Tutorial_Handler::playTutorial()
{
	currentBackGround = CCSprite::create("tutorial_1.png");
	screenSize = CCDirector::sharedDirector()->getWinSize();
	currentBackGround->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	ScaleSpriteToDesignSize(currentBackGround);
	this->addChild(currentBackGround,-100);
	bIsPlayingForFirstTime = mustPlayTutorial();
	CreateCCMenu();
	createButtons();
}

CCScene* JG_Tutorial_Handler::scene()
{
	JG_Tutorial_Handler * tutorialHandler = new JG_Tutorial_Handler();
	CCScene* scene = CCScene::create();

	scene->addChild(tutorialHandler);

	CCLOG("tutorial handler created successfully");
	return scene;
}

bool JG_Tutorial_Handler::mustPlayTutorial()
{
	string record=CCUserDefault::sharedUserDefault()->getStringForKey(haveSeenTutorial.getCString());
	if(record.empty())
	{
		return true;
	}
	return false;
}

void JG_Tutorial_Handler::showNextPage(CCObject* pSender)
{
	if(currentShowingPictureNumber < NumberOfPicturs)
	{
		removeChild(currentBackGround);
		currentShowingPictureNumber++;
		currentBackGround = CCSprite::create(CCString::createWithFormat("tutorial_%i.png",currentShowingPictureNumber)->getCString());
		currentBackGround->setPosition(ccp(screenSize.width/2,screenSize.height/2));
		this->addChild(currentBackGround,-100);
	}
	else
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(haveSeenTutorial.getCString(),"1");
		CCUserDefault::sharedUserDefault()->flush();
		CCDirector::sharedDirector()->replaceScene(JG_Game_Main::scene());
	}

	if(currentShowingPictureNumber == NumberOfPicturs )
	{
		if(bIsPlayingForFirstTime)
		{
				toggleNextToPlayButton(true);
		}
		else
		{
			nextButton->setVisible(false);
		}

	}

	if(currentShowingPictureNumber == 2)
	{
		backButton->setVisible(true);
	}
}

void JG_Tutorial_Handler::showPrevPage(CCObject* pSender)
{
	if(currentShowingPictureNumber < 2)
		return;

	removeChild(currentBackGround);
	currentShowingPictureNumber--;
	currentBackGround = CCSprite::create(CCString::createWithFormat("tutorial_%i.png",currentShowingPictureNumber)->getCString());
	currentBackGround->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	this->addChild(currentBackGround,-100);

	if(currentShowingPictureNumber == 1)
	{
		if(bIsPlayingForFirstTime)
		{
			toggleNextToPlayButton(false);
		}
		else
		{
			nextButton->setVisible(true);
		}
		
	}

	if(currentShowingPictureNumber == (NumberOfPicturs -1) )
	{
		toggleNextToPlayButton(false);
	}
}

void JG_Tutorial_Handler::CreateCCMenu()
{
	ccMenue = CCMenu::create();
	this->addChild(ccMenue);
	ccMenue->setPosition(CCPointZero);
	ccMenue->retain();
}

CCMenuItemSprite* JG_Tutorial_Handler::CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio)
{
	CCMenuItemSprite * menuItem;
	menuItem = CCMenuItemSprite::create(CCSprite::create(normalImage.getCString())
		,CCSprite::create(selectedImage.getCString())
		,target
		,selector);
	menuItem->setPosition(ccp(screenSize.width * positionRatio.x ,screenSize.height * positionRatio.y));

	ccMenue->addChild(menuItem,100);
	return menuItem;
}

void JG_Tutorial_Handler::createButtons()
{
	nextButton = CreateButton("Buttons/Tutorial/next1.png"
		, "Buttons/Tutorial/next2.png", this
		, menu_selector(JG_Tutorial_Handler::showNextPage),ccp(0.8,0.1));
	nextButton->setVisible(true);

	backButton = CreateButton("Buttons/Tutorial/previous1.png"
		, "Buttons/Tutorial/previous2.png", this
		, menu_selector(JG_Tutorial_Handler::showPrevPage), ccp(0.1,0.1));
	backButton->setVisible(false);

	backToMenueButton = CreateButton("Buttons/Tutorial/menu1.png"
		, "Buttons/Tutorial/menu2.png", this
		, menu_selector(JG_Tutorial_Handler::goBackToMainMenue), ccp(0.8,0.8));
	backToMenueButton->setVisible(true);

}

void JG_Tutorial_Handler::toggleNextToPlayButton(bool bMustToggle)
{
	CCLog("toggle function declared");
	if(bMustToggle)
	{
		nextButton->setNormalImage(CCSprite::create("Buttons/Tutorial/play1.png"));
		nextButton->setSelectedImage(CCSprite::create("Buttons/Tutorial/play2.png"));
	}
	else{
		nextButton->setNormalImage(CCSprite::create("Buttons/Tutorial/next1.png"));
		nextButton->setSelectedImage(CCSprite::create("Buttons/Tutorial/next2.png"));
	}
	
}

void JG_Tutorial_Handler::goBackToMainMenue(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(JG_Menu_Main::scene());
}
