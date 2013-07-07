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

#include "UIClippingLayerColor.h"

NS_CC_EXT_BEGIN

UIClippingLayerColor::UIClippingLayerColor():
m_bClippingEnable(false),
m_fScissorX(0.0f),
m_fScissorY(0.0),
m_fScissorWidth(0.0),
m_fScissorHeight(0.0),
m_bEnableCustomArea(false),
m_bColorEnable(false),
m_loacationInWorld(ccp(0, 0)),
m_pClippingParent(NULL),
m_bHandleScissor(true)
{
    
}

UIClippingLayerColor::~UIClippingLayerColor()
{
    
}

UIClippingLayerColor* UIClippingLayerColor::create(const cocos2d::ccColor4B &color,float width,float height)
{
    UIClippingLayerColor * pLayer = new UIClippingLayerColor();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

UIClippingLayerColor* UIClippingLayerColor::create()
{
    UIClippingLayerColor * pLayer = new UIClippingLayerColor();
    if( pLayer && pLayer->init())
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

void UIClippingLayerColor::onEnter()
{
    CCLayerColor::onEnter();
    m_loacationInWorld = convertToWorldSpace(CCPointZero);
    checkClippingOption();
}

void UIClippingLayerColor::onExit()
{
    CCLayerColor::onExit();
    m_pClippingParent = NULL;
    m_bHandleScissor = true;
}

void UIClippingLayerColor::checkClippingOption()
{
    if (!m_bClippingEnable)
    {
        return;
    }
    CCNode* parent = this;
    while (parent)
    {
        parent = parent->getParent();
        if(parent)
        {
            m_pClippingParent = dynamic_cast<UIClippingLayerColor*>(parent);
            if (m_pClippingParent && m_pClippingParent->isClippingEnable())
            {
                m_bHandleScissor = false;
                break;
            }
            else
            {
                m_pClippingParent = NULL;
            }
        }
    }
}

void UIClippingLayerColor::visit()
{
    if (m_bClippingEnable)
    {
        if (m_bHandleScissor)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        
        if (m_bEnableCustomArea)
        {
            CCRect clippingRect = getClippingRect();
            CCEGLView::sharedOpenGLView()->setScissorInPoints(clippingRect.origin.x, clippingRect.origin.y, clippingRect.size.width, clippingRect.size.height);
        }
        else
        {
            CCSize s = boundingBox().size;
            CCEGLView::sharedOpenGLView()->setScissorInPoints(m_loacationInWorld.x, m_loacationInWorld.y, s.width, s.height);
        }
        CCLayerColor::visit();
        if (m_bHandleScissor)
        {
            glDisable(GL_SCISSOR_TEST);
        }
        else
        {
            if (m_pClippingParent)
            {
                CCRect pClippingRect = m_pClippingParent->getClippingRect();
                CCEGLView::sharedOpenGLView()->setScissorInPoints(pClippingRect.origin.x, pClippingRect.origin.y, pClippingRect.size.width, pClippingRect.size.height);
            }
        }
        
    }
    else
    {
        CCLayerColor::visit();
    }
}

void UIClippingLayerColor::setClippingEnable(bool able)
{
    m_bClippingEnable = able;
    checkClippingOption();
    updateChildrenClippingOptions();
}

void UIClippingLayerColor::updateChildrenClippingOptions()
{
    for (int i=0; i<m_pChildren->count(); i++)
    {
        UIClippingLayerColor* cChild = dynamic_cast<UIClippingLayerColor*>(m_pChildren->objectAtIndex(i));
        if (cChild)
        {
            cChild->checkClippingOption();
            cChild->updateChildrenClippingOptions();
        }
    }
}

bool UIClippingLayerColor::isClippingEnable()
{
    return m_bClippingEnable;
}

void UIClippingLayerColor::setColorEnable(bool enable)
{
    m_bColorEnable = enable;
}

bool UIClippingLayerColor::getColorEnable()
{
    return m_bColorEnable;
}

void UIClippingLayerColor::setClipRect(const cocos2d::CCRect &rect)
{
    
}

const CCRect& UIClippingLayerColor::getClippingRect()
{
    if (m_pClippingParent)
    {
        CCRect pRect = m_pClippingParent->getClippingRect();
        float finalX = m_loacationInWorld.x;
        float finalY = m_loacationInWorld.y;
        float finalWidth = m_fScissorWidth;
        float finalHeight = m_fScissorHeight;
        
        float leftOffset = m_loacationInWorld.x - pRect.origin.x;
        if (leftOffset <= 0.0f)
        {
            finalX = pRect.origin.x;
            finalWidth += leftOffset;
        }
        float rightOffset = (m_loacationInWorld.x + m_fScissorWidth) - (pRect.origin.x + pRect.size.width);
        if (rightOffset >= 0)
        {
            finalWidth -= rightOffset;
        }
        float topOffset = (m_loacationInWorld.y + m_fScissorHeight) - (pRect.origin.y + pRect.size.height);
        if (topOffset >= 0)
        {
            finalHeight -= topOffset;
        }
        float bottomOffset = m_loacationInWorld.y - pRect.origin.y;
        if (bottomOffset <= 0)
        {
            finalY = pRect.origin.x;
            finalHeight += bottomOffset;
        }
        if (finalWidth < 0.0f)
        {
            finalWidth = 0.0f;
        }
        if (finalHeight < 0.0f)
        {
            finalHeight = 0.0f;
        }
        m_clippingRect.origin.x = finalX;
        m_clippingRect.origin.y = finalY;
        m_clippingRect.size.width = finalWidth;
        m_clippingRect.size.height = finalHeight;
//        m_clippingRect.origin = m_loacationInWorld;
//        m_clippingRect.size.width = m_fScissorWidth;
//        m_clippingRect.size.height = m_fScissorHeight;
    }
    else
    {
        m_clippingRect.origin = m_loacationInWorld;
        m_clippingRect.size.width = m_fScissorWidth;
        m_clippingRect.size.height = m_fScissorHeight;
    }
    return m_clippingRect;
    
}

void UIClippingLayerColor::setClipSize(float width, float height)
{
    m_bEnableCustomArea = true;
    m_fScissorWidth = width;
    m_fScissorHeight = height;
}

void UIClippingLayerColor::draw()
{
//        to head off the draw call
    if (m_bColorEnable)
    {
        CCLayerColor::draw();
    }
}

void UIClippingLayerColor::setPosition(const cocos2d::CCPoint &pos)
{
    CCLayerColor::setPosition(pos);
    m_loacationInWorld = convertToWorldSpace(CCPointZero);
}

NS_CC_EXT_END
