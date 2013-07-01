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

#include "UIPageView.h"

NS_CC_EXT_BEGIN

UIPageView::UIPageView()
{
    
}

UIPageView::~UIPageView()
{
    m_pages->release();
}

UIPageView* UIPageView::create()
{
    UIPageView* widget = new UIPageView();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UIPageView::init()
{
    if (UIPanel::init()) {
        m_pages = CCArray::create();
        m_pages->retain();
        return true;
    }
    return false;
}

void UIPageView::addWidgetToPage(UIWidget *widget, int pageIdx, bool forceCreate)
{
    
}

void UIPageView::addPage(UIContainerWidget* page)
{
    
}

void UIPageView::insertPage(UIContainerWidget* page, int idx)
{
    
}

void UIPageView::removePage(UIContainerWidget* page, bool cleanup)
{
    
}

bool UIPageView::addChild(UIWidget* widget)
{
    return UIPanel::addChild(widget);
}

bool UIPageView::removeChild(UIWidget* widget, bool cleanup)
{
    
}

void UIPageView::removeAllChildrenAndCleanUp(bool cleanup)
{
    
}

bool UIPageView::onTouchBegan(cocos2d::CCPoint &touchPoint)
{
    
}

bool UIPageView::onTouchMoved(cocos2d::CCPoint &touchPoint)
{
    
}

bool UIPageView::onTouchEnded(cocos2d::CCPoint &touchPoint)
{
    
}

bool UIPageView::onTouchCancelled(cocos2d::CCPoint &touchPoint)
{
    
}

NS_CC_EXT_END