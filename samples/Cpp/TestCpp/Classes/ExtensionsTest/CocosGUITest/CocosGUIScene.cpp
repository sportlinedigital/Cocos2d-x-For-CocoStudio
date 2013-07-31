#include "CocosGUIScene.h"
#include "CocosGUIExamplesScene.h"


CocosGUITestScene::CocosGUITestScene(bool bPortrait)
{
	TestScene::init();
}

CocosGUITestScene::~CocosGUITestScene()
{
	cocos2d::extension::CCJsonReader::purgeJsonReader();
	cocos2d::extension::UIActionManager::purgeUIActionManager();
	cocos2d::extension::UIHelper::purgeUIHelper();
}

void CocosGUITestScene::runThisTest()
{
    
	CCDirector::sharedDirector()->replaceScene(this);
    
    ul = UILayer::create();
    ul->scheduleUpdate();
    this->addChild(ul);
    ul->addWidget(CCUIHELPER->createWidgetFromJsonFile("cocosgui/CocoGUISample.json"));
    UIButton* exitBtn = dynamic_cast<UIButton*>(ul->getWidgetByName("exitbutton"));
    exitBtn->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUIExampleScene));
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
    ul->removeFromParent();
    TestScene::MainMenuCallback(pSender);
}

void CocosGUITestScene::toCocosGUIExampleScene(CCObject* pSender)
{
    CCLOG("p2 click");
    ul->removeFromParent();
    
    CocosGUIExamplesScene* pScene = new CocosGUIExamplesScene();
    if (pScene)
    {
        pScene->runThisTest();
        pScene->release();
    }
}
