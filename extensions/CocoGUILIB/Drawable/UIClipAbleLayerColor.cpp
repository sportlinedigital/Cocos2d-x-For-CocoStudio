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

#include "UIClipAbleLayerColor.h"

NS_CC_EXT_BEGIN

UIClipAbleLayerColor::UIClipAbleLayerColor():
m_bClippingEnable(false),
m_fScissorX(0.0f),
m_fScissorY(0.0),
m_fScissorWidth(0.0),
m_fScissorHeight(0.0),
m_bEnableCustomArea(false),
m_bColorEnable(false),
m_loacationInWorld(ccp(0, 0))
{
    
}

UIClipAbleLayerColor::~UIClipAbleLayerColor()
{
    
}

UIClipAbleLayerColor* UIClipAbleLayerColor::create(const cocos2d::ccColor4B &color,float width,float height)
{
    UIClipAbleLayerColor * pLayer = new UIClipAbleLayerColor();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

UIClipAbleLayerColor* UIClipAbleLayerColor::create()
{
    UIClipAbleLayerColor * pLayer = new UIClipAbleLayerColor();
    if( pLayer && pLayer->init())
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

void UIClipAbleLayerColor::onEnter()
{
    CCLayerColor::onEnter();
    m_loacationInWorld = convertToWorldSpace(CCPointZero);
}

void UIClipAbleLayerColor::visit()
{
    if (m_bClippingEnable)
    {
        glEnable(GL_SCISSOR_TEST);
        
        if (m_bEnableCustomArea)
        {
            CCEGLView::sharedOpenGLView()->setScissorInPoints(m_loacationInWorld.x, m_loacationInWorld.y, m_fScissorWidth, m_fScissorHeight);
        }
        else
        {
            CCSize s = boundingBox().size;
            CCEGLView::sharedOpenGLView()->setScissorInPoints(m_loacationInWorld.x, m_loacationInWorld.y, s.width, s.height);
        }
        CCLayerColor::visit();
        glDisable(GL_SCISSOR_TEST);
    }
    else
    {
        CCLayerColor::visit();
    }
}

void UIClipAbleLayerColor::setClippingEnable(bool able)
{
    m_bClippingEnable = able;
}

void UIClipAbleLayerColor::setColorEnable(bool enable)
{
    m_bColorEnable = enable;
}

bool UIClipAbleLayerColor::getColorEnable()
{
    return m_bColorEnable;
}

void UIClipAbleLayerColor::setClipRect(const cocos2d::CCRect &rect)
{
    
}

const CCRect& UIClipAbleLayerColor::getClippingRect() const
{
    
}

void UIClipAbleLayerColor::setClipSize(float width, float height)
{
    m_bEnableCustomArea = true;
    m_fScissorWidth = width;
    m_fScissorHeight = height;
}

void UIClipAbleLayerColor::draw()
{
//        to head off the draw call
    if (m_bColorEnable)
    {
        CCLayerColor::draw();
    }
}

void UIClipAbleLayerColor::setPosition(const cocos2d::CCPoint &pos)
{
    CCLayerColor::setPosition(pos);
    m_loacationInWorld = convertToWorldSpace(CCPointZero);
}

NS_CC_EXT_END
