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
    
    UIPanel* pp = UIPanel::create();
    pp->setBackGroundColorEnable(true);
    pp->setColor(ccc3(0, 255, 0));
    pp->setOpacity(255);
    pp->setPosition(ccp(100, 100));
    pp->setSize(CCSizeMake(100, 100));


    ul->addWidget(pp);
    
    UIPanel* ps = UIPanel::create();
    ps->setBackGroundColorEnable(true);
    ps->setColor(ccc3(0, 0, 255));
    ps->setOpacity(255);
    ps->setPosition(ccp(75, 75));
    ps->setSize(CCSizeMake(50, 50));

    pp->addChild(ps);
//    ul->addWidget(ps);
    pp->setClippingEnable(true);
        ps->setClippingEnable(true);

//    ps->setCloseClip(false);
    
    
    UIButton* b1 = UIButton::create();
    b1->setTextures("cocosgui/CloseNormal.png", NULL, NULL);
    
    UIButton* b2 = UIButton::create();
    b2->setTextures("cocosgui/CloseNormal.png", NULL, NULL);
    
    pp->addChild(b1);
    ps->addChild(b2);
    
//    ul->addWidget(CCUIHELPER->createWidgetFromJsonFile("cocosgui/CocoGUISample.json"));
//    UIScrollView* sc = (UIScrollView*)(ul->getWidgetByName("scrollview"));
//    sc->setUpdateEnable(true);
//    sc->setMoveMode(SCROLLVIEW_MOVE_MODE_NORMAL);
//    UIButton* exitBtn = dynamic_cast<UIButton*>(ul->getWidgetByName("exitbutton"));
//    exitBtn->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUISlotScene));
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
	TestScene::MainMenuCallback(pSender);
//    UIActionManager::shareManager()->PlayActionByName("Animation1");
}

void CocosGUITestScene::toCocosGUISlotScene(CCObject* pSender)
{    
    CocosGUISlotScene *pScene = new CocosGUISlotScene();
    if (pScene)
    {
        ul->clear();
        
        pScene->runThisTest();
        pScene->release();
    }
}