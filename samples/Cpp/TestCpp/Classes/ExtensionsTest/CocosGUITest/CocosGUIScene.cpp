#include "CocosGUIScene.h"


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
    ul->addWidget(CCUIHELPER->createWidgetFromFile_json("cocosgui/CocoGUISample.json"));
    CocoScrollView* sc = (CocoScrollView*)(ul->getWidgetByName("scrollview"));
    sc->setUpdateEnable(true);
    sc->setMoveMode(SCROLLVIEW_MOVE_MODE_NORMAL);
    
    CocoImageView* image = dynamic_cast<CocoImageView*>(ul->getWidgetByName("imageview"));
    image->setBeTouchAble(true);
//    image->addReleaseEvent(this, coco_releaseselector(CocosGUITestScene::MainMenuCallback));
    
    
}
void CocosGUITestScene::MainMenuCallback(CCObject* pSender)
{
    CCSize size = dynamic_cast<CocoWidget*>(ul->getRootWidget()->getChildren()->lastObject())->getFileDesignSize();
//    static bool is = true;
//    if (is) {
//        ul->setUIType(UILAYER_MODEL);
//    }
//    else{
//        ul->setUIType(UILAYER_SCENE);
//    }
//    is = !is;
    
	TestScene::MainMenuCallback(pSender);
}