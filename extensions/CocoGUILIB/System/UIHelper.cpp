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

#include "UIHelper.h"
#include "../../CocostudioReader/DictionaryHelper.h"
#include "../UIWidgets/UIButton.h"
#include "../UIWidgets/UICheckBox.h"
#include "../UIWidgets/UIImageView.h"
#include "../UIWidgets/UILabel.h"
#include "../UIWidgets/UILabelAtlas.h"
#include "../UIWidgets/UIPanel.h"
#include "../UIWidgets/UIScrollView.h"
#include "../UIWidgets/UILoadingBar.h"
#include "../UIWidgets/UISlider.h"
#include "../UIWidgets/UITextArea.h"
#include "../UIWidgets/UITextButton.h"
#include "../UIWidgets/UITextField.h"
#include "cocos2d.h"
#include "CCSReader.h"


NS_CC_EXT_BEGIN
    
static UIHelper* helperInstance = NULL;

UIHelper* UIHelper::instance()
{
    if (!helperInstance)
    {
        helperInstance = new UIHelper();
    }
    return helperInstance;
}

void UIHelper::purgeUIHelper()
{
	CC_SAFE_DELETE(helperInstance);
}

UIHelper::UIHelper():
m_textureFiles(NULL)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_fFileDesignWidth = winSize.width;
    m_fFileDesignHeight = winSize.height;
    init();
}

UIHelper::~UIHelper()
{
    cocos2d::extension::CCSReader::purgeCCSReader();
}

void UIHelper::init()
{
    m_textureFiles = CCArray::create();
    m_textureFiles->retain();
}

UIWidget* UIHelper::createWidgetFromJsonFile(const char *fileName)
{
    return CCSReader::shareReader()->widgetFromJsonFile(fileName);
}

UIWidget* UIHelper::createWidgetFromJsonFileWithAdapt(const char *fileName, bool scaleAdapt, bool equalProportions)
{
    UIWidget* widget = createWidgetFromJsonFile(fileName);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    adjustWidgetProperty(widget, winSize.width/getFileDesignWidth(),winSize.height/getFileDesignHeight(), scaleAdapt, equalProportions);
    return widget;
}

void UIHelper::adjustWidgetProperty(UIWidget* root,float xProportion,float yProportion,bool scaleAdapt,bool equalProportions)
{
    switch (root->getWidgetType())
    {
        case 0:
            if (root->getWidgetParent()->getWidgetType() == 1)
            {
                if (scaleAdapt)
                {
                    if (equalProportions)
                    {
//                            root->setScale(xProportion > yProportion ? xProportion : yProportion);
                        float res = xProportion > yProportion ? xProportion : yProportion;
                        root->adaptSize(res, res);
                    }
                    else
                    {
                        root->adaptSize(xProportion, yProportion);
                    }
                }
            }
            break;
        case 1:
        {
            UIContainerWidget* cRoot = (UIContainerWidget*)root;
            cRoot->setSize(CCSize(cRoot->getWidth()*xProportion,cRoot->getHeight()*yProportion));
        }
            break;
        default:
            break;
    }
    if (!root->getWidgetParent() || root->getWidgetParent()->getWidgetType() == 1)
    {
        root->setPosition(ccp(root->getPosition().x*xProportion, root->getPosition().y*yProportion));

    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int rootChildrenCount = arrayRootChildren->num;
    for (int i=0; i<rootChildrenCount; i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        adjustWidgetProperty(child,xProportion,yProportion,scaleAdapt,equalProportions);
    }
}

void UIHelper::addSpriteFrame(const char *fileName)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    ccArray* arrayTextures = m_textureFiles->data;
    int length = arrayTextures->num;
    for (int i=0;i<length;i++)
    {
        CCString* file = (CCString*)(arrayTextures->arr[i]);
        if (strcmp(file->m_sString.c_str(), fileName) == 0)
        {
            return;
        }
    }
    m_textureFiles->addObject(CCString::create(fileName));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileName);
}

void UIHelper::removeSpriteFrame(const char *fileName)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    ccArray* arrayTextures = m_textureFiles->data;
    int length = arrayTextures->num;
    for (int i=0;i<length;i++)
    {
        CCString* file = (CCString*)(arrayTextures->arr[i]);
        if (strcmp(file->m_sString.c_str(), fileName) == 0)
        {
            CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(fileName);
            m_textureFiles->removeObject(file);
            return;
        }
    }
}

void UIHelper::removeAllSpriteFrame()
{
    ccArray* arrayTextures = m_textureFiles->data;
    int length = arrayTextures->num;
    for (int i=0;i<length;i++)
    {
        CCString* file = (CCString*)(arrayTextures->arr[i]);
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(file->m_sString.c_str());
    }
    m_textureFiles->removeAllObjects();
}

UIWidget* UIHelper::seekWidgetByTag(UIWidget* root, int tag)
{
    if (!root)
    {
        return NULL;
    }
    if (root->getWidgetTag() == tag)
    {
        return root;
    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=0;i<length;i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        UIWidget* res = seekWidgetByTag(child,tag);
        if (res != NULL)
        {
            return res;
        }
    }
    return NULL;
}

UIWidget* UIHelper::seekWidgetByName(UIWidget* root, const char *name)
{
    if (!root)
    {
        return NULL;
    }
    if (strcmp(root->getName(), name) == 0)
    {
        return root;
    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=0;i<length;i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        UIWidget* res = seekWidgetByName(child,name);
        if (res != NULL)
        {
            return res;
        }
    }
    return NULL;
}

UIWidget* UIHelper::seekActionWidgetByActionTag(UIWidget* root, int tag)
{
	if (!root)
	{
		return NULL;
	}
	if (root->getActionTag() == tag)
	{
		return root;
	}
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
	for (int i=0;i<length;i++)
	{
		UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
		UIWidget* res = seekActionWidgetByActionTag(child,tag);
		if (res != NULL)
		{
			return res;
		}
	}
	return NULL;
}

UIWidget* UIHelper::seekWidgetByRelativeName(UIWidget *root, const char *name)
{
    if (!root)
    {
        return NULL;
    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=0;i<length;i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        if (strcmp(child->getRelativeLayoutName(), name) == 0)
        {
            return child;
        }
    }
    return NULL;
}

void UIHelper::setFileDesignWidth(float width)
{
    m_fFileDesignWidth = width;
}

float UIHelper::getFileDesignWidth()
{
    return m_fFileDesignWidth;
}

void UIHelper::setFileDesignHeight(float height)
{
    m_fFileDesignHeight = height;
}

float UIHelper::getFileDesignHeight()
{
    return m_fFileDesignHeight;
}

NS_CC_EXT_END