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

#include "UICheckBox.h"

NS_CC_EXT_BEGIN

UICheckBox::UICheckBox():
m_pBackGroundBox(NULL),
m_pBackGroundSelectedBox(NULL),
m_pFrontCross(NULL),
m_bIsSelected(true),
m_pSelectListener(NULL),
m_pfnSelectSelector(NULL),
m_pUnSelectListener(NULL),
m_pfnUnSelectSelector(NULL),
m_pBackGroundBoxDisabled(NULL),
m_pFrontCrossDisabled(NULL)
{

}

UICheckBox::~UICheckBox()
{
    
}

UICheckBox* UICheckBox::create()
{
    UICheckBox* widget = new UICheckBox();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UICheckBox::init()
{
    if (UIWidget::init())
    {
        this->setPressState(WidgetStateNormal);
        this->setSelectedState(false);
        return true;
    }
    return false;
}

void UICheckBox::initNodes()
{
    UIWidget::initNodes();
    this->m_pBackGroundBox = cocos2d::CCSprite::create();
    this->m_pBackGroundSelectedBox = cocos2d::CCSprite::create();
    this->m_pFrontCross = cocos2d::CCSprite::create();
    this->m_pBackGroundBoxDisabled = cocos2d::CCSprite::create();
    this->m_pFrontCrossDisabled = cocos2d::CCSprite::create();
    this->m_pCCRenderNode->addChild(m_pBackGroundBox);
    this->m_pCCRenderNode->addChild(m_pBackGroundSelectedBox);
    this->m_pCCRenderNode->addChild(m_pFrontCross);
    this->m_pCCRenderNode->addChild(m_pBackGroundBoxDisabled);
    this->m_pCCRenderNode->addChild(m_pFrontCrossDisabled);
}

void UICheckBox::setTextures(const char *backGround, const char *backGroundSelected, const char *cross,const char* backGroundDisabled,const char* frontCrossDisabled,bool useSpriteFrame)
{
    this->setBackGroundTexture(backGround,useSpriteFrame);
    this->setBackGroundSelectedTexture(backGroundSelected,useSpriteFrame);
    this->setFrontCrossTexture(cross,useSpriteFrame);
    this->setBackGroundDisabledTexture(backGroundDisabled,useSpriteFrame);
    this->setFrontCrossDisabledTexture(frontCrossDisabled,useSpriteFrame);
}

void UICheckBox::setBackGroundTexture(const char *backGround,bool useSpriteFrame)
{
    if (useSpriteFrame)
    {
        this->m_pBackGroundBox->initWithSpriteFrameName(backGround);
    }
    else
    {
        this->m_pBackGroundBox->initWithFile(backGround);
    }
}

void UICheckBox::setBackGroundSelectedTexture(const char *backGroundSelected,bool useSpriteFrame)
{
    if (useSpriteFrame)
    {
        this->m_pBackGroundSelectedBox->initWithSpriteFrameName(backGroundSelected);
    }
    else
    {
        this->m_pBackGroundSelectedBox->initWithFile(backGroundSelected);
    }
}

void UICheckBox::setFrontCrossTexture(const char *cross,bool useSpriteFrame)
{
    if (useSpriteFrame)
    {
        this->m_pFrontCross->initWithSpriteFrameName(cross);
    }
    else
    {
        this->m_pFrontCross->initWithFile(cross);
    }
}

void UICheckBox::setBackGroundDisabledTexture(const char *backGroundDisabled,bool useSpriteFrame)
{
    if (useSpriteFrame)
    {
        this->m_pBackGroundBoxDisabled->initWithSpriteFrameName(backGroundDisabled);
    }
    else
    {
        this->m_pBackGroundBoxDisabled->initWithFile(backGroundDisabled);
    }
}

void UICheckBox::setFrontCrossDisabledTexture(const char *frontCrossDisabled,bool useSpriteFrame)
{
    if (useSpriteFrame)
    {
        this->m_pFrontCrossDisabled->initWithSpriteFrameName(frontCrossDisabled);
    }
    else
    {
        this->m_pFrontCrossDisabled->initWithFile(frontCrossDisabled);
    }
}

bool UICheckBox::onTouchEnded(cocos2d::CCPoint &touchPoint)
{
    if (this->m_bFocus)
    {
        this->releaseUpEvent();
        if (this->m_bIsSelected){
            this->setSelectedState(false);
            this->unSelectedEvent();
        }
        else
        {
            this->setSelectedState(true);
            this->selectedEvent();
        }
    }
    this->setFocus(false);
    this->m_pWidgetParent->checkChildInfo(2,this,touchPoint);
    return true;
}

void UICheckBox::onPressStateChangedToNormal()
{
    this->m_pBackGroundBox->setVisible(true);
    this->m_pBackGroundSelectedBox->setVisible(false);
    this->m_pBackGroundBoxDisabled->setVisible(false);
    this->m_pFrontCrossDisabled->setVisible(false);
}

void UICheckBox::onPressStateChangedToPressed()
{
    this->m_pBackGroundBox->setVisible(false);
    this->m_pBackGroundSelectedBox->setVisible(true);
    this->m_pBackGroundBoxDisabled->setVisible(false);
    this->m_pFrontCrossDisabled->setVisible(false);
}

void UICheckBox::onPressStateChangedToDisabled()
{
    this->m_pBackGroundBox->setVisible(false);
    this->m_pBackGroundSelectedBox->setVisible(false);
    this->m_pBackGroundBoxDisabled->setVisible(true);
    this->m_pFrontCross->setVisible(false);
    if (this->m_bIsSelected)
    {
        this->m_pFrontCrossDisabled->setVisible(true);
    }
}

void UICheckBox::setSelectedState(bool selected)
{
    if (selected == this->m_bIsSelected)
    {
        return;
    }
    this->m_bIsSelected = selected;
    this->m_pFrontCross->setVisible(this->m_bIsSelected);
}

bool UICheckBox::getSelectedState()
{
    return this->m_bIsSelected;
}

void UICheckBox::selectedEvent()
{
    if (m_pSelectListener && m_pfnSelectSelector)
    {
        (m_pSelectListener->*m_pfnSelectSelector)(this);
    }
}

void UICheckBox::unSelectedEvent()
{
    if (m_pUnSelectListener && m_pfnUnSelectSelector)
    {
        (m_pUnSelectListener->*m_pfnUnSelectSelector)(this);
    }
}

cocos2d::CCNode* UICheckBox::getValidNode()
{
    return this->m_pBackGroundBox;
}

void UICheckBox::addSelectEvent(cocos2d::CCObject *target, SEL_SelectEvent selector)
{
    this->m_pSelectListener = target;
    this->m_pfnSelectSelector = selector;
}

void UICheckBox::addUnSelectEvent(cocos2d::CCObject *target, SEL_UnSelectEvent selector)
{
    this->m_pUnSelectListener = target;
    this->m_pfnUnSelectSelector = selector;
}

void UICheckBox::setFlipX(bool flipX)
{
    this->m_pBackGroundBox->setFlipX(flipX);
    this->m_pBackGroundSelectedBox->setFlipX(flipX);
    this->m_pFrontCross->setFlipX(flipX);
    this->m_pBackGroundBoxDisabled->setFlipX(flipX);
    this->m_pFrontCrossDisabled->setFlipX(flipX);
}

void UICheckBox::setFlipY(bool flipY)
{
    this->m_pBackGroundBox->setFlipX(flipY);
    this->m_pBackGroundSelectedBox->setFlipY(flipY);
    this->m_pFrontCross->setFlipY(flipY);
    this->m_pBackGroundBoxDisabled->setFlipY(flipY);
    this->m_pFrontCrossDisabled->setFlipY(flipY);
}

void UICheckBox::setAnchorPoint(const cocos2d::CCPoint &pt)
{
    UIWidget::setAnchorPoint(pt);
    this->m_pBackGroundBox->setAnchorPoint(pt);
    this->m_pBackGroundSelectedBox->setAnchorPoint(pt);
    this->m_pBackGroundBoxDisabled->setAnchorPoint(pt);
    this->m_pFrontCross->setAnchorPoint(pt);
    this->m_pFrontCrossDisabled->setAnchorPoint(pt);
}

NS_CC_EXT_END