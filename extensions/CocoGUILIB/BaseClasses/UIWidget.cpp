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

#include "UIWidget.h"
#include "../System/UIHelper.h"
#include "../System/UILayer.h"

NS_CC_EXT_BEGIN

#define DYNAMIC_CAST_CCBLENDPROTOCOL dynamic_cast<cocos2d::CCBlendProtocol*>(this->m_pCCRenderNode)

#define DYNAMIC_CAST_CCRGBAPROTOCOL dynamic_cast<cocos2d::CCRGBAProtocol*>(this->m_pCCRenderNode)

#define DYNAMIC_CAST_GUINODERGBA dynamic_cast<GUINodeRGBA*>(this->m_pCCRenderNode)
    
UIWidget::UIWidget():
m_bEnabled(true),
m_bActived(true),
m_bFocus(false),
m_nWidgetZOrder(0),
m_pWidgetParent(NULL),
m_nCurPressState(WidgetStateNone),
m_nPrevPressstate(WidgetStateNone),
m_bBeTouchEnabled(false),
m_nWidgetTag(-1),
m_bUpdateEnable(false),
m_pCCRenderNode(NULL),
m_strName("default"),
m_children(NULL),
m_WidgetType(WidgetTypeWidget),
m_bVisible(true),
m_bNeedCheckVisibleDependParent(false),
m_pPushListener(NULL),
m_pfnPushSelector(NULL),
m_pMoveListener(NULL),
m_pfnMoveSelector(NULL),
m_pReleaseListener(NULL),
m_pfnReleaseSelector(NULL),
m_pCancelListener(NULL),
m_pfnCancelSelector(NULL),
m_fContentSizeHeight(0),
m_fContentSizeWidth(0),
m_bUseMergedTexture(false),
m_fAbsoluteScaleX(1.0),
m_fAbsoluteScaleY(1.0),
m_bScaleXDirty(true),
m_bScaleYDirty(true),
m_bAbsoluteVisible(true),
m_bVisibleDirty(true),
m_fAdaptScaleX(1.0f),
m_fAdaptScaleY(1.0f),
m_anchorPoint(ccp(0.5f, 0.5f)),
m_pUILayer(NULL),
m_bIsCreatedFromFile(false),
m_fileDesignSize(CCSizeZero)
{
    
}

UIWidget::~UIWidget()
{
    
}

UIWidget* UIWidget::create()
{
    UIWidget* widget = new UIWidget();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UIWidget::init()
{
    this->m_children = cocos2d::CCArray::create();
    this->m_children->retain();
    this->initNodes();
    this->m_pCCRenderNode->retain();
    this->m_pCCRenderNode->setZOrder(this->m_nWidgetZOrder);
    return true;
}

void UIWidget::releaseResoures()
{
    this->setUpdateEnable(false);
    this->setUILayer(NULL);
    this->removeAllChildrenAndCleanUp(true);
    this->m_pCCRenderNode->removeAllChildrenWithCleanup(true);
    this->m_pCCRenderNode->removeFromParentAndCleanup(true);
    this->m_pCCRenderNode->release();
    this->m_children->release();
}

void UIWidget::initNodes()
{
    this->m_pCCRenderNode = GUINodeRGBA::create();
}

bool UIWidget::addChild(UIWidget *child)
{
    if (!child)
    {
        return false;
    }
    if (this->m_children->containsObject(child))
    {
        return false;
    }
    child->m_pWidgetParent = this;
    int childrenCount = this->m_children->count();
    if (childrenCount <= 0)
    {
        this->m_children->addObject(child);
    }
    else
    {
        bool seekSucceed = false;
        for (int i=childrenCount-1; i>=0; --i)
        {
            UIWidget* widget = (UIWidget*)(this->m_children->objectAtIndex(i));
            if (child->getWidgetZOrder() >= widget->getWidgetZOrder())
            {
                if (i == childrenCount-1)
                {
                    this->m_children->addObject(child);
                    seekSucceed = true;
                    break;
                }
                else
                {
                    this->m_children->insertObject(child, i+1);
                    seekSucceed = true;
                    break;
                }
            }
        }
        if (!seekSucceed)
        {
            this->m_children->insertObject(child,0);
        }
    }
    child->m_pCCRenderNode->setZOrder(child->getWidgetZOrder());
    this->m_pCCRenderNode->addChild(child->m_pCCRenderNode);
    
    if (this->m_pUILayer) {
        for (int i=0; i<this->m_children->count(); i++) {
            UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
            child->updateChildrenUILayer(this->m_pUILayer);
        }
    }
    structureChangedEvent();
    return true;
}

void UIWidget::setUILayer(cocos2d::extension::UILayer *uiLayer)
{
    this->m_pUILayer = uiLayer;
}

void UIWidget::updateChildrenUILayer(UILayer* uiLayer)
{
    this->setUILayer(uiLayer);
    this->setUpdateEnable(this->getUpdateEnable());
    for (int i=0; i<this->m_children->count(); i++) {
        UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
        child->updateChildrenUILayer(this->m_pUILayer);
    }
}

void UIWidget::disableUpdate()
{
    if (m_pUILayer) {
        m_pUILayer->removeUpdateEnableWidget(this);
    }
    for (int i=0; i<this->m_children->count(); i++) {
        UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
        child->disableUpdate();
    }
}

void UIWidget::structureChangedEvent()
{
    if (this->m_pUILayer) {
        this->m_pUILayer->getInputManager()->uiSceneHasChanged();
    }
}

bool UIWidget::removeChild(UIWidget *child, bool cleanup)
{
    if (!child)
    {
        return false;
    }
    if (cleanup)
    {
        this->removeChildMoveToTrash(child);
    }
    else
    {
        this->removeChildReferenceOnly(child);
    }
    return true;
}

void UIWidget::removeChildMoveToTrash(UIWidget *child)
{
    if (!child)
    {
        return;
    }
    if (this->m_children->containsObject(child))
    {
        this->m_children->removeObject(child);
        child->structureChangedEvent();
        child->releaseResoures();
        child->m_pWidgetParent = NULL;
        delete child;
    }
}

void UIWidget::removeChildReferenceOnly(UIWidget *child)
{
    if (!child)
    {
        return;
    }
    if (this->m_children->containsObject(child))
    {
        child->structureChangedEvent();
        child->disableUpdate();
        child->updateChildrenUILayer(NULL);
        this->m_pCCRenderNode->removeChild(child->m_pCCRenderNode, false);
        child->setNeedCheckVisibleDepandParent(false);
        this->m_children->removeObject(child);
        child->m_pWidgetParent = NULL;
    }
}

void UIWidget::removeFromParentAndCleanup(bool cleanup)
{
    if (this->m_pWidgetParent)
    {
        this->m_pWidgetParent->removeChild(this, cleanup);
    }
    else
    {
        structureChangedEvent();
        releaseResoures();
        m_pWidgetParent = NULL;
        delete this;
    }
}

void UIWidget::removeAllChildrenAndCleanUp(bool cleanup)
{
    int times = this->m_children->count();
    for (int i=0;i<times;i++)
    {
        UIWidget* child = (UIWidget*)(m_children->lastObject());
        m_children->removeObject(child);
        child->structureChangedEvent();
        child->releaseResoures();
        delete child;
        child = NULL;
    }
}

void UIWidget::setWidgetZOrder(int z)
{
    this->m_nWidgetZOrder = z;
    this->m_pCCRenderNode->setZOrder(z);
    if (this->m_pWidgetParent) {
        this->m_pWidgetParent->reorderChild(this);
    }
}

int UIWidget::getWidgetZOrder()
{
    return this->m_nWidgetZOrder;
}

void UIWidget::reorderChild(UIWidget* child)
{
    this->m_children->removeObject(child);
    int childrenCount = this->m_children->count();
    if (childrenCount <= 0) {
        this->m_children->addObject(child);
    }
    else
    {
        bool seekSucceed = false;
        for (int i=childrenCount-1; i>=0; --i)
        {
            UIWidget* widget = (UIWidget*)(this->m_children->objectAtIndex(i));
            if (child->getWidgetZOrder() >= widget->getWidgetZOrder())
            {
                if (i == childrenCount-1)
                {
                    this->m_children->addObject(child);
                    seekSucceed = true;
                    break;
                }
                else
                {
                    this->m_children->insertObject(child, i+1);
                    seekSucceed = true;
                    break;
                }
            }
        }
        if (!seekSucceed)
        {
            this->m_children->insertObject(child,0);
        }
    }
    structureChangedEvent();
}

void UIWidget::setNeedCheckVisibleDepandParent(bool need)
{
    this->m_bNeedCheckVisibleDependParent = need;
    if (this->m_children)
    {
        for (int i=0;i<this->m_children->count();i++)
        {
            UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
            child->setNeedCheckVisibleDepandParent(need);
        }
    }
}

void UIWidget::setBeTouchEnable(bool enable)
{
    this->m_bBeTouchEnabled = enable;
    structureChangedEvent();
}

bool UIWidget::getBeTouchEnable()
{
    return this->m_bBeTouchEnabled;
}

void UIWidget::setUpdateEnable(bool enable)
{
    m_bUpdateEnable = enable;
    if (enable)
    {
        if (this->m_pUILayer)
        {
            m_pUILayer->addUpdateEnableWidget(this);
        }
    }
    else
    {
        if (this->m_pUILayer)
        {
            m_pUILayer->removeUpdateEnableWidget(this);
        }
    }
}

bool UIWidget::getUpdateEnable()
{
    return this->m_bUpdateEnable;
}

bool UIWidget::isFocus()
{
    return this->m_bFocus;
}

void UIWidget::setFocus(bool fucos)
{
    if (fucos == this->m_bFocus)
    {
        return;
    }
    this->m_bFocus = fucos;
    if (this->m_bFocus)
    {
        this->setPressState(WidgetStateSelected);
    }
    else
    {
        this->setPressState(WidgetStateNormal);
    }
}

void UIWidget::initPressState(WidgetState state)
{
    if (this->m_nCurPressState == state)
    {
        return;
    }
    this->m_nPrevPressstate = this->m_nCurPressState;
    this->m_nCurPressState = state;
}

void UIWidget::setPressState(WidgetState state)
{
    if (this->m_nCurPressState == state)
    {
        return;
    }
    this->m_nPrevPressstate = this->m_nCurPressState;
    this->m_nCurPressState = state;
    switch (this->m_nCurPressState)
    {
        case WidgetStateNormal:
            this->onPressStateChangedToNormal();
            break;
        case WidgetStateSelected:
            this->onPressStateChangedToPressed();
            break;
        case WidgetStateDisabled:
            this->onPressStateChangedToDisabled();
            break;
        default:
            break;
            
    }
}

void UIWidget::disable()
{
    this->m_bActived = false;
    this->setPressState(WidgetStateDisabled);
}

void UIWidget::active()
{
    this->m_bActived = true;
    this->setPressState(WidgetStateNormal);
}

bool UIWidget::isActive()
{
    return this->m_bActived;
}

void UIWidget::onPressStateChangedToNormal()
{
    
}

void UIWidget::onPressStateChangedToPressed()
{
    
}

void UIWidget::onPressStateChangedToDisabled()
{
    
}

void UIWidget::didNotSelectSelf()
{
    
}

bool UIWidget::onTouchBegan(cocos2d::CCPoint &touchPoint)
{
    this->setFocus(true);
    this->m_touchStartPos.x = touchPoint.x;
    this->m_touchStartPos.y = touchPoint.y;
    if (this->m_pWidgetParent)
    {
        this->m_pWidgetParent->checkChildInfo(0,this,touchPoint);
    }
    this->pushDownEvent();
    return true;
}

bool UIWidget::onTouchMoved(cocos2d::CCPoint &touchPoint)
{
    this->m_touchMovePos.x = touchPoint.x;
    this->m_touchMovePos.y = touchPoint.y;
    this->setFocus(this->pointAtSelfBody(touchPoint));
    if (this->m_pWidgetParent)
    {
        this->m_pWidgetParent->checkChildInfo(1,this,touchPoint);
    }
    this->moveEvent();
    return true;
}

bool UIWidget::onTouchEnded(cocos2d::CCPoint &touchPoint)
{
    this->m_touchEndPos.x = touchPoint.x;
    this->m_touchEndPos.y = touchPoint.y;
    bool focus = this->m_bFocus;
    this->setFocus(false);
    if (this->m_pWidgetParent)
    {
        this->m_pWidgetParent->checkChildInfo(2,this,touchPoint);
    }
    if (focus)
    {
        this->releaseUpEvent();
    }
    else
    {
        this->cancelUpEvent();
    }
    return true;
}

bool UIWidget::onTouchCancelled(cocos2d::CCPoint &touchPoint)
{
    this->setPressState(WidgetStateNormal);
    return true;
}

bool UIWidget::onTouchLongClicked(cocos2d::CCPoint &touchPoint)
{
    this->longClickEvent();
    return true;
}

void UIWidget::pushDownEvent()
{
    if (m_pPushListener && m_pfnPushSelector)
    {
        (m_pPushListener->*m_pfnPushSelector)(this);
    }
}

void UIWidget::moveEvent()
{
    if (m_pMoveListener && m_pfnMoveSelector)
    {
        (m_pMoveListener->*m_pfnMoveSelector)(this);
    }
}

void UIWidget::releaseUpEvent()
{
    if (m_pReleaseListener && m_pfnReleaseSelector)
    {
        (m_pReleaseListener->*m_pfnReleaseSelector)(this);
    }
}

void UIWidget::cancelUpEvent()
{
    if (m_pCancelListener && m_pfnCancelSelector)
    {
        (m_pCancelListener->*m_pfnCancelSelector)(this);
    }
}

void UIWidget::longClickEvent()
{
    
}

void UIWidget::addPushDownEvent(cocos2d::CCObject*target, SEL_PushEvent selector)
{
    this->m_pPushListener = target;
    this->m_pfnPushSelector = selector;
}

void UIWidget::addMoveEvent(cocos2d::CCObject* target,SEL_MoveEvent selector)
{
    this->m_pMoveListener = target;
    this->m_pfnMoveSelector = selector;
}

void UIWidget::addReleaseEvent(cocos2d::CCObject* target,SEL_ReleaseEvent selector)
{
    this->m_pReleaseListener = target;
    this->m_pfnReleaseSelector = selector;
}

void UIWidget::addCancelEvent(cocos2d::CCObject *target, SEL_CancelEvent selector)
{
    this->m_pCancelListener = target;
    this->m_pfnCancelSelector = selector;
}

void UIWidget::getLocationInWindow()
{
    this->m_locationInWindow = this->m_pCCRenderNode->convertToWorldSpace(cocos2d::CCPointZero);
}

cocos2d::CCRect UIWidget::getRect()
{
    cocos2d::CCNode* validNode = this->getValidNode();
    float width = 0.0;
    float height = 0.0;
    float anchorPointX = 0.0;
    float anchorPointY = 0.0;
    this->getLocationInWindow();
    cocos2d::CCSize nodeSize = validNode->getContentSize();
    width = nodeSize.width*this->getAbsoluteScaleX();
    height = nodeSize.height*this->getAbsoluteScaleY();
    cocos2d::CCPoint nodeAnchorPoint = validNode->getAnchorPoint();
    anchorPointX = nodeAnchorPoint.x;
    anchorPointY = nodeAnchorPoint.y;
    switch (this->m_WidgetType)
    {
        case WidgetTypeWidget:
            this->m_rect.origin.x = this->m_locationInWindow.x - width * anchorPointX;
            this->m_rect.origin.y = this->m_locationInWindow.y - height * anchorPointY;
            break;
        case WidgetTypeContainer:
            this->m_rect.origin.x = this->m_locationInWindow.x;
            this->m_rect.origin.y = this->m_locationInWindow.y;
            break;
    }
    this->m_rect.size.width = width;
    this->m_rect.size.height = height;
    
    return this->m_rect;
}

cocos2d::CCRect UIWidget::getRelativeRect()
{
    cocos2d::CCNode* validNode = this->getValidNode();
    float width = 0.0;
    float height = 0.0;
    float anchorPointX = 0.0;
    float anchorPointY = 0.0;
    cocos2d::CCSize nodeSize = validNode->getContentSize();
    width = nodeSize.width*this->getScaleX();
    height = nodeSize.height*this->getScaleY();
    cocos2d::CCPoint nodeAnchorPoint = validNode->getAnchorPoint();
    anchorPointX = nodeAnchorPoint.x;
    anchorPointY = nodeAnchorPoint.y;
    switch (this->m_WidgetType)
    {
        case WidgetTypeWidget:
            this->m_relativeRect.origin.x = this->getPosition().x - width * anchorPointX;
            this->m_relativeRect.origin.y = this->getPosition().y - height * anchorPointY;
            break;
        case WidgetTypeContainer:
            this->m_relativeRect.origin.x = this->getPosition().x;
            this->m_relativeRect.origin.y = this->getPosition().y;
            break;
    }
    this->m_relativeRect.size.width = width;
    this->m_relativeRect.size.height = height;
    return this->m_relativeRect;
}

cocos2d::CCNode* UIWidget::getValidNode()
{
    return this->m_pCCRenderNode;
}

cocos2d::CCNode* UIWidget::getContainerNode()
{
    return this->m_pCCRenderNode;
}

bool UIWidget::pointAtSelfBody(cocos2d::CCPoint &pt)
{
    if (!this->getAbsoluteVisible())
    {
        return false;
    }
    return this->hitTest(this->getValidNode(),pt);
}

bool UIWidget::hitTest(cocos2d::CCNode* node, cocos2d::CCPoint &pt)
{
    cocos2d::CCPoint nsp = node->convertToNodeSpace(pt);
    cocos2d::CCRect bb = node->boundingBox();
    if (nsp.x >= 0 && nsp.x <= bb.size.width && nsp.y >= 0 && nsp.y <= bb.size.height)
    {
        return true;
    }
    return false;
}

bool UIWidget::checkVisibleDependParent(cocos2d::CCPoint &pt)
{
    if (!this->m_bNeedCheckVisibleDependParent)
    {
        return true;
    }
    if (this->m_pWidgetParent)
    {
        bool bRet = false;
        if (this->m_pWidgetParent->pointAtSelfBody(pt))
        {
            bRet = true;
        }
        if (bRet)
        {
            return this->m_pWidgetParent->checkVisibleDependParent(pt);
        }
        return false;
    }
    return true;
}

bool UIWidget::checkBeVisibleInParent()
{   
    cocos2d::CCRect parentRect = this->m_pWidgetParent->getRect();
    cocos2d::CCRect selfRect = this->getRect();
    bool res = !((selfRect.origin.x+selfRect.size.width) < parentRect.origin.x ||
                (parentRect.origin.x+parentRect.size.width) <   selfRect.origin.x ||
                (selfRect.origin.y+selfRect.size.height) < parentRect.origin.y ||
                parentRect.origin.y+parentRect.size.height <    selfRect.origin.y);
    return res;
}

void UIWidget::checkChildInfo(int handleState, UIWidget *sender, cocos2d::CCPoint &touchPoint)
{
    if (this->m_pWidgetParent)
    {
        this->m_pWidgetParent->checkChildInfo(handleState,sender,touchPoint);
    }
}

void UIWidget::setPosition(const cocos2d::CCPoint &pos)
{
    this->m_pCCRenderNode->setPosition(pos);
}

void UIWidget::setAnchorPoint(const cocos2d::CCPoint &pt)
{
    this->m_anchorPoint = pt;
    this->m_pCCRenderNode->setAnchorPoint(pt);
}

void UIWidget::updateAnchorPoint()
{
    this->setAnchorPoint(this->m_anchorPoint);
}

cocos2d::CCPoint UIWidget::getPosition()
{
    return this->m_pCCRenderNode->getPosition();
}

cocos2d::CCPoint UIWidget::getAnchorPoint()
{
    return this->m_anchorPoint;
}

void UIWidget::setScale(float scale)
{
    this->m_pCCRenderNode->setScale(scale);
    this->onScaleDirtyChanged();
}

void UIWidget::onScaleDirtyChanged()
{
    this->m_bScaleXDirty = this->m_bScaleYDirty = true;
    for (int i=0; i<this->getChildren()->count(); i++)
    {
        UIWidget* child = (UIWidget*)(this->getChildren()->objectAtIndex(i));
        child->onScaleDirtyChanged();
    }
}

void UIWidget::onScaleXDirtyChanged()
{
    this->m_bScaleXDirty = true;
    for (int i=0; i<this->getChildren()->count(); i++)
    {
        UIWidget* child = (UIWidget*)(this->getChildren()->objectAtIndex(i));
        child->onScaleXDirtyChanged();
    }
}

void UIWidget::onScaleYDirtyChanged()
{
    this->m_bScaleYDirty = true;
    for (int i=0; i<this->getChildren()->count(); i++)
    {
        UIWidget* child = (UIWidget*)(this->getChildren()->objectAtIndex(i));
        child->onScaleYDirtyChanged();
    }
}

float UIWidget::getScale()
{
    return this->m_pCCRenderNode->getScale();
}

void UIWidget::setScaleX(float scaleX)
{
    this->m_pCCRenderNode->setScaleX(scaleX);
    this->onScaleXDirtyChanged();
}

float UIWidget::getScaleX()
{
    return this->m_pCCRenderNode->getScaleX();
}

void UIWidget::setScaleY(float scaleY)
{
    this->m_pCCRenderNode->setScaleY(scaleY);
    this->onScaleYDirtyChanged();
}

float UIWidget::getScaleY()
{
    return this->m_pCCRenderNode->getScaleY();
}

void UIWidget::setRotation(float rotation)
{
    this->m_pCCRenderNode->setRotation(rotation);
}

float UIWidget::getRotation()
{
    return this->m_pCCRenderNode->getRotation();
}

void UIWidget::setRotationX(float rotationX)
{
    this->m_pCCRenderNode->setRotationX(rotationX);
}

float UIWidget::getRotationX()
{
    return this->m_pCCRenderNode->getRotationX();
}

void UIWidget::setRotationY(float rotationY)
{
    this->m_pCCRenderNode->setRotationY(rotationY);
}

float UIWidget::getRotationY()
{
    return this->m_pCCRenderNode->getRotationY();
}

void UIWidget::setSkewX(float skewX)
{
    this->m_pCCRenderNode->setSkewX(skewX);
}

float UIWidget::getSkewX()
{
    return this->m_pCCRenderNode->getSkewX();
}

void UIWidget::setSkewY(float skewY)
{
    this->m_pCCRenderNode->setSkewY(skewY);
}

float UIWidget::getSkewY()
{
    return this->m_pCCRenderNode->getSkewY();
}

void UIWidget::setVisible(bool visible)
{
    this->m_bVisibleDirty = true;
    this->updateChildrenVisibleDirty(this->m_bVisibleDirty);
    this->m_bVisible = visible;
    this->m_pCCRenderNode->setVisible(visible);
}

bool UIWidget::isVisible()
{
    return this->m_bVisible;
}

float UIWidget::getRelativeLeftPos()
{
    return this->getRelativeRect().origin.x;
}

float UIWidget::getRelativeBottomPos()
{
    return this->getRelativeRect().origin.y;
}

float UIWidget::getRelativeRightPos()
{
    cocos2d::CCRect rect = this->getRelativeRect();
    return rect.origin.x + rect.size.width;
}

float UIWidget::getRelativeTopPos()
{
    cocos2d::CCRect rect = this->getRelativeRect();
    return rect.origin.y + rect.size.height;
}

UIWidget* UIWidget::getWidgetParent()
{
    return this->m_pWidgetParent;
}

UIWidget* UIWidget::getChildByName(const char *name)
{
    for (int i=0; i<this->m_children->count(); i++)
    {
        UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
        if (strcmp(child->getName(), name) == 0)
        {
            return child;
        }
    }
    return NULL;
}

UIWidget* UIWidget::getChildByTag(int tag)
{
    for (int i=0; i<this->m_children->count(); i++)
    {
        UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
        if (child->getWidgetTag() == tag)
        {
            return child;
        }
    }
    return NULL;
}

CCArray* UIWidget::getChildren()
{
    return m_children;
}

cocos2d::CCAction* UIWidget::runAction(cocos2d::CCAction *action)
{
    return this->m_pCCRenderNode->runAction(action);
}

void UIWidget::setActionManager(cocos2d::CCActionManager *actionManager)
{
    this->m_pCCRenderNode->setActionManager(actionManager);
}

cocos2d::CCActionManager* UIWidget::getActionManager()
{
    return this->m_pCCRenderNode->getActionManager();
}

void UIWidget::stopAllActions()
{
    this->m_pCCRenderNode->stopAllActions();
}

void UIWidget::stopAction(cocos2d::CCAction *action)
{
    this->m_pCCRenderNode->stopAction(action);
}

void UIWidget::stopActionByTag(int tag)
{
    this->m_pCCRenderNode->stopActionByTag(tag);
}

cocos2d::CCAction* UIWidget::getActionByTag(int tag)
{
    return this->m_pCCRenderNode->getActionByTag(tag);
}

float UIWidget::getAbsoluteScaleX()
{
    if (this->m_bScaleXDirty)
    {
        float scaleX = this->getScaleX();
        UIWidget* parent = this->getWidgetParent();
        while (parent)
        {
            scaleX *= parent->getScaleX();
            parent = parent->getWidgetParent();
        }
        this->m_fAbsoluteScaleX = scaleX;
        this->m_bScaleXDirty = false;
    }
    return this->m_fAbsoluteScaleX;
}

float UIWidget::getAbsoluteScaleY()
{
    if (this->m_bScaleYDirty)
    {
        float scaleY = this->getScaleY();
        UIWidget* parent = this->getWidgetParent();
        while (parent)
        {
            scaleY *= parent->getScaleY();
            parent = parent->getWidgetParent();
        }
        this->m_fAbsoluteScaleY = scaleY;
        this->m_bScaleYDirty = false;
    }
    return this->m_fAbsoluteScaleY;
}

bool UIWidget::getAbsoluteVisible()
{
    if (this->m_bVisibleDirty)
    {
        UIWidget* parent = this;
        bool visible = this->isVisible();
        while (parent){
            visible &= parent->isVisible();
            if (!visible)
            {
                break;
            }
            parent = parent->getWidgetParent();
        }
        this->m_bAbsoluteVisible = visible;
        this->m_bVisibleDirty = false;
    }
    return this->m_bAbsoluteVisible;
}

void UIWidget::updateChildrenVisibleDirty(bool dirty)
{
    for (int i=0; i<this->getChildren()->count(); i++)
    {
        UIWidget* child = (UIWidget*)(this->getChildren()->objectAtIndex(i));
        child->m_bVisibleDirty = dirty;
        child->updateChildrenVisibleDirty(dirty);
    }
}

void UIWidget::updateChildrenOpacityDirty(bool dirty)
{
    for (int i = 0; i < this->getChildren()->count(); ++i)
    {
        UIWidget* child = (UIWidget*)(this->getChildren()->objectAtIndex(i));
        child->m_bOpacityDirty = dirty;
        child->updateChildrenOpacityDirty(dirty);
    }
}    

void UIWidget::adaptSize(float xProportion,float yProportion)
{
    this->m_fAdaptScaleX = xProportion;
    this->m_fAdaptScaleY = yProportion;
    this->setScaleX(m_fAdaptScaleX*this->getScaleX());
    this->setScaleY(m_fAdaptScaleY*this->getScaleY());
}

void UIWidget::setCreateFromFile(bool is)
{
    this->m_bIsCreatedFromFile = is;
}

void UIWidget::setFileDesignSize(const cocos2d::CCSize &size)
{
    this->m_fileDesignSize = size;
}

CCSize UIWidget::getFileDesignSize()
{
    return m_fileDesignSize;
}

void UIWidget::setColor(const cocos2d::ccColor3B &color)
{
    GUINodeRGBA * guiNode = DYNAMIC_CAST_GUINODERGBA;
    if (guiNode)
    {
        guiNode->setColor(color);
        return;
    }
    cocos2d::CCRGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        rgbap->setColor(color);
    }
}

const cocos2d::ccColor3B& UIWidget::getColor()
{
    return DYNAMIC_CAST_CCRGBAPROTOCOL->getColor();
}

void UIWidget::setOpacity(int opacity)
{
    GUINodeRGBA * guiNode = DYNAMIC_CAST_GUINODERGBA;
    if (guiNode)
    {
        guiNode->setOpacity(opacity);
        return;
    }
    cocos2d::CCRGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        rgbap->setOpacity(opacity);
    }
}

int UIWidget::getOpacity()
{
    return DYNAMIC_CAST_CCRGBAPROTOCOL->getOpacity();
}

void UIWidget::setBlendFunc(cocos2d::ccBlendFunc blendFunc)
{
    cocos2d::CCBlendProtocol * blendNode = DYNAMIC_CAST_CCBLENDPROTOCOL;
    if (blendNode)
    {
        blendNode->setBlendFunc(blendFunc);
    }
}

void UIWidget::ignoreAnchorPointForPosition(bool ignore)
{
    m_pCCRenderNode->ignoreAnchorPointForPosition(ignore);
}

CCPoint UIWidget::getTouchStartPos()
{
    return m_touchStartPos;
}

CCPoint UIWidget::getTouchMovePos()
{
    return m_touchMovePos;
}

CCPoint UIWidget::getTouchEndPos()
{
    return m_touchEndPos;
}

void UIWidget::setWidgetTag(int tag)
{
    m_nWidgetTag = tag;
}

int UIWidget::getWidgetTag()
{
    return m_nWidgetTag;
}

void UIWidget::setName(const char* name)
{
    m_strName = name;
}

const char* UIWidget::getName()
{
    return m_strName.c_str();
}

void UIWidget::setUseMergedTexture(bool useMergedTexture)
{
    m_bUseMergedTexture = useMergedTexture;
}

bool UIWidget::getUseMergedTexture()
{
    return m_bUseMergedTexture;
}

WidgetType UIWidget::getWidgetType()
{
    return m_WidgetType;
}

NS_CC_EXT_END
