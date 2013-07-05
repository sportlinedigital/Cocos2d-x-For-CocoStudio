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

#include "UIPanel.h"
#include "../../GUI/CCControlExtension/CCScale9Sprite.h"
#include "../Drawable/UIClipAbleLayerColor.h"


NS_CC_EXT_BEGIN

#define DYNAMIC_CAST_CLIPLAYERCOLOR dynamic_cast<UIClipAbleLayerColor*>(this->m_pCCRenderNode)

UIPanel::UIPanel():
m_bBackGroundScale9Enable(false),
m_pBackGroundImage(NULL),
m_strBackGroundImageFileName("")
{
    m_WidgetName = WIDGET_PANEL;
}

UIPanel* UIPanel::create()
{
    UIPanel* widget = new UIPanel();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

void UIPanel::initNodes()
{
    UIContainerWidget::initNodes();
    this->m_pBackGroundImage = cocos2d::CCSprite::create();
    m_pBackGroundImage->setZOrder(-1);
    this->m_pCCRenderNode->addChild(this->m_pBackGroundImage);
}

UIPanel::~UIPanel()
{
    
}

void UIPanel::setBackGroundImageScale9Enable(bool able)
{
    if (this->m_bBackGroundScale9Enable == able)
    {
        return;
    }
    this->m_pCCRenderNode->removeChild(this->m_pBackGroundImage, true);
    this->m_pBackGroundImage = NULL;
    this->m_bBackGroundScale9Enable = able;
    if (m_bBackGroundScale9Enable)
    {
        this->m_pBackGroundImage = CCScale9Sprite::create();
        this->m_pCCRenderNode->addChild(this->m_pBackGroundImage);
    }
    else
    {
        this->m_pBackGroundImage = CCSprite::create();
        this->m_pCCRenderNode->addChild(this->m_pBackGroundImage);
    }
    m_pBackGroundImage->setZOrder(-1);
    setBackGroundImage(m_strBackGroundImageFileName.c_str(),getUseMergedTexture());
}
    
void UIPanel::setSize(const cocos2d::CCSize &size)
{
    UIContainerWidget::setSize(size);
    this->m_pBackGroundImage->setPosition(ccp(this->m_pCCRenderNode->getContentSize().width/2.0f, this->m_pCCRenderNode->getContentSize().height/2.0f));
    if (this->m_bBackGroundScale9Enable)
    {
        dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pBackGroundImage)->setContentSize(this->m_pCCRenderNode->getContentSize());
    }
}

void UIPanel::setBackGroundImage(const char* fileName,bool useSpriteFrame)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    m_strBackGroundImageFileName = fileName;
    setUseMergedTexture(useSpriteFrame);
    if (this->m_bBackGroundScale9Enable)
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pBackGroundImage)->initWithSpriteFrameName(fileName);
        }
        else
        {
            dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pBackGroundImage)->initWithFile(fileName);
        }
        dynamic_cast<cocos2d::extension::CCScale9Sprite*>(this->m_pBackGroundImage)->setContentSize(this->m_pCCRenderNode->getContentSize());
    }
    else
    {
        if (useSpriteFrame)
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pBackGroundImage)->initWithSpriteFrameName(fileName);
        }
        else
        {
            dynamic_cast<cocos2d::CCSprite*>(this->m_pBackGroundImage)->initWithFile(fileName);
        }
    }
    this->m_pBackGroundImage->setPosition(ccp(this->m_pCCRenderNode->getContentSize().width/2, this->m_pCCRenderNode->getContentSize().height/2));
}

void UIPanel::setBackGroundImageCapInset(const cocos2d::CCRect &capInset)
{
    
}

void UIPanel::setBackGroundColorEnable(bool able)
{
    DYNAMIC_CAST_CLIPLAYERCOLOR->setColorEnable(able);
}

NS_CC_EXT_END