#include "CocosGUIScene.h"
#include "CocosGUISlotScene.h"
//#include "CCScale9Sprite.h"


CocosGUITestScene::CocosGUITestScene(bool bPortrait)
{
	TestScene::init();
    
	CCSprite *bg = CCSprite::create("armature/bg.jpg");
	bg->setPosition(VisibleRect::center());
    
	float scaleX = VisibleRect::getVisibleRect().size.width / bg->getContentSize().width;
	float scaleY = VisibleRect::getVisibleRect().size.height / bg->getContentSize().height;
    
	bg->setScaleX(scaleX);
	bg->setScaleY(scaleY);
    
	addChild(bg);
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
    
    ul->addWidget(CCUIHELPER->createWidgetFromJsonFile("cocosgui/test/UI_Register.json"));
//    UIScrollView* sc = (UIScrollView*)(ul->getWidgetByName("scrollview"));
//    sc->setUpdateEnable(true);
//    UIButton* exitBtn = dynamic_cast<UIButton*>(ul->getWidgetByName("exitbutton"));
//    exitBtn->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUISlotScene));
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
    CCLOG("p1 click");
    CCLOG("p1 retain count == %d",ul->retainCount());
    ul->removeFromParent();
}

void CocosGUITestScene::toCocosGUISlotScene(CCObject* pSender)
{
    CCLOG("p2 click");
}