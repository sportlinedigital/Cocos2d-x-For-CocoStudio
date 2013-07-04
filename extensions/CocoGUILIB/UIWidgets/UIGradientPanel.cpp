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

#include "UIGradientPanel.h"
#include "../Drawable/UIClipAbleLayerGradient.h"

NS_CC_EXT_BEGIN

#define DYNAMIC_CAST_CLIPLAYERGRADIENT dynamic_cast<UIClipAbleLayerGradient*>(this->m_pRender)

UIGradientPanel::UIGradientPanel()
{
    m_renderType = RENDER_TYPE_LAYERGRADIENT;
    m_WidgetName = WIDGET_GRADIENTPANEL;
}

UIGradientPanel* UIGradientPanel::create()
{
    UIGradientPanel* widget = new UIGradientPanel();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

UIGradientPanel::~UIGradientPanel()
{
    
}

void UIGradientPanel::initNodes()
{
    this->m_pRender = UIClipAbleLayerGradient::create();
}

void UIGradientPanel::setStartColor(const cocos2d::ccColor3B &color)
{
    DYNAMIC_CAST_CLIPLAYERGRADIENT->setStartColor(color);
}

void UIGradientPanel::setEndColor(const cocos2d::ccColor3B &color)
{
    DYNAMIC_CAST_CLIPLAYERGRADIENT->setEndColor(color);
}

void UIGradientPanel::setBackGroundColorEnable(bool able)
{
    DYNAMIC_CAST_CLIPLAYERGRADIENT->setColorEnable(able);
}

void UIGradientPanel::setStartOpacity(int opacity)
{
    DYNAMIC_CAST_CLIPLAYERGRADIENT->setStartOpacity(opacity);
}

void UIGradientPanel::setEndOpacity(int opacity)
{
    DYNAMIC_CAST_CLIPLAYERGRADIENT->setEndOpacity(opacity);
}

void UIGradientPanel::setVector(const CCPoint &vector)
{
    DYNAMIC_CAST_CLIPLAYERGRADIENT->setVector(vector);
}

NS_CC_EXT_END