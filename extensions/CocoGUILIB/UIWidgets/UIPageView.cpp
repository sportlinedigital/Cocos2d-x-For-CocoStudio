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
: state_(0)
, stealTouches(true)
, minimumTouchLengthToSlide(10.0)
, minimumTouchLengthToChangePage(20.0)
, marginOffset(0)
, pagesWidthOffset(0)
, currentScreen_(0)
, isRunningAction(false)
, m_fTouchStartLocation(0.0)
, m_fTouchMoveStartLocation(0.0)
, m_fTouchEndLocation(0.0)
, m_fDisBoundaryToChild_0(0.0)
, m_fDisBetweenChild(0.0)
, m_fDragForce(0.0)
, m_eDirection(PAGEVIEW_DIR_VERTICAL)
{
    
}

UIPageView::~UIPageView()
{
    
}

UIPageView* UIPageView::create()
{
    UIPageView* pRet = new UIPageView();
    
    if (pRet && pRet->init())
    {
        return pRet;
    }
    CC_SAFE_DELETE(pRet);        
    return NULL;
}

bool UIPageView::init()
{
    if (UIPanel::init())
    {
        return true;
    }        
    return false;
}

bool UIPageView::addChild(UIWidget *widget)
{
    UIPanel::addChild(widget);
    
    initProperty();
    
    return true;
}

void UIPageView::initProperty()
{
    resetProperty();
}

