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

#include "UIWrapPanel.h"

NS_CC_EXT_BEGIN

UIWrapPanel::UIWrapPanel()
: m_fLeftBoundary(0.0f)
, m_fTopBoundary(0.0f)
, m_fRightBoundary(0.0f)
, m_fBottomBoundary(0.0f)
{
    
}

UIWrapPanel::~UIWrapPanel()
{
    
}

UIWrapPanel* UIWrapPanel::create()
{
    UIWrapPanel* widget = new UIWrapPanel();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UIWrapPanel::init()
{
    if (UIPanel::init())
    {
        return true;
    }
    return false;
}

bool UIWrapPanel::addChild(UIWidget *widget)
{
    UIPanel::addChild(widget);
    
    updateWidthAndHeight();
    return true;
}

void UIWrapPanel::removeAllChildrenAndCleanUp(bool cleanup)
{
    UIPanel::removeAllChildrenAndCleanUp(cleanup);
    setSize(CCSizeMake(0, 0));
}

void UIWrapPanel::removeChildMoveToTrash(UIWidget *child)
{
    UIPanel::removeChildMoveToTrash(child);
    updateWidthAndHeight();
}

void UIWrapPanel::removeChildReferenceOnly(UIWidget* child)
{
    UIPanel::removeChildReferenceOnly(child);
    updateWidthAndHeight();
}

void UIWrapPanel::updateWidthAndHeight()
{
    if (m_children->count() <= 0)
    {
        return;
    }
    
    UIWidget* leftChild = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    UIWidget* rightChild = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    UIWidget* topChild = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    UIWidget* bottomChild = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    
    
    ccArray* arrayChildren = m_children->data;
    int childrenCount = arrayChildren->num;
    for (int i = 0; i < childrenCount; i++)
    {
        UIWidget* child = (UIWidget*)(arrayChildren->arr[i]);
        
        if (leftChild->getRelativeLeftPos() > child->getRelativeLeftPos())
        {
            leftChild = child;
        }
        if (rightChild->getRelativeRightPos() < child->getRelativeRightPos())
        {
            rightChild = child;
        }
        if (topChild->getRelativeTopPos() < child->getRelativeTopPos())
        {
            topChild = child;
        }
        if (bottomChild->getRelativeBottomPos() > child->getRelativeBottomPos())
        {
            bottomChild = child;
        }
    }
    
    m_fLeftBoundary = leftChild->getRelativeLeftPos();
    m_fRightBoundary = rightChild->getRelativeRightPos();
    m_fTopBoundary = topChild->getRelativeTopPos();;
    m_fBottomBoundary = bottomChild->getRelativeBottomPos();;
    
    float resWidth = m_fRightBoundary - m_fLeftBoundary;
    float resHeight = m_fTopBoundary - m_fBottomBoundary;
    setSize(CCSizeMake(resWidth, resHeight));
    
    setPosition(ccp(getPosition().x + m_fLeftBoundary, getPosition().y + m_fBottomBoundary));
    for (int i = 0; i < childrenCount; i++)
    {
        UIWidget* child = (UIWidget*)(arrayChildren->arr[i]);
        child->setPosition(ccp(child->getPosition().x - m_fLeftBoundary, child->getPosition().y - m_fBottomBoundary));
    } 
}

float UIWrapPanel::getLeftBoundary()
{
    return m_fLeftBoundary;
}

float UIWrapPanel::getTopBoundary()
{
    return m_fTopBoundary;
}

float UIWrapPanel::getRightBoundary()
{
    return m_fRightBoundary;
}

float UIWrapPanel::getBottomBoundary()
{
    return m_fBottomBoundary;
}

NS_CC_EXT_END
