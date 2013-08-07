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

#include "CocoActionNode.h"
#include "CocoActionNodeFrame.h"
#include "../../CocostudioReader/DictionaryHelper.h"
#include "../System/UIHelper.h"

NS_CC_EXT_BEGIN

	CocoActionNode::CocoActionNode()
{
	currentFrameIndex = 0;
	m_actionNode = NULL;

	m_action = NULL;

	m_ActionArray = cocos2d::CCArray::create();
	m_ActionArray->retain();

	int num = (int)kKeyframeMax;
	for(int i = 0; i < num; i++)
	{
		cocos2d::CCArray* cArray = cocos2d::CCArray::create();
		m_ActionArray->addObject(cArray);
	}
}

CocoActionNode::~CocoActionNode()
{
	if (m_action != NULL)
	{
		m_action->release();
	}

	for(int i = 0; i < m_ActionArray->count(); i++)
	{
		m_ActionArray[i].removeAllObjects();
		m_ActionArray[i].release();
	}
	m_ActionArray->removeAllObjects();
	m_ActionArray->release();
}

void CocoActionNode::initWithDictionary(cs::CSJsonDictionary *dic,CCObject* root)
{
	setActionTag(DICTOOL->getIntValue_json(dic, "actiontag"));
	
	int actionFrameCount = DICTOOL->getArrayCount_json(dic, "actionframelist");
	for (int i=0; i<actionFrameCount; i++) {

		cs::CSJsonDictionary* actionFrameDic = DICTOOL->getDictionaryFromArray_json(dic, "actionframelist", i);
		FrameType frameType = (FrameType)DICTOOL->getIntValue_json(actionFrameDic, "frametype");
		CCArray* cActionArray = (CCArray*)m_ActionArray->objectAtIndex(frameType);
		CocoActionFrame* actionFrame = NULL;
		switch (frameType)
		{
		case kKeyframeMove:
			{
				actionFrame = new CocoActionMoveFrame();
				((CocoActionMoveFrame*)actionFrame)->setPosition(ccp(DICTOOL->getFloatValue_json(dic, "positionx"), DICTOOL->getFloatValue_json(dic, "positiony")));
			}
			break;
		case kKeyframeScale:
			{
				actionFrame = new CocoActionScaleFrame();
				((CocoActionScaleFrame*)actionFrame)->setScaleX(DICTOOL->getFloatValue_json(dic, "scalex"));
				((CocoActionScaleFrame*)actionFrame)->setScaleY(DICTOOL->getFloatValue_json(dic, "scaley"));
			}
			break;
		case kKeyframeRotate:
			{
				actionFrame = new CocoActionRotationFrame();
				((CocoActionRotationFrame*)actionFrame)->setRotation(DICTOOL->getFloatValue_json(dic, "rotation"));
			}
			break;
		case kKeyframeTint:
			{
				actionFrame= new CocoActionFadeFrame();
				((CocoActionFadeFrame*)actionFrame)->setOpacity(DICTOOL->getIntValue_json(dic, "opacity"));
			}
			break;
		case kKeyframeFade:
			{
				actionFrame = new CocoActionTintFrame();
				((CocoActionTintFrame*)actionFrame)->setColor(ccc3(DICTOOL->getIntValue_json(dic, "colorr"), DICTOOL->getIntValue_json(dic, "colorg"), DICTOOL->getIntValue_json(dic, "colorb")));
			}
			break;
		}
		if (cActionArray != NULL && actionFrame != NULL)
		{
			cActionArray->addObject(actionFrame);
		}
		CC_SAFE_DELETE(actionFrameDic);
	}
	initActionNodeFromRoot(root);
}
void CocoActionNode::initActionNodeFromRoot(CCObject* root)
{
	CCLOG("Need a definition of <initActionNodeFromRoot>");
}

void CocoActionNode::setActionTag(int tag)
{
	m_ActionTag = tag;
}
int CocoActionNode::getActionTag()
{
	return m_ActionTag;
}
void CocoActionNode::setActionNode(CCNode* node)
{
	m_actionNode = node;
}
CCNode* CocoActionNode::getActionNode()
{
	return m_actionNode;
}
void CocoActionNode::insertFrame(int index, CocoActionFrame* frame)
{
	if (frame == NULL)
	{
		return;
	}
	int frameType = frame->getFrameType();
	cocos2d::CCArray* cArray = (cocos2d::CCArray*)m_ActionArray->objectAtIndex(frameType);
	if (cArray == NULL)
	{
		return;
	}	
	cArray->insertObject(frame,index);
}

void CocoActionNode::addFrame(CocoActionFrame* frame)
{
	if (frame == NULL)
	{
		return;
	}
	int frameType = frame->getFrameType();
	cocos2d::CCArray* cArray = (cocos2d::CCArray*)m_ActionArray->objectAtIndex(frameType);
	if (cArray == NULL)
	{
		return;
	}
	cArray->addObject(frame);
}

void CocoActionNode::deleteFrame(CocoActionFrame* frame)
{
	if (frame == NULL)
	{
		return;
	}
	int frameType = frame->getFrameType();
	cocos2d::CCArray* cArray = (cocos2d::CCArray*)m_ActionArray->objectAtIndex(frameType);
	if (cArray == NULL)
	{ 
		return;
	}
	cArray->removeObject(frame);
}

void CocoActionNode::clearAllFrame()
{
	for(int i = 0; i < m_ActionArray->count(); i++)
	{
		m_ActionArray[i].removeAllObjects();
	}
}

void CocoActionNode::runAction(bool bloop)
{
	if ( m_actionNode == NULL )
	{
		return;
	}

	CCArray* cSpawnArray = CCArray::create();
	for (int n = 0; n < m_ActionArray->count(); n++)
	{
		cocos2d::CCArray* cArray = (cocos2d::CCArray*)(m_ActionArray->objectAtIndex(n));
		if (cArray == NULL)
		{
			continue;
		}

		CCArray* cSequenceArray = CCArray::create();
		for (int i = 0; i < cArray->count(); i++)
		{
			CocoActionFrame* frame = (CocoActionFrame*)(cArray->objectAtIndex(i));
			if (i == 0)
			{
			}
			else
			{
				CocoActionFrame* srcFrame = (CocoActionFrame*)(cArray->objectAtIndex(i-1));
				float duration = frame->getFrameTime() - srcFrame->getFrameTime();
				CCAction* cAction = frame->getAction(duration);
				cSequenceArray->addObject(cAction);
			}
		}
		CCSequence* cSequence = CCSequence::create(cSequenceArray);
		cSpawnArray->addObject(cSequence);
	}
	CCSpawn * actionSpawn = CCSpawn::create(cSpawnArray);

	if (bloop)
	{
		if (m_actionNode)
		{
			if (m_action!=NULL)
			{
				m_action->release();
			}
			m_action = CCRepeatForever::create(actionSpawn);
			m_action->retain();
		}
	}
	else
	{
		if (m_actionNode)
		{
			if (m_action!=NULL)
			{
				m_action->release();
			}
			m_action = CCSequence::create(actionSpawn);
			m_action->retain();
		}
	}
}

void CocoActionNode::stopAction()
{
	if (m_actionNode) {
		m_actionNode->stopAction(m_action);
	}
}

bool CocoActionNode::updateActionToTimeLine(float fTime)
{
	bool bFindFrame = false;

	CocoActionFrame* srcFrame = NULL;
	CocoActionFrame* destFrame = NULL;
	for (int n = 0; n < m_ActionArray->count(); n++)
	{
		cocos2d::CCArray* cArray = (cocos2d::CCArray*)(m_ActionArray->objectAtIndex(n));
		if (cArray == NULL)
		{
			continue;
		}
		for (int i = 0; i < cArray->count(); i++)
		{
			CocoActionFrame* frame = (CocoActionFrame*)(cArray->objectAtIndex(i));
			if (frame->getFrameTime() > fTime)
			{
				if (i == 0)
				{
				}
				else
				{
					srcFrame = (CocoActionFrame*)(cArray->objectAtIndex(i-1));
					float duration = frame->getFrameTime() - srcFrame->getFrameTime();
					float delaytime = frame->getFrameTime() - fTime;
					this->easingToFrame(duration,delaytime,frame);
					bFindFrame = true;
				}
				break;
			}
		}
	}
	return bFindFrame;
}
void CocoActionNode::easingToFrame(float duration,float delayTime,CocoActionFrame* destFrame)
{
	CCAction* cAction = destFrame->getAction(duration);
	if (cAction==NULL)
	{
		return;
	}
	
	cAction->setTarget(m_actionNode);
	cAction->update(delayTime);
}
NS_CC_EXT_END