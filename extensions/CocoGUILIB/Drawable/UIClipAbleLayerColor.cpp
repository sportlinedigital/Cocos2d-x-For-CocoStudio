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

void UIClipAbleLayerColor::visit()
{
    if (this->m_bClipAble)
    {
        glEnable(GL_SCISSOR_TEST);
        CCPoint local = this->convertToWorldSpace(CCPointZero);
        
        if (this->m_bEnableCustomArea)
        {
            CCEGLView::sharedOpenGLView()->setScissorInPoints(local.x, local.y, this->m_fScissorWidth, this->m_fScissorHeight);
        }
        else
        {
            CCSize s = this->boundingBox().size;
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

void UIClipAbleLayerColor::setClipAble(bool able)
{
    this->m_bClipAble = able;
}

void UIClipAbleLayerColor::setColorEnable(bool enable)
{
    this->m_bColorEnable = enable;
}

bool UIClipAbleLayerColor::getColorEnable()
{
    return this->m_bColorEnable;
}

void UIClipAbleLayerColor::setClipRect(const cocos2d::CCRect &rect)
{
    
}

void UIClipAbleLayerColor::setClipSize(float width, float height)
{
    this->m_bEnableCustomArea = true;
    this->m_fScissorWidth = width;
    this->m_fScissorHeight = height;
}

void UIClipAbleLayerColor::draw()
{
//        to head off the draw call
    if (this->m_bColorEnable)
    {
        CCLayerColor::draw();
    }
}

NS_CC_EXT_END
