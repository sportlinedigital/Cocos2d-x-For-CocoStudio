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

#include "UIContainerWidget.h"
#include "../Drawable/UIClippingLayer.h"
#include "../System/UIHelper.h"

NS_CC_EXT_BEGIN

#define DYNAMIC_CAST_CLIPPINGLAYER dynamic_cast<UIClippingLayer*>(m_pRender)
    
UIContainerWidget::UIContainerWidget():
m_fWidth(0.0),
m_fHeight(0.0),
m_bClipAble(false),
m_renderType(RENDER_TYPE_LAYERCOLOR),
m_eLayoutType(UI_LAYOUT_ABSOLUTE)
{
    m_WidgetType = WidgetTypeContainer;
    m_WidgetName = WIDGET_CONTAINERWIDGET;
}

UIContainerWidget::~UIContainerWidget()
{
    
}

UIContainerWidget* UIContainerWidget::create()
{
    UIContainerWidget* widget = new UIContainerWidget();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

/*****************temp***********/
bool UIContainerWidget::init()
{
    m_children = cocos2d::CCArray::create();
    m_children->retain();
    initNodes();
    m_pRender->retain();
    m_pRender->setZOrder(m_nWidgetZOrder);
    CCRGBAProtocol* renderRGBA = dynamic_cast<CCRGBAProtocol*>(m_pRender);
    if (renderRGBA)
    {
        renderRGBA->setCascadeColorEnabled(false);
        renderRGBA->setCascadeOpacityEnabled(false);
    }
    return true;
}
/********************************/

void UIContainerWidget::setLayoutType(LayoutType type)
{
    if (type == m_eLayoutType)
    {
        return;
    }
    m_eLayoutType = type;
    doLayout();
}

LayoutType UIContainerWidget::getLayoutType()
{
    return m_eLayoutType;
}

void UIContainerWidget::doLayout()
{
    switch (m_eLayoutType)
    {
        case UI_LAYOUT_ABSOLUTE:
            break;
        case UI_LAYOUT_LINEAR_VERTICAL:
        {
            ccArray* arrayChildren = m_children->data;
            int childrenCount = arrayChildren->num;
            float topBoundary = m_fHeight;
            for (int i=0; i<childrenCount; i++)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(arrayChildren->arr[i]);
                UILinearGravity childGravity = child->getLinearGravity();
                float finalPosX = child->getAnchorPoint().x*child->getContentSize().width;
                switch (childGravity)
                {
                    case LINEAR_GRAVITY_NONE:
                    case LINEAR_GRAVITY_LEFT:
                        break;
                    case LINEAR_GRAVITY_RIGHT:
                        finalPosX = m_fWidth - ((1.0f-child->getAnchorPoint().x)*child->getContentSize().width);
                        break;
                    case LINEAR_GRAVITY_CENTER_HORIZONTAL:
                        finalPosX = m_fWidth/2.0f - (child->getContentSize().width)*(0.5f-child->getAnchorPoint().x);
                        break;
                    default:
                        break;
                }
                UIMargin mg = child->getMargin();
                child->setPosition(ccp(finalPosX+mg.left, topBoundary-((1.0f-child->getAnchorPoint().y)*child->getContentSize().height)-mg.top));
                topBoundary = child->getRelativeBottomPos()-mg.bottom;
            }
            break;
        }
        case UI_LAYOUT_LINEAR_HORIZONTAL:
        {
            ccArray* arrayChildren = m_children->data;
            int childrenCount = arrayChildren->num;
            float leftBoundary = 0.0f;
            for (int i=0; i<childrenCount; i++)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(arrayChildren->arr[i]);
                UILinearGravity childGravity = child->getLinearGravity();
                float finalPosY = m_fHeight - (1.0f-child->getAnchorPoint().y)*child->getContentSize().height;
                switch (childGravity)
                {
                    case LINEAR_GRAVITY_NONE:
                    case LINEAR_GRAVITY_TOP:
                        break;
                    case LINEAR_GRAVITY_BOTTOM:
                        finalPosY = child->getAnchorPoint().y*child->getContentSize().height;
                        break;
                    case LINEAR_GRAVITY_CENTER_VERTICAL:
                        finalPosY = m_fHeight/2.0f - (child->getContentSize().height)*(0.5f-child->getAnchorPoint().y);
                        break;
                    default:
                        break;
                }
                UIMargin mg = child->getMargin();
                child->setPosition(ccp(leftBoundary+mg.left+(child->getAnchorPoint().x*child->getContentSize().width), finalPosY-mg.top));
                leftBoundary = child->getRelativeRightPos()+mg.right;
            }
            break;
        }
        case UI_LAYOUT_RELATIVE:
        {
            ccArray* arrayChildren = m_children->data;
            int childrenCount = arrayChildren->num;
            for (int i=0; i<childrenCount; i++)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(arrayChildren->arr[i]);
                float finalPosX = child->getAnchorPoint().x*child->getContentSize().width;
                float finalPosY = m_fHeight-((1.0f-child->getAnchorPoint().y)*child->getContentSize().height);
                UIRelativeAlign align = child->getRelativeAlign();
                const char* relativeName = child->getRelativeWidgetName();
                UIWidget* relativeWidget = NULL;
                if (relativeName && strcmp(relativeName, ""))
                {
                    relativeWidget = CCUIHELPER->seekWidgetByRelativeName(this, relativeName);
                }
                switch (align)
                {
                    case RELATIVE_ALIGN_PARENT_NONE:
                        break;
                    case RELATIVE_ALIGN_PARENT_LEFT:
                        break;
                    case RELATIVE_ALIGN_PARENT_TOP:
                        break;
                    case RELATIVE_ALIGN_PARENT_RIGHT:
                        finalPosX = m_fWidth - ((1.0f-child->getAnchorPoint().x)*child->getContentSize().width);
                        break;
                    case RELATIVE_ALIGN_PARENT_BOTTOM:
                        finalPosY = child->getAnchorPoint().y*child->getContentSize().height;
                        break;
                    case RELATIVE_CENTER_IN_PARENT:
                        finalPosX = m_fWidth/2.0f - (child->getContentSize().width)*(0.5f-child->getAnchorPoint().x);
                        finalPosY = m_fHeight/2.0f - (child->getContentSize().height)*(0.5f-child->getAnchorPoint().y);
                        break;
                    case RELATIVE_CENTER_HORIZONTAL:
                        finalPosX = m_fWidth/2.0f - (child->getContentSize().width)*(0.5f-child->getAnchorPoint().x);
                        break;
                    case RELATIVE_CENTER_VERTICAL:
                        finalPosY = m_fHeight/2.0f - (child->getContentSize().height)*(0.5f-child->getAnchorPoint().y);
                        break;
                    case RELATIVE_LOCATION_ABOVE:
                        if (relativeWidget)
                        {
                            finalPosX = relativeWidget->getPosition().x;
                            float locationBottom = relativeWidget->getRelativeTopPos();
                            float locationLeft = relativeWidget->getRelativeLeftPos();
                            finalPosY = locationBottom+child->getAnchorPoint().y*child->getContentSize().height;
                            finalPosX = locationLeft+child->getAnchorPoint().x*child->getContentSize().width;
                        }
                        break;
                    case RELATIVE_LOCATION_BELOW:
                        if (relativeWidget)
                        {
                            float locationTop = relativeWidget->getRelativeBottomPos();
                            float locationLeft = relativeWidget->getRelativeLeftPos();
                            finalPosY = locationTop-(1.0f-child->getAnchorPoint().y)*child->getContentSize().height;
                            finalPosX = locationLeft+child->getAnchorPoint().x*child->getContentSize().width;
                        }
                        break;
                    case RELATIVE_LOCATION_LEFT_OF:
                        if (relativeWidget)
                        {
                            float locationTop = relativeWidget->getRelativeTopPos();
                            float locationRight = relativeWidget->getRelativeLeftPos();
                            finalPosY = locationTop-child->getAnchorPoint().y*child->getContentSize().height;
                            finalPosX = locationRight-(1.0f-child->getAnchorPoint().x)*child->getContentSize().width;
                        }
                        break;
                    case RELATIVE_LOCATION_RIGHT_OF:
                        if (relativeWidget)
                        {
                            float locationTop = relativeWidget->getRelativeTopPos();
                            float locationLeft = relativeWidget->getRelativeRightPos();
                            finalPosY = locationTop-child->getAnchorPoint().y*child->getContentSize().height;
                            finalPosX = locationLeft+child->getAnchorPoint().x*child->getContentSize().width;
                        }
                        break;
                    default:
                        break;
                }
                UIRelativeAlignWidget alignWidget = child->getRelativeAlignWidget();
                switch (alignWidget)
                {
                    case RELATIVE_ALIGN_WIDGET_NONE:
                        break;
                    case RELATIVE_ALIGN_WIDGET_LEFT:
                        break;
                    case RELATIVE_ALIGN_WIDGET_TOP:
                        break;
                    case RELATIVE_ALIGN_WIDGET_RIGHT:
                    {
                        UIRelativeAlign align = child->getRelativeAlign();
                        if (align == RELATIVE_LOCATION_ABOVE || align == RELATIVE_LOCATION_BELOW)
                        {
                            if (relativeWidget)
                            {
                                float locationRight = relativeWidget->getRelativeRightPos();
                                finalPosX = locationRight-(1.0f-child->getAnchorPoint().x)*child->getContentSize().width;
                            }
                        }
                        break;
                    }
                    case RELATIVE_ALIGN_WIDGET_BOTTOM:
                    {
                        UIRelativeAlign align = child->getRelativeAlign();
                        if (align == RELATIVE_LOCATION_LEFT_OF || align == RELATIVE_LOCATION_RIGHT_OF)
                        {
                            if (relativeWidget)
                            {
                                float locationBottom = relativeWidget->getRelativeBottomPos();
                                finalPosY = locationBottom+child->getAnchorPoint().y*child->getContentSize().height;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                UIMargin relativeWidgetMargin;
                UIMargin mg;
                if (relativeWidget)
                {
                    relativeWidgetMargin = relativeWidget->getMargin();
                    mg = child->getMargin();
                }
                //handle margin
                switch (align)
                {
                    case RELATIVE_LOCATION_ABOVE:
                        finalPosY += relativeWidgetMargin.top;
                        finalPosY += relativeWidgetMargin.bottom;
                        break;
                    case RELATIVE_LOCATION_BELOW:
                        finalPosY -= relativeWidgetMargin.bottom;
                        finalPosY -= mg.top;
                        break;
                    case RELATIVE_LOCATION_LEFT_OF:
                        finalPosX -= relativeWidgetMargin.left;
                        finalPosX -= mg.right;
                        break;
                    case RELATIVE_LOCATION_RIGHT_OF:
                        finalPosX += relativeWidgetMargin.right;
                        finalPosX += mg.left;
                        break;
                    default:
                        break;
                }
                
                child->setPosition(ccp(finalPosX, finalPosY));
            }
            break;
        }
        case UI_LAYOUT_GRID:
            break;
        case UI_LAYOUT_BORDER:
            break;
        case UI_LAYOUT_TABLE:
            break;
        default:
            break;
    }
}

void UIContainerWidget::initNodes()
{
    m_pRender = UIClippingLayer::create();
}

bool UIContainerWidget::isClippingEnable()
{
    return m_bClipAble;
}

bool UIContainerWidget::addChild(UIWidget* child)
{   
    UIWidget::addChild(child);
    bool needSetChildCheckAble = false;
    UIWidget* parent = this;
    while (parent)
    {
        if (parent->isClippingEnable())
        {
            needSetChildCheckAble = true;
            break;
        }
        parent = parent->getWidgetParent();
    }
    
    if (needSetChildCheckAble)
    {
        child->setNeedCheckVisibleDepandParent(true);
    }
    return true;
}

void UIContainerWidget::setClippingEnable(bool able)
{
    m_bClipAble = able;
    DYNAMIC_CAST_CLIPPINGLAYER->setClippingEnable(able);
    ccArray* arrayChildren = m_children->data;
    int childrenCount = arrayChildren->num;
    for (int i=0; i<childrenCount; i++)
    {
        UIWidget* child = (UIWidget*)(arrayChildren->arr[i]);
        child->setNeedCheckVisibleDepandParent(able);
    }
}

void UIContainerWidget::setClipRect(const cocos2d::CCRect &rect)
{
    DYNAMIC_CAST_CLIPPINGLAYER->setClipRect(rect);
}

void UIContainerWidget::updateWidth()
{

}

void UIContainerWidget::updateHeight()
{
    
}

void UIContainerWidget::setSize(const cocos2d::CCSize &size)
{
    DYNAMIC_CAST_CLIPPINGLAYER->setContentSize(size);
    m_fWidth = size.width;
    m_fHeight = size.height;
}

float UIContainerWidget::getWidth()
{
    return m_fWidth;
}

float UIContainerWidget::getHeight()
{
    return m_fHeight;
}

bool UIContainerWidget::hitTest(cocos2d::CCNode *node, cocos2d::CCPoint &pt)
{
    cocos2d::CCPoint nsp = node->convertToNodeSpace(pt);
    cocos2d::CCSize bb = node->getContentSize();
    if (nsp.x >= 0.0f && nsp.x <= bb.width && nsp.y >= 0.0f && nsp.y <= bb.height)
    {
        return true;
    }
    return false;
}

CCSize UIContainerWidget::getWrapSize() const
{
    ccArray* arrayChildren = m_children->data;
    int childrenCount = arrayChildren->num;
    for (int i=0; i<childrenCount; i++)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(arrayChildren->arr[i]);
        switch (child->getWidgetType())
        {
            case WidgetTypeWidget:
                break;
            case WidgetTypeContainer:
                break;
            default:
                break;
        }
    }
    return CCSizeZero;
}

NS_CC_EXT_END