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

#include "CocoActionManager.h"
#include "../../CocostudioReader/DictionaryHelper.h"
#include "CocoActionObject.h"

NS_CC_EXT_BEGIN

static CocoActionManager* sharedActionManager = NULL;

CocoActionManager* CocoActionManager::shareManager()
{
    if (!sharedActionManager) {
        sharedActionManager = new CocoActionManager();
    }
    return sharedActionManager;
}

CocoActionManager::CocoActionManager()
{
	m_ActionList = cocos2d::CCArray::create();
	m_ActionList->retain();
}

CocoActionManager::~CocoActionManager()
{
	m_ActionList->removeAllObjects();
	m_ActionList->release();
}

void CocoActionManager::initWithDictionary(cs::CSJsonDictionary *dic,CCNode* root)
{
    int actionCount = DICTOOL->getArrayCount_json(dic, "actionlist");
    for (int i=0; i<actionCount; i++) {
        CocoActionObject* action = new CocoActionObject();
        cs::CSJsonDictionary* actionDic = DICTOOL->getDictionaryFromArray_json(dic, "actionlist", i);
        action->initWithDictionary(actionDic,root);
        m_ActionList->addObject(action);
		CC_SAFE_DELETE(actionDic);
    }
}

CocoActionObject* CocoActionManager::getActionByName(const char* actionName)
{
    for (int i=0; i<m_ActionList->count(); i++) {
        CocoActionObject* action = dynamic_cast<CocoActionObject*>(m_ActionList->objectAtIndex(i));
        if (strcmp(actionName, action->getName()) == 0) {
            return action;
        }
    }
    return NULL;
}

void CocoActionManager::playActionByName(const char* actionName)
{
    CocoActionObject* action = getActionByName(actionName);
    if (action) {
        action->play();
    }
}

NS_CC_EXT_END