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

#ifndef __UICLIPABLELAYERCOLOR_H__
#define __UICLIPABLELAYERCOLOR_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN
    
class UIClipAbleLayerColor : public cocos2d::CCLayerColor
{
public:
    UIClipAbleLayerColor();
    virtual ~UIClipAbleLayerColor();
    static UIClipAbleLayerColor* create(const cocos2d::ccColor4B &color,float width ,float height);
    static UIClipAbleLayerColor* create();
    virtual void visit();
    void setClippingEnable(bool able);
    void setColorEnable(bool enable);
    bool getColorEnable();
    void setClipRect(const cocos2d::CCRect &rect);
    const CCRect& getClippingRect() const;
    void setClipSize(float width,float height);
    virtual void draw();
    virtual void setPosition(const CCPoint &pos);
    virtual void onEnter();
protected:
    bool m_bClippingEnable;
    float m_fScissorX;
    float m_fScissorY;
    float m_fScissorWidth;
    float m_fScissorHeight;
    bool m_bEnableCustomArea;
    bool m_bColorEnable;
    CCPoint m_loacationInWorld;
};

NS_CC_EXT_END

#endif
