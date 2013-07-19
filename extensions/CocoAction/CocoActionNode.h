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

#ifndef __CocoActionNODE_H__
#define __CocoActionNODE_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "../BaseClasses/UIWidget.h"
#include "CocoActionFrame.h"
#include "../../CCArmature/external_tool/Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN

class CocoActionNode:public cocos2d::CCObject
{
protected:
	int currentFrameIndex;
	int destFrameIndex;
	
	float m_fUnitTime;

	int m_ActionTag;
	CCAction* m_action;
	CCNode* m_actionNode;

	cocos2d::CCArray* m_ActionArray;

public:
    CocoActionNode();
    virtual ~CocoActionNode();

	void setActionTag(int tag);
	int getActionTag();

	void setActionNode(CCNode* node);
	CCNode* getActionNode();

	void insertFrame(int index, CocoActionFrame* frame);
	void addFrame(CocoActionFrame* frame);
	void deleteFrame(CocoActionFrame* frame );
	void clearAllFrame();

	void runAction(bool bloop);
	void stopAction();

	bool updateActionToTimeLine(float fTime);

	void easingToFrame(float duration,float delayTime,CocoActionFrame* destFrame);

	virtual void initActionNodeFromRoot(CCObject* root);
    virtual void initWithDictionary(cs::CSJsonDictionary* dic,CCObject* root);
};

NS_CC_EXT_END

#endif
