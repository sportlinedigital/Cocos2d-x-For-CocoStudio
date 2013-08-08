/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "UISlider.h"
#include "../../GUI/CCControlExtension/CCScale9Sprite.h"

NS_CC_EXT_BEGIN

UISlider::UISlider():
m_fBarLength(0.0),
m_nDirection(0),
m_nBarPercent(0),
m_pSlidBall(NULL),
m_fBarNodeScaleValue(1.0),
m_fTouchMoveStartLocation(0.0),
m_fMinLength(0.0),
m_pBarNode(NULL),
m_pPercentListener(NULL),
m_pfnPercentSelector(NULL),
m_bBarScale9Enable(false),
m_bProgressBarVisible(false),
m_pProgressBarNode(NULL),
m_eBarTexType(UI_TEX_TYPE_LOCAL),
m_eBarTexS9Type(UI_TEX_TYPE_LOCAL),
m_eBallNTexType(UI_TEX_TYPE_LOCAL),
m_eBallPTexType(UI_TEX_TYPE_LOCAL),
m_eBallDTexType(UI_TEX_TYPE_LOCAL),
m_strTextureFile(""),
m_capInsets(CCRectZero),
m_scale9Size(CCSizeZero)
{
    m_WidgetName = WIDGET_SLIDER;
}

UISlider::~UISlider()
{
    
}

UISlider* UISlider::create()
{
    UISlider* widget = new UISlider();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UISlider::init()
{
    if (UIWidget::init())
    {
        m_pBarNode = CCSprite::create();
        m_pRender->addChild(m_pBarNode, -1);
        m_pSlidBall = UIButton::create();
		m_pSlidBall->setWidgetZOrder(1);
        addChild(m_pSlidBall);
        return true;
    }
    return false;
}

void UISlider::setBarTexture(const char* fileName, TextureResType texType)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    m_strTextureFile = fileName;
    m_eBarTexType = texType;
    switch (m_eBarTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            if (m_bBarScale9Enable)
            {
                dynamic_cast<CCScale9Sprite*>(m_pBarNode)->initWithFile(fileName);
            }
            else
            {
                dynamic_cast<CCSprite*>(m_pBarNode)->initWithFile(fileName);
            }
            break;
        case UI_TEX_TYPE_PLIST:
            if (m_bBarScale9Enable)
            {
                dynamic_cast<CCScale9Sprite*>(m_pBarNode)->initWithSpriteFrameName(fileName);
            }
            else
            {
                dynamic_cast<CCSprite*>(m_pBarNode)->initWithSpriteFrameName(fileName);
            }
            break;
        default:
            break;
    }
    if (m_bBarScale9Enable)
    {
        dynamic_cast<CCScale9Sprite*>(m_pBarNode)->setColor(getColor());
        dynamic_cast<CCScale9Sprite*>(m_pBarNode)->setOpacity(getOpacity());
    }
    else
    {
        dynamic_cast<CCSprite*>(m_pBarNode)->setColor(getColor());
        dynamic_cast<CCSprite*>(m_pBarNode)->setOpacity(getOpacity());
    }
    
    m_fBarLength = m_pBarNode->getContentSize().width;
    setSlidBallPercent(m_nBarPercent);
}

void UISlider::setScale9Enable(bool able)
{
    if (m_bBarScale9Enable == able)
    {
        return;
    }
    
    m_bBarScale9Enable = able;
    m_pRender->removeChild(m_pBarNode, true);
    m_pBarNode = NULL;
    if (m_bBarScale9Enable)
    {
        m_pBarNode = CCScale9Sprite::create();
    }
    else
    {
        m_pBarNode = CCSprite::create();
    }
    setBarTexture(m_strTextureFile.c_str(), m_eBarTexType);
    setCapInsets(m_capInsets);
    setScale9Size(m_scale9Size);
    m_pRender->addChild(m_pBarNode, -1);
}

void UISlider::setCapInsets(const CCRect &capInsets)
{
    m_capInsets = capInsets;
    if (!m_bBarScale9Enable)
    {
        return;
    }
    dynamic_cast<CCScale9Sprite*>(m_pBarNode)->setCapInsets(capInsets);
}

void UISlider::setScale9Size(const CCSize &size)
{
    if (size.equals(CCSizeZero))
    {
        return;
    }
    else
    {
        m_scale9Size = size;
    }
    if (!m_bBarScale9Enable)
    {
        return;
    }
    dynamic_cast<CCScale9Sprite*>(m_pBarNode)->setContentSize(size);
    m_fBarLength = m_pBarNode->getContentSize().width;
    setSlidBallPercent(m_nBarPercent);
}

void UISlider::setSlidBallTextures(const char* normal,const char* pressed,const char* disabled,TextureResType texType)
{
    m_pSlidBall->setTextures(normal, pressed, disabled,texType);
}

void UISlider::setSlidBallNormalTexture(const char* normal,TextureResType texType)
{
    m_pSlidBall->setNormalTexture(normal,texType);
}

void UISlider::setSlidBallPressedTexture(const char* pressed,TextureResType texType)
{
    m_pSlidBall->setPressedTexture(pressed,texType);
}

void UISlider::setSlidBallDisabledTexture(const char* disabled,TextureResType texType)
{
    m_pSlidBall->setDisabledTexture(disabled,texType);
}

void UISlider::setBarLength(float length)
{
    if (!m_bBarScale9Enable)
    {
        return;
    }
    m_fBarLength = length;
    dynamic_cast<CCScale9Sprite*>(m_pBarNode)->setContentSize(CCSize(length, m_pBarNode->getContentSize().height));
    setSlidBallPercent(m_nBarPercent);
}

void UISlider::setSlidBallPercent(int percent)
{
    m_nBarPercent = percent;
    float dis = m_fBarLength*(percent/100.0);
    m_pSlidBall->setPosition(ccp(-m_fBarLength/2.0+dis,0));
}

int UISlider::getClickPercent(float location)
{
    getLocationInWindow();
    float leftEndPos = m_locationInWindow.x - m_fBarLength/2.0;
    float per = (location - leftEndPos)/m_fBarLength;
    per *= 100;
    return per;
}

void UISlider::checkSlidBoundary()
{
    if (m_pSlidBall->getPosition().x > m_fBarLength/2.0)
    {
        m_pSlidBall->setPosition(ccp(m_fBarLength/2.0,0));
    }
    else if(m_pSlidBall->getPosition().x < -m_fBarLength/2.0)
    {
        m_pSlidBall->setPosition(ccp(-m_fBarLength/2.0,0));
    }
}

void UISlider::setProgressBarVisible(bool show)
{
    if (m_bProgressBarVisible == show)
    {
        return;
    }
    m_bProgressBarVisible = show;
    
    if (m_bProgressBarVisible)
    {
        if (m_bBarScale9Enable)
        {
            m_pProgressBarNode = CCScale9Sprite::create();
        }
        else
        {
            m_pProgressBarNode = CCSprite::create();
        }
        m_pRender->addChild(m_pProgressBarNode, m_pBarNode->getZOrder() + 1);
        m_pSlidBall->setWidgetZOrder(m_pProgressBarNode->getZOrder() + 1);
    }
    else
    {
        if (m_pProgressBarNode != NULL)
        {
            m_pRender->removeChild(m_pProgressBarNode, true);
        }
    }
}

void UISlider::setProgressBarTexture(const char *fileName, TextureResType texType)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    switch (m_eBarTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            if (m_bBarScale9Enable)
            {
                dynamic_cast<CCScale9Sprite*>(m_pProgressBarNode)->initWithFile(fileName);
            }
            else
            {
                dynamic_cast<CCSprite*>(m_pProgressBarNode)->initWithFile(fileName);
            }
            break;
        case UI_TEX_TYPE_PLIST:
            if (m_bBarScale9Enable)
            {
                dynamic_cast<CCScale9Sprite*>(m_pProgressBarNode)->initWithSpriteFrameName(fileName);
            }
            else
            {
                dynamic_cast<CCSprite*>(m_pProgressBarNode)->initWithSpriteFrameName(fileName);
            }
            break;
        default:
            break;
    }
    if (m_bBarScale9Enable)
    {
        dynamic_cast<CCScale9Sprite*>(m_pProgressBarNode)->setColor(getColor());
        dynamic_cast<CCScale9Sprite*>(m_pProgressBarNode)->setOpacity(getOpacity());
    }
    else
    {
        dynamic_cast<CCSprite*>(m_pProgressBarNode)->setColor(getColor());
        dynamic_cast<CCSprite*>(m_pProgressBarNode)->setOpacity(getOpacity());
    }
    m_pProgressBarNode->setAnchorPoint(ccp(0.0, 0.5));
    m_pProgressBarNode->setPosition(ccp(m_pBarNode->getPosition().x - m_pBarNode->getContentSize().width / 2, m_pBarNode->getPosition().y - 1));
    m_pProgressBarNode->setContentSize(CCSizeMake(m_pBarNode->getContentSize().width, m_pBarNode->getContentSize().height));
    setProgressBarScale();
}

void UISlider::setProgressBarScale()
{
    float width = (float)(m_nBarPercent) / 100 * m_fBarLength;
    
    if (m_bBarScale9Enable)
    {
        m_pProgressBarNode->setContentSize(CCSize(width, m_pProgressBarNode->getContentSize().height));
    }
    else
    {
        dynamic_cast<CCSprite*>(m_pProgressBarNode)->setTextureRect(CCRectMake(0, 0, width, m_pProgressBarNode->getContentSize().height));
    }
}

void UISlider::onTouchBegan(const CCPoint &touchPoint)
{
    UIWidget::onTouchBegan(touchPoint);
    CCPoint nsp = m_pRender->convertToNodeSpace(touchPoint);
    m_pSlidBall->setPosition(ccp(nsp.x,0));
    m_pSlidBall->setPressState(WidgetStateSelected);
    m_nBarPercent = getPercentWithBallPos(m_pSlidBall->getPosition().x,0);
    percentChangedEvent();
    if (m_bProgressBarVisible)
    {
        setProgressBarScale();
    }
}

void UISlider::onTouchMoved(const CCPoint &touchPoint)
{
    CCPoint nsp = m_pRender->convertToNodeSpace(touchPoint);
    m_pSlidBall->setPosition(ccp(nsp.x,0));
    checkSlidBoundary();
    m_nBarPercent = getPercentWithBallPos(m_pSlidBall->getPosition().x,0);
    percentChangedEvent();
    if (m_bProgressBarVisible)
    {
        setProgressBarScale();
    }
}

void UISlider::onTouchEnded(const CCPoint &touchPoint)
{
    UIWidget::onTouchEnded(touchPoint);
    m_pSlidBall->setPressState(WidgetStateNormal);
}

void UISlider::onTouchCancelled(const CCPoint &touchPoint)
{
    UIWidget::onTouchCancelled(touchPoint);
    m_pSlidBall->setPressState(WidgetStateNormal);
}

float UISlider::getPercentWithBallPos(float px,float py)
{
    return (((px-(-m_fBarLength/2.0))/m_fBarLength)*100);
}

bool UISlider::pointAtSelfBody(const CCPoint &pt)
{
    if (!getAbsoluteVisible())
    {
        return false;
    }
    if (UIWidget::pointAtSelfBody(pt))
    {
        return true;
    }
    
    bool hitBall = hitTest(m_pSlidBall->getValidNode(),pt);
    if (hitBall) {
        return true;
    }
    return false;
}

CCNode* UISlider::getValidNode()
{
    return m_pBarNode;
}

void UISlider::addPercentChangedEvent(CCObject *target, SEL_PushEvent selector)
{
    m_pPercentListener = target;
    m_pfnPercentSelector = selector;
}

void UISlider::percentChangedEvent()
{
    if (m_pPercentListener && m_pfnPercentSelector)
    {
        (m_pPercentListener->*m_pfnPercentSelector)(this);
    }
}

int UISlider::getPercent()
{
    return m_nBarPercent;
}

NS_CC_EXT_END