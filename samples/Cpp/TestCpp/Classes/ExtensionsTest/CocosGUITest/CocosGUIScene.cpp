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
    p->setSize(CCSizeMake(300, 300));
    p->setPosition(ccp(10, 10));
    p->setBackGroundColor(ccGREEN);
    p->setBackGroundColorType(PANEL_COLOR_SOLID);
    p->setClippingEnable(true);
    for (int i=0; i<3; i++)
    {
        UIButton* btn = UIButton::create();
        btn->setTextures("cocosgui/animationbuttonnormal.png", "cocosgui/animationbuttonpressed.png", NULL);
//        btn->setMargin(UIMargin(0, 10, 0, 10));
        btn->setTouchEnable(true);
        p->addChild(btn);
        if (i==1)
        {
//            btn->setAnchorPoint(ccp(2345, 2345));
            btn->setLinearGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
            btn->setRelativeAlign(RELATIVE_CENTER_IN_PARENT);
            btn->setName("btn");
            btn->setRelativeLayoutName("relaBtn");
            btn->setMargin(UIMargin(10,10,10,10));
        }
        if (i == 2)
        {
            btn->setTextures("cocosgui/backtotopnormal.png", "cocosgui/backtotopnormal.png", NULL);
            btn->setRelativeAlign(RELATIVE_LOCATION_ABOVE);
            btn->setRelativeAlignWidget(RELATIVE_ALIGN_WIDGET_BOTTOM);
            btn->setRelativeWidgetName("relaBtn");
            btn->setName("relaBtn");
        }
//        if (i == 3)
//        {
//            btn->setRelativeAlign(RELATIVE_CENTER_IN_PARENT);
//        }
//        
//        if (i == 4)
//        {
//            btn->setRelativeAlign(RELATIVE_CENTER_HORIZONTAL);
//        }
//        if (i == 5) {
//            btn->setRelativeAlign(RELATIVE_CENTER_VERTICAL);
//        }
    }
    p->setName("linearlayout");
    
    p->setLayoutType(UI_LAYOUT_RELATIVE);
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
