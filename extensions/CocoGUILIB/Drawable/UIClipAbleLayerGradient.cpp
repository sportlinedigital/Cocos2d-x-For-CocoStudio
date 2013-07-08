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

#include "UIClipAbleLayerGradient.h"

using namespace cocos2d;

NS_CC_EXT_BEGIN
    
UIClipAbleLayerGradient* UIClipAbleLayerGradient::create(const cocos2d::ccColor4B &colorStart,const cocos2d::ccColor4B &colorEnd)
{
    UIClipAbleLayerGradient * pLayer = new UIClipAbleLayerGradient();
    if( pLayer && pLayer->initWithColor(colorStart,colorEnd))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

UIClipAbleLayerGradient* UIClipAbleLayerGradient::create()
{
    UIClipAbleLayerGradient * pLayer = new UIClipAbleLayerGradient();
    if( pLayer && pLayer->init())
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

void UIClipAbleLayerGradient::visit()
{
    if (m_bClippingEnable)
    {
        glEnable(GL_SCISSOR_TEST);
        CCPoint local = convertToWorldSpace(CCPointZero);
        
        if (m_bEnableCustomArea)
        {
            CCEGLView::sharedOpenGLView()->setScissorInPoints(local.x, local.y, m_fScissorWidth, m_fScissorHeight);
        }
        else
        {
            CCSize s = boundingBox().size;
            CCEGLView::sharedOpenGLView()->setScissorInPoints(local.x, local.y, s.width, s.height);
        }
        
        CCLayerColor::visit();
        glDisable(GL_SCISSOR_TEST);
    }
    else
    {
        CCLayerColor::visit();
    }
}

void UIClipAbleLayerGradient::setClippingEnable(bool able)
{
    m_bClippingEnable = able;
}

void UIClipAbleLayerGradient::setColorEnable(bool enable)
{
    m_bColorEnable = enable;
}

bool UIClipAbleLayerGradient::getColorEnable()
{
    return m_bColorEnable;
}

void UIClipAbleLayerGradient::setClipRect(const cocos2d::CCRect &rect)
{
    
}

void UIClipAbleLayerGradient::setClipSize(float width, float height)
{
    m_bEnableCustomArea = true;
    m_fScissorWidth = width;
    m_fScissorHeight = height;
}

void UIClipAbleLayerGradient::draw()
{
    //        to head off the draw call
    if (m_bColorEnable)
    {
        CCLayerColor::draw();
    }
}

NS_CC_EXT_END
