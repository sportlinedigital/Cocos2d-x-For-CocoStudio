

#include "UILabelTest.h"

// UILabelTest

bool UILabelTest::init()
{
    if (UIScene::init())
    {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        UILabel *alert = UILabel::create();
        alert->setText("Label");
        alert->setFontName("Marker Felt");
        alert->setFontSize(30);
        alert->setColor(ccc3(159, 168, 176));
        alert->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - alert->getRect().size.height * 1.75));
        m_pUiLayer->addWidget(alert);        
        
        // Create the label
        UILabel* label = UILabel::create();
        label->setText("Label");
        label->setFontName("AmericanTypewriter");
        label->setFontSize(30);
        label->setPosition(ccp(screenSize.width / 2, screenSize.height / 2 + label->getRect().size.height / 4));
        m_pUiLayer->addWidget(label);
        
        return true;
    }
    return false;
}