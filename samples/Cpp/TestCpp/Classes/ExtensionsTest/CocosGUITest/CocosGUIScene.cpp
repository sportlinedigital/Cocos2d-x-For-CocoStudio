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
    p->setClippingEnable(true);
    
    
    
    ul->addWidget(p);
    
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
    static bool is = true;
    if (is)
    {
//        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->getChildByName("relaBtn")->setRelativeAlign(RELATIVE_LOCATION_LEFT_OF);
//        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->getChildByName("relaBtn")->setMargin(UIMargin(10,10,10,10));
        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->setSize(CCSizeMake(200, 200));
    }
    else
    {
//        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->getChildByName("relaBtn")->setRelativeAlign(RELATIVE_LOCATION_RIGHT_OF);
//        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->getChildByName("relaBtn")->setMargin(UIMargin(10,10,10,10));
        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->setSize(CCSizeMake(300, 300));
    }
    is = !is;
    CCLOG("p1 click");
    CCLOG("p1 retain count == %d",ul->retainCount());
//    ul->removeFromParent();
//    TestScene::MainMenuCallback(pSender);
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
