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

#include "UITextButton.h"

NS_CC_EXT_BEGIN

UITextButton::UITextButton():
m_pTextLable(NULL)
{
    m_WidgetName = WIDGET_TEXTBUTTON;
}

UITextButton::~UITextButton()
{
    
}

UITextButton* UITextButton::create()
{
    UITextButton* widget = new UITextButton();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UITextButton::init()
{
    if (UIButton::init())
    {
        this->m_pTextLable = cocos2d::CCLabelTTF::create();
        this->m_pCCRenderNode->addChild(this->m_pTextLable);
        return true;
    }
    return false;
}

void UITextButton::setText(const char* text)
{
	if (!text)
	{
		return;
	}
    std::string strText(text);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	const char *des = UTF8ToGBK(strText.c_str());
//	if (des == NULL)
//	{
//		return;
//	}
//	strText.assign(des);
//	CC_SAFE_DELETE(des);
//#endif
    this->m_pTextLable->setString(strText.c_str());
}

void UITextButton::setTextColor(int r,int g,int b)
{
    this->m_pTextLable->setColor(cocos2d::ccc3(r, g, b));
}

void UITextButton::setFontSize(int size)
{
    this->m_pTextLable->setFontSize(size);
}

void UITextButton::setFontName(const char *fontName)
{
    this->m_pTextLable->setFontName(fontName);
}

void UITextButton::setFlipX(bool flipX)
{
    UIButton::setFlipX(flipX);
    this->m_pTextLable->setFlipX(flipX);
}

void UITextButton::setFlipY(bool flipY)
{
    UIButton::setFlipY(flipY);
    this->m_pTextLable->setFlipY(flipY);
}

void UITextButton::setAnchorPoint(const cocos2d::CCPoint &pt)
{
    UIButton::setAnchorPoint(pt);
    this->m_pTextLable->setPosition(ccp(this->m_pButtonNormal->getContentSize().width*(0.5f-this->m_pButtonNormal->getAnchorPoint().x), this->m_pButtonNormal->getContentSize().height*(0.5f-this->m_pButtonNormal->getAnchorPoint().y)));
}

NS_CC_EXT_END