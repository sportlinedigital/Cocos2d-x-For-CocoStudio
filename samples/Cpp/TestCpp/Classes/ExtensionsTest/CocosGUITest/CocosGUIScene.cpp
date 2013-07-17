#include "CocosGUIScene.h"
#include "CocosGUIExamplesScene.h"


CocosGUITestScene::CocosGUITestScene(bool bPortrait)
{
	TestScene::init();
    
//	CCSprite *bg = CCSprite::create("armature/bg.jpg");
//	bg->setPosition(VisibleRect::center());
//    
//	float scaleX = VisibleRect::getVisibleRect().size.width / bg->getContentSize().width;
//	float scaleY = VisibleRect::getVisibleRect().size.height / bg->getContentSize().height;
//    
//	bg->setScaleX(scaleX);
//	bg->setScaleY(scaleY);
//    
//	addChild(bg);
}
void CocosGUITestScene::runThisTest()
{
    
	CCDirector::sharedDirector()->replaceScene(this);
    
    ul = UILayer::create();
    ul->scheduleUpdate();
    this->addChild(ul);
    
//    UILayer* ul2 = UILayer::create();
//    this->addChild(ul2);
//    
//    UIPanel* p1 = UIPanel::create();
//    p1->setBackGroundImageScale9Enable(true);
//    p1->setBackGroundColorType(PANEL_COLOR_GRADIENT);
//    p1->setBackGroundColor(ccGREEN,ccWHITE);
//    p1->setOpacity(255);
//    p1->setPosition(ccp(100, 100));
//    p1->setSize(CCSizeMake(100, 100));
//    p1->setTouchEnable(true);
//    p1->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::MainMenuCallback));
//    
//    UIPanel* p2 = UIPanel::create();
//    p2->setBackGroundImageScale9Enable(true);
//    p2->setBackGroundColorType(PANEL_COLOR_SOLID);
//    p2->setBackGroundColor(ccGREEN);
//    p2->setOpacity(255);
//    p2->setPosition(ccp(150, 150));
//    p2->setSize(CCSizeMake(100, 100));
//    p2->setTouchEnable(true);
//    p2->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUISlotScene));
//    
//    ul->unscheduleUpdate();
//    CCLOG("p1 retain count == %d",ul->retainCount());
//    
//    ul->addWidget(p1);
//    ul2->addWidget(p2);
//    return;
    
    

    ul->addWidget(CCUIHELPER->createWidgetFromJsonFile("cocosgui/CocoGUISample.json"));
    
//    ul->setScale(0.5f);
//    ul->getWidgetByName("scrollview")->setScale(2);
//    ul->getWidgetByName("scrollview")->setRotation(90);
    
//    ul->getWidgetByName("animationbutton")->setColor(ccGREEN);
    
//
    UIButton* exitBtn = dynamic_cast<UIButton*>(ul->getWidgetByName("exitbutton"));
    exitBtn->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUIExampleScene));
    
//    CCSprite* sp = CCSprite::create("cocosgui/CloseNormal.png");
    
//    UIPanel* panel = UIPanel::create();
//    panel->setSize(CCSizeMake(100, 100));
//    panel->setPosition(ccp(100, 100));
//    panel->setBackGroundColorOpacity(100);
//    panel->setOpacity(1);
//    panel->setBackGroundColorType(PANEL_COLOR_SOLID);
//    panel->setBackGroundColor(ccGREEN);
//    
//    panel->setBackGroundImage("cocosgui/animationbuttonpressed.png");
//    
//    ul->addWidget(panel);
//
//    
//    panel->setClippingEnable(true);
//    
////    UINodeContainer* nc = UINodeContainer::create();
////    nc->addCCNode(sp);
//    UIButton* btn = UIButton::create();
//    btn->setTextures("cocosgui/CloseNormal.png", "", NULL);
//    btn->setOpacity(100);
//    panel->addChild(btn);
    
    
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
