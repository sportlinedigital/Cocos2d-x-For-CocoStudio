#include "CocosGUIScene.h"
#include "CocosGUISlotScene.h"
#include "CCScale9Sprite.h"


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
    
    /*
    CCSprite* spP = CCSprite::create("army_1.png");
    CCSprite* spS = CCSprite::create("army_1.png");
    spS->setPosition(ccp(0, 0));
    
    spP->addChild(spS);
    
    spP->setPosition(ccp(100, 100));
    
    ul->addChild(spP);
    
    CocoImageView* ivP = CocoImageView::create();
    ivP->setTexture("army_1.png");
    CocoImageView* ivS = CocoImageView::create();
    ivS->setTexture("army_1.png");
    ivS->setPosition(ccp(0+(-(ivP->getRelativeRect().size.width/2.0f)), 0+(-(ivP->getRelativeRect().size.height/2.0f))));
    
    ivP->addChild(ivS);
    
    ul->addWidget(ivP);
    ivP->setPosition(ccp(300, 100));
     */
    
    
    /*
    CCScale9Sprite* sp9 = CCScale9Sprite::create("army_1.png");
    
    ul->addChild(sp9);
    
    
    CocoImageView* iv9 = CocoImageView::create();
    iv9->setTexture("army_1.png");
    
    iv9->setScale9Enable(true);
    
    ul->addWidget(iv9);
    iv9->setPosition(ccp(300, 100));
    sp9->setPosition(ccp(100, 100));
    
    
    iv9->setAnchorPoint(ccp(0, 0));
    sp9->setAnchorPoint(ccp(0, 0));
     */
    

//    ul->addWidget(CCUIHELPER->createWidgetFromFile_ccb("ccb_parse/Tests/TestSprites.ccb"));
//    ul->addWidget(CCUIHELPER->createWidgetFromFile_ccb("ccb_parse/HelloCocosBuilder.ccb"));

//    /*
    ul->addWidget(CCUIHELPER->createWidgetFromFile_json("cocosgui/CocoGUISample.json"));
    CocoScrollView* sc = (CocoScrollView*)(ul->getWidgetByName("scrollview"));
    sc->setUpdateEnable(true);
    sc->setMoveMode(SCROLLVIEW_MOVE_MODE_NORMAL);
    
    CocoImageView* image = dynamic_cast<CocoImageView*>(ul->getWidgetByName("imageview"));
    image->setBeTouchAble(true);
//    image->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::MainMenuCallback));
    
    CocoButton* exitBtn = dynamic_cast<CocoButton*>(ul->getWidgetByName("exitbutton"));
    exitBtn->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::toCocosGUISlotScene));
//     */
    
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
	TestScene::MainMenuCallback(pSender);
}

void CocosGUITestScene::toCocosGUISlotScene(CCObject* pSender)
{    
    CocosGUISlotScene *pScene = new CocosGUISlotScene();
    if (pScene)
    {
        ul->dispose();
        
        pScene->runThisTest();
        pScene->release();
    }
}