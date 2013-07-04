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

#include "UIButton.h"
#include "../../GUI/CCControlExtension/CCScale9Sprite.h"

NS_CC_EXT_BEGIN
    
UIButton::UIButton():
m_pButtonNormal(NULL),
m_pButtonClicked(NULL),
m_pButtonDisable(NULL),
m_bScale9Enable(false),
m_strClickedFileName(""),
m_strDisabledFileName(""),
m_strNormalFileName("")
{
    m_WidgetName = WIDGET_BUTTON;
}

UIButton::~UIButton()
{
    
}

UIButton* UIButton::create()
{
    UIButton* widget = new UIButton();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UIButton::init()
{
    if (UIWidget::init())
    {
        
        this->initPressState(WidgetStateNormal);
        return true;
    }
    return false;
}

void UIButton::initNodes()
{
    UIWidget::initNodes();
    if (this->m_bScale9Enable)
    {
        this->m_pButtonNormal = cocos2d::extension::CCScale9Sprite::create();
        this->m_pButtonClicked = cocos2d::extension::CCScale9Sprite::create();
        this->m_pButtonDisable = cocos2d::extension::CCScale9Sprite::create();
    }
    else
    {
        this->m_pButtonNormal = cocos2d::CCSprite::create();
        this->m_pButtonClicked = cocos2d::CCSprite::create();
        this->m_pButtonDisable = cocos2d::CCSprite::create();
    }
    this->m_pRender->addChild(this->m_pButtonNormal);
    this->m_pRender->addChild(this->m_pButtonClicked);
    this->m_pRender->addChild(this->m_pButtonDisable);
}

void UIButton::initPressState(WidgetState state)
{
    UIWidget::initPressState(state);
    this->m_pButtonNormal->setVisible(true);
    this->m_pButtonClicked->setVisible(false);
    this->m_pButtonDisable->setVisible(false);
}

void UIButton::setScale9Enable(bool able)
{
    if (this->m_bScale9Enable == able)
    {
        return;
    }
    this->m_nPrevPressstate = WidgetStateNone;
    this->m_nCurPressState = WidgetStateNone;
    this->m_bScale9Enable = able;
    
    this->m_pRender->removeChild(this->m_pButtonNormal, true);
    this->m_pRender->removeChild(this->m_pButtonClicked, true);
    this->m_pRender->removeChild(this->m_pButtonDisable, true);
    
    this->m_pButtonNormal = NULL;
    this->m_pButtonClicked = NULL;
    this->m_pButtonDisable = NULL;
    if (this->m_bScale9Enable)
    {
        this->m_pButtonNormal = cocos2d::extension::CCScale9Sprite::create();
        this->m_pButtonClicked = cocos2d::extension::CCScale9Sprite::create();
        this->m_pButtonDisable = cocos2d::extension::CCScale9Sprite::create();
    }
    else
    {
        this->m_pButtonNormal = cocos2d::CCSprite::create();
        this->m_pButtonClicked = cocos2d::CCSprite::create();
        this->m_pButtonDisable = cocos2d::CCSprite::create();
    }
    setTextures(m_strNormalFileName.c_str(), m_strClickedFileName.c_str(), m_strDisabledFileName.c_str(),getUseMergedTexture());
    this->m_pRender->addChild(this->m_pButtonNormal);
    this->m_pRender->addChild(this->m_pButtonClicked);
    this->m_pRender->addChild(this->m_pButtonDisable);
    this->initPressState(WidgetStateNormal);
}

void UIButton::setScale9Size(float width, float height)
{
    if (!this->m_bScale9Enable)
    {
        return;
    }
    cocos2d::CCSize size = cocos2d::CCSize(width,height);
    dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonNormal)->setContentSize(size);
    dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonClicked)->setContentSize(size);
    dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonDisable)->setContentSize(size);
}

void UIButton::setTextures(const char* normal,const char* selected,const char* disabled,bool useSpriteFrame)
{
    this->setNormalTexture(normal,useSpriteFrame);
    this->setPressedTexture(selected,useSpriteFrame);
    this->setDisabledTexture(disabled,useSpriteFrame);
}

void UIButton::setNormalTexture(const char* normal,bool useSpriteFrame)
{
    if (!normal || strcmp(normal, "") == 0)
    {
        return;
    }
    m_strNormalFileName = normal;
    setUseMergedTexture(useSpriteFrame);
    if (m_bScale9Enable)
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonNormal)->initWithSpriteFrameName(normal);
        }
        else
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonNormal)->initWithFile(normal);
        }
    }
    else
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonNormal)->initWithSpriteFrameName(normal);
        }
        else
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonNormal)->initWithFile(normal);
        }
    }
    this->updateAnchorPoint();
}

void UIButton::setPressedTexture(const char* selected,bool useSpriteFrame)
{
    if (!selected || strcmp(selected, "") == 0)
    {
        return;
    }
    m_strClickedFileName = selected;
    setUseMergedTexture(useSpriteFrame);
    if (m_bScale9Enable)
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonClicked)->initWithSpriteFrameName(selected);
        }
        else
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonClicked)->initWithFile(selected);
        }
    }
    else
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonClicked)->initWithSpriteFrameName(selected);
        }
        else
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonClicked)->initWithFile(selected);
        }
    }
    this->updateAnchorPoint();
}

void UIButton::setDisabledTexture(const char* disabled,bool useSpriteFrame)
{
    if (!disabled || strcmp(disabled, "") == 0)
    {
        return;
    }
    m_strDisabledFileName = disabled;
    setUseMergedTexture(useSpriteFrame);
    if (m_bScale9Enable)
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonDisable)->initWithSpriteFrameName(disabled);
        }
        else
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonDisable)->initWithFile(disabled);
        }
    }
    else
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonDisable)->initWithSpriteFrameName(disabled);
        }
        else
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonDisable)->initWithFile(disabled);
        }
    }

    this->updateAnchorPoint();
}

void UIButton::setCapInsets(const cocos2d::CCRect &capInset)
{
    if (!m_bScale9Enable)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonNormal)->setCapInsets(capInset);
    dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonClicked)->setCapInsets(capInset);
    dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pButtonDisable)->setCapInsets(capInset);
}

void UIButton::onPressStateChangedToNormal()
{
    this->m_pButtonNormal->setVisible(true);
    this->m_pButtonClicked->setVisible(false);
    this->m_pButtonDisable->setVisible(false);
}

void UIButton::onPressStateChangedToPressed()
{
    this->m_pButtonNormal->setVisible(false);
    this->m_pButtonClicked->setVisible(true);
    this->m_pButtonDisable->setVisible(false);
}

void UIButton::onPressStateChangedToDisabled()
{
    this->m_pButtonNormal->setVisible(false);
    this->m_pButtonClicked->setVisible(false);
    this->m_pButtonDisable->setVisible(true);
}

cocos2d::CCNode* UIButton::getValidNode()
{
    cocos2d::CCNode* res = NULL;
    switch (this->m_nCurPressState)
    {
        case WidgetStateNormal:
            res = this->m_pButtonNormal;
            break;
        case WidgetStateSelected:
            res = this->m_pButtonClicked;
            break;
        case WidgetStateDisabled:
            res = this->m_pButtonDisable;
            break;
        default:
            break;
    }
    return res;
}

void UIButton::setFlipX(bool flipX)
{
    dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonNormal)->setFlipX(flipX);
    dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonClicked)->setFlipX(flipX);
    dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonDisable)->setFlipX(flipX);
}

void UIButton::setFlipY(bool flipY)
{
    dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonNormal)->setFlipY(flipY);
    dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonClicked)->setFlipY(flipY);
    dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonDisable)->setFlipY(flipY);
}

bool UIButton::isFlipX()
{
    return dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonNormal)->isFlipX();
}

bool UIButton::isFlipY()
{
    return dynamic_cast<cocos2d::CCSprite*>(this->m_pButtonNormal)->isFlipY();
}

void UIButton::setAnchorPoint(const cocos2d::CCPoint &pt)
{
    UIWidget::setAnchorPoint(pt);
    this->m_pButtonNormal->setAnchorPoint(pt);
    this->m_pButtonClicked->setAnchorPoint(pt);
    this->m_pButtonDisable->setAnchorPoint(pt);
}

void UIButton::setNormalSpriteFrame(cocos2d::CCSpriteFrame *frame)
{
    if (!frame)
    {
        return;
    }
    if (m_bScale9Enable)
    {
        dynamic_cast<cocos2d::extension::CCScale9Sprite*>(m_pButtonNormal)->setSpriteFrame(frame);
    }
    else
    {
        dynamic_cast<cocos2d::CCSprite*>(m_pButtonNormal)->setDisplayFrame(frame);
    }
}

void UIButton::setPressedSpriteFrame(cocos2d::CCSpriteFrame *frame)
{
    if (!frame)
    {
        return;
    }
    if (m_bScale9Enable)
    {
        dynamic_cast<cocos2d::extension::CCScale9Sprite*>(m_pButtonClicked)->setSpriteFrame(frame);
    }
    else
    {
        dynamic_cast<cocos2d::CCSprite*>(m_pButtonClicked)->setDisplayFrame(frame);
    }
}

void UIButton::setDisabledSpriteFrame(cocos2d::CCSpriteFrame *frame)
{
    if (!frame)
    {
        return;
    }
    if (m_bScale9Enable)
    {
        dynamic_cast<cocos2d::extension::CCScale9Sprite*>(m_pButtonDisable)->setSpriteFrame(frame);
    }
    else
    {
        dynamic_cast<cocos2d::CCSprite*>(m_pButtonDisable)->setDisplayFrame(frame);
    }
}

NS_CC_EXT_END