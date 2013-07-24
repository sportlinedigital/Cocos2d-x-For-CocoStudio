#include "CocosGUIScene.h"
#include "CocosGUIExamplesScene.h"


CocosGUITestScene::CocosGUITestScene(bool bPortrait)
{
	TestScene::init();
}
void CocosGUITestScene::runThisTest()
{
    
	CCDirector::sharedDirector()->replaceScene(this);
    
    ul = UILayer::create();
    ul->scheduleUpdate();
    this->addChild(ul);
//    ul->addWidget(CCUIHELPER->createWidgetFromJsonFile("cocosgui/CocoGUISample.json"));
//    UIButton* exitBtn = dynamic_cast<UIButton*>(ul->getWidgetByName("exitbutton"));
//    exitBtn->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUIExampleScene));
    UIPanel* p = UIPanel::create();
    p->setSize(CCSizeMake(100, 100));
    p->setPosition(ccp(100, 100));
    p->setBackGroundColor(ccGREEN);
    p->setBackGroundColorType(PANEL_COLOR_SOLID);
    
    for (int i=0; i<10; i++)
    {
        UIButton* btn = UIButton::create();
        btn->setTextures("cocosgui/backtotopnormal.png", NULL, NULL);
        p->addChild(btn);
    }
    
    p->setLayoutType(UI_LAYOUT_LINEAR_VERTICAL);
    
    ul->addWidget(p);
    
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
    CCLOG("p1 click");
    CCLOG("p1 retain count == %d",ul->retainCount());
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