void UIPageView::resetProperty()
{
    float scroll_top = getRect().size.height;
    float scroll_left = 0;
    
    switch (m_children->count())
    {
        case 1:
            {
                UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
                
                switch (m_eDirection)
                {
                    case PAGEVIEW_DIR_VERTICAL: // vertical
                        {
                            float child_0_top = child_0->getRelativeTopPos();
                            float offset = scroll_top - child_0_top;
                            m_fDisBoundaryToChild_0 = (offset > 0) ? offset : 0;
                        }
                        break;
                        
                    case PAGEVIEW_DIR_HORIZONTAL: // horizontal
                        {
                            float child_0_left = child_0->getRelativeLeftPos();
                            float offset = child_0_left - scroll_left;
                            m_fDisBoundaryToChild_0 = (offset > 0) ? offset : 0;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            break;
            
        default:
            {
                UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
                UIWidget* child_1 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(1));
                
                switch (m_eDirection)
                {
                    case PAGEVIEW_DIR_VERTICAL: // vertical
                        {
                            float child_0_top = child_0->getRelativeTopPos();
                            float offset = scroll_top - child_0_top;
                            m_fDisBoundaryToChild_0 = (offset > 0) ? offset : 0;
                            m_fDisBetweenChild = child_0->getPosition().y - child_1->getPosition().y;
                        }
                        break;
                        
                    case PAGEVIEW_DIR_HORIZONTAL: // horizontal
                        {
                            float child_0_left = child_0->getRelativeLeftPos();
                            float offset = child_0_left - scroll_left;
                            m_fDisBoundaryToChild_0 = (offset > 0) ? offset : 0;
                            m_fDisBetweenChild = child_1->getPosition().x - child_0->getPosition().x;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            break;
    }
}

//    void UIPageView::setColorAndSize(int r, int g, int b, int o, float width, float height)
//    {
//        CocoPanel::setColorAndSize(r, g, b, o, width, height);
//        marginOffset = width / 8 * 5;
//        m_fDragForce = width / 8 * 5;
////        marginOffset = m_fWidth;
//    }

void UIPageView::setSize(const CCSize &size)
{
    UIPanel::setSize(size);
    marginOffset = size.width / 8 * 5;
    m_fDragForce = size.width / 8 * 5;
//        marginOffset = m_fWidth;
}

void UIPageView::updatePages()
{
    // Loop through the array and add the screens if needed.
    UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    float child_0_x = child_0->getRelativeLeftPos();
    float child_0_y = child_0->getRelativeBottomPos();
    
    float w = getRect().size.width;
//        float h = getRect().size.height;
    
    for (int i = 0; i < m_children->count(); ++i)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(m_children->objectAtIndex(i));
        child->setPosition(ccp(child_0_x + (i * (w - pagesWidthOffset)), child_0_y));
    }
}

void UIPageView::addPage(UIWidget *widget, int pageNumber)
{
    pageNumber = MIN(pageNumber, m_children->count());
    pageNumber = MAX(pageNumber, 0);
    
    addChild(widget);
    updatePages();
    moveToPage(currentScreen_);
}

void UIPageView::addPage(UIWidget *widget)
{
    addPage(widget, m_children->count());
}

void UIPageView::removePage(UIWidget *widget)
{
    removeChild(widget, true);
    updatePages();
    
    prevScreen_ = currentScreen_;
    currentScreen_ = MIN(currentScreen_, m_children->count() - 1);
    moveToPage(currentScreen_);        
}

void UIPageView::removePageWithNumber(int page)
{
    if (page >= 0 && page < m_children->count())
    {
        removePage(dynamic_cast<UIWidget*>(m_children->objectAtIndex(page)));            
    }
}

#pragma mark Moving To / Selecting Pages
void UIPageView::moveToPage(int page)
{
    if (page < 0 || page >= m_children->count())
    {
        CCLOGERROR("UIPageView#moveToPage: %d - wrong page number, out of bounds. ", page);
        return;
    }
    
    UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    float child_0_x = child_0->getRelativeLeftPos();
    float child_0_y = child_0->getRelativeBottomPos();
    CCPoint des = positionForPageWithNumber(page);
    CCPoint delta = ccp(des.x + m_fDisBoundaryToChild_0 - child_0_x, des.y - child_0_y);
    
    int times = m_children->count();
    for (int i = 0; i < times - 1; ++i)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(m_children->objectAtIndex(i));
        CCMoveBy* moveBy = CCMoveBy::create(0.3, delta);
        child->runAction(moveBy);
    }
    
    UIWidget* child_last = dynamic_cast<UIWidget*>(m_children->lastObject());
    CCMoveBy* moveBy = CCMoveBy::create(0.3, delta);
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(UIPageView::moveToPageEnded));
    CCSequence* seq = CCSequence::create(moveBy, callFunc, NULL);
    child_last->runAction(seq);
    
    isRunningAction = true;
    
    currentScreen_ = page;        
}

void UIPageView::selectPage(int page)
{
    if (page < 0 || page >= m_children->count()) {
        CCLOGERROR("UIPageView#selectPage: %d - wrong page number, out of bounds. ", page);
        return;
    }
    
    UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    float child_0_x = child_0->getPosition().x;
    float child_0_y = child_0->getPosition().y;
    CCPoint des = positionForPageWithNumber(page);
    CCPoint delta = ccp(des.x + m_fDisBoundaryToChild_0 - child_0_x, des.y - child_0_y);
    
    int times = m_children->count();
    for (int i = 0; i < times; ++i)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(m_children->objectAtIndex(i));
        child->setPosition(ccp(child->getPosition().x + delta.x, child->getPosition().y + delta.y));
    }
            
    prevScreen_ = currentScreen_;
    currentScreen_ = page;
}

void UIPageView::moveToPageEnded()
{
    isRunningAction = false;
    // pipu later test
//        prevScreen_ = currentScreen_ = pageNumberForPosition(getPosition());
}

int UIPageView::pageNumberForPosition(cocos2d::CCPoint position)
{
    float w = getRect().size.width;
    float pageFloat = - position.x / (w - pagesWidthOffset);
    int pageNumber = ceilf(pageFloat);
    if ( (float)pageNumber - pageFloat  >= 0.5f)
    {
        pageNumber--;
    }
    
    pageNumber = MAX(0, pageNumber);
    pageNumber = MIN(m_children->count() - 1, pageNumber);
    
    return pageNumber;
}

CCPoint UIPageView::positionForPageWithNumber(int pageNumber)
{
    UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
    float child_0_y = child_0->getRelativeBottomPos();
    
    float w = getRect().size.width;
    return ccp(-pageNumber * (w - pagesWidthOffset), child_0_y);
}

void UIPageView::moveChildren(float offset)
{
    for (int i=0;i<this->m_children->count();i++)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(m_children->objectAtIndex(i));
        child->setPosition(ccp(child->getPosition().x + offset, child->getPosition().y));
        child->setVisible(child->checkBeVisibleInParent());
    }
}

void UIPageView::stopAction()
{
    int times = m_children->count();
    for (int i = 0; i < times; ++i)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(m_children->objectAtIndex(i));
        child->stopAllActions();
    }
}

bool UIPageView::onTouchBegan(cocos2d::CCPoint &touchPoint)
{
    UIWidget::onTouchBegan(touchPoint);
    
    cocos2d::CCPoint nsp = this->m_pCCRenderNode->convertToNodeSpace(touchPoint);
    m_fTouchMoveStartLocation = nsp.x;
    m_fTouchStartLocation = nsp.x;
    
    startSwipe_ = nsp.x;
    state_ = kCCScrollLayerStateIdle;
    
    if (isRunningAction)
    {
        isRunningAction = false;
        stopAction();
    }
    
    return true;
}

bool UIPageView::onTouchMoved(cocos2d::CCPoint &touchPoint)
{
    UIWidget::onTouchMoved(touchPoint);
    
    cocos2d::CCPoint nsp = m_pCCRenderNode->convertToNodeSpace(touchPoint);
    float moveX = nsp.x;
    float distance = moveX - m_fTouchMoveStartLocation;
    m_fTouchMoveStartLocation = moveX;
    
    // If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
    // Of course only if we not already in sliding mode
    if ( (state_ != kCCScrollLayerStateSliding)
        && (fabsf(nsp.x - startSwipe_) >= minimumTouchLengthToSlide) )
    {
        state_ = kCCScrollLayerStateSliding;
        
        // Avoid jerk after state change.
        startSwipe_ = moveX;
        
        // move page start event
        // later
    }
    
    if (state_ == kCCScrollLayerStateSliding)
    {
        float w = getRect().size.width;
        float desiredX = (-currentScreen_ * (w - pagesWidthOffset)) + moveX - startSwipe_;
        int page = pageNumberForPosition(ccp(desiredX, 0));
        float offset = distance;
//            float offset = desiredX - positionForPageWithNumber(page).x;
        
        if ((page == 0 && offset > 0) || (page == m_children->count() - 1 && offset < 0))
        {
            offset -= marginOffset * offset / m_fWidth;
        }
        moveChildren(offset);
    }
    
    return true;
}

bool UIPageView::onTouchEnded(cocos2d::CCPoint &touchPoint)
{
    UIPanel::onTouchEnded(touchPoint);
    
    cocos2d::CCPoint nsp = this->m_pCCRenderNode->convertToNodeSpace(touchPoint);
    m_fTouchEndLocation = nsp.x;
    
    int selectedPage = currentScreen_;
    float delta = nsp.x - startSwipe_;
    
    if (fabsf(delta) >= minimumTouchLengthToChangePage)
    {
        UIWidget* child_0 = dynamic_cast<UIWidget*>(m_children->objectAtIndex(0));
        
        CCPoint point = ccp(child_0->getRelativeLeftPos(), child_0->getRelativeBottomPos());
        selectedPage = pageNumberForPosition(point);
        if (selectedPage == currentScreen_)
        {
            if (delta < 0.f && selectedPage < m_children->count() - 1)
            {
                selectedPage++;                    
            }
            else if (delta > 0.f && selectedPage > 0)
            {
                selectedPage--;
            }
        }
    }
    moveToPage(selectedPage);
    
    return true;
}

bool UIPageView::onTouchCancelled(cocos2d::CCPoint &touchPoint)
{
    UIPanel::onTouchCancelled(touchPoint);
//        selectPage(currentScreen_);
    return true;
}

NS_CC_EXT_END