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
    for (int i=0; i<6; i++)
    {
        UIButton* btn = UIButton::create();
        btn->setTextures("cocosgui/animationbuttonnormal.png", "cocosgui/animationbuttonpressed.png", NULL);
//        btn->setMargin(UIMargin(0, 10, 0, 10));
        btn->setTouchEnable(true);
        p->addChild(btn);
        if (i==1)
        {
            btn->setAnchorPoint(ccp(2345, 2345));
            btn->setLinearGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
            btn->setRelativeAlign(RELATIVE_ALIGN_PARENT_RIGHT);
            btn->setName("btn");
        }
        if (i == 2)
        {
            btn->setRelativeAlign(RELATIVE_ALIGN_PARENT_BOTTOM);
        }
        if (i == 3)
        {
            btn->setRelativeAlign(RELATIVE_CENTER_IN_PARENT);
        }
        
        if (i == 4)
        {
            btn->setRelativeAlign(RELATIVE_CENTER_HORIZONTAL);
        }
        if (i == 5) {
            btn->setRelativeAlign(RELATIVE_CENTER_VERTICAL);
        }
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
        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->getChildByName("btn")->setMargin(UIMargin(10,10,10,10));
    }
    else
    {
        ((UIContainerWidget*)ul->getWidgetByName("linearlayout"))->getChildByName("btn")->setMargin(UIMargin(0,0,0,0));
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
