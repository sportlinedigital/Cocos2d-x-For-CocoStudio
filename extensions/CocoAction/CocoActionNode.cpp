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
#include "../../JsonReader/DictionaryHelper.h"
#include "../System/UIHelper.h"

NS_CC_EXT_BEGIN

CocoActionNode::CocoActionNode()
{
	currentFrameIndex = 0;
	m_actionNode = NULL;

	m_action = NULL;

	m_ActionMoveFrameList = cocos2d::CCArray::create();
	m_ActionMoveFrameList->retain();

	m_ActionScaleFrameList = cocos2d::CCArray::create();
	m_ActionScaleFrameList->retain();

	m_ActionRotateFrameList = cocos2d::CCArray::create();
	m_ActionRotateFrameList->retain();

	m_ActionTintFrameList = cocos2d::CCArray::create();
	m_ActionTintFrameList->retain();

	m_ActionFadeFrameList = cocos2d::CCArray::create();
	m_ActionFadeFrameList->retain();

}

CocoActionNode::~CocoActionNode()
{
	if (m_action != NULL)
	{
		m_action->release();
	}

	m_ActionMoveFrameList->removeAllObjects();
	m_ActionMoveFrameList->release();

	m_ActionScaleFrameList->removeAllObjects();
	m_ActionScaleFrameList->release();

	m_ActionRotateFrameList->removeAllObjects();
	m_ActionRotateFrameList->release();

	m_ActionTintFrameList->removeAllObjects();
	m_ActionTintFrameList->release();

	m_ActionFadeFrameList->removeAllObjects();
	m_ActionFadeFrameList->release();
}

void CocoActionNode::initWithDictionary(cs::CSJsonDictionary *dic,CCObject* root)
{
    //setActionTag(DICTOOL->getIntValue_json(dic, "actiontag"));
    //int actionFrameCount = DICTOOL->getArrayCount_json(dic, "actionframelist");
    //SetActionNode(CCUIHELPER->seekActionWidgetByActionTag(root, getActionTag()));
    //for (int i=0; i<actionFrameCount; i++) {
    //    CocoActionFrame* actionFrame = new CocoActionFrame();
    //    cs::CSJsonDictionary* actionFrameDic = DICTOOL->getDictionaryFromArray_json(dic, "actionframelist", i);
    //    actionFrame->initWithDictionary(actionFrameDic);
    //    m_ActionFrameList->addObject(actionFrame);
    //}
}
void CocoActionNode::setActionTag(int tag)
{
	m_ActionTag = tag;
}
int CocoActionNode::getActionTag()
{
	return m_ActionTag;
}
void CocoActionNode::setActionNode(CCObject* node)
{
	m_actionNode = node;
}
void CocoActionNode::insertFrame(int index, CocoActionFrame* frame)
{
	m_ActionNodeFrameList->insertObject(frame,index);
}

void CocoActionNode::addFrame(CocoActionFrame* frame)
{
	m_ActionNodeFrameList->addObject(frame);
}

void CocoActionNode::deleteFrame(CocoActionFrame* frame)
{
	m_ActionNodeFrameList->removeObject(frame);
}

void CocoActionNode::clearAllFrame()
{
	m_ActionNodeFrameList->removeAllObjects();
}

void CocoActionNode::updateToFrameByIndex(int index)
{
	currentFrameIndex = index;

	int frameNum = m_ActionNodeFrameList->count();

	bool bFindFrame = false;

	CocoActionNodeFrame* frame = NULL;
	for (int i = 0; i < frameNum; i++)
	{
		frame = (CocoActionNodeFrame*)m_ActionNodeFrameList->objectAtIndex(index);
		if (frame->getFrameId() == index)
		{
			bFindFrame = true;
			//updateToFrame(frame);
			break;
		}
	}

	if (!bFindFrame)
	{
		setVisiable(false);
	}
}

void CocoActionNode::updateToFrame(CocoActionFrame* frame)
{

}

void CocoActionNode::runAction(float fUnitTime, bool bloop)
{
	int frameNum = m_ActionNodeFrameList->count();

	if ( m_actionNode == NULL || frameNum <= 0 )
	{
		return;
	}	

	CCArray* actionFrame = new CCArray();

	for ( int i = 0; i < frameNum; i++ )
	{
		float duration;

		CocoActionNodeFrame* frame = (CocoActionNodeFrame*)m_ActionNodeFrameList->objectAtIndex(i);

		if ( i == 0 )
		{
			//duration = frame->getFrameId() * fUnitTime;
			duration = 0.01f;
		}
		else
		{
			CocoActionNodeFrame* frame_pre = (CocoActionNodeFrame*)m_ActionNodeFrameList->objectAtIndex(i-1);
			duration = (frame->getFrameId() - frame_pre->getFrameId()) * fUnitTime;
		}

		//CCMoveTo* action_1 = CCMoveTo::create(duration,frame->getPosition());
		//CCRotateTo* action_2 = CCRotateTo::create(duration,frame->getRotation());
		//CCScaleTo* action_3 = CCScaleTo::create(duration,frame->getScaleX(),frame->getScaleY());
		//CCFadeTo* action_4 = CCFadeTo::create(duration,frame->getOpacity());
		//CCTintTo* action_5 = CCTintTo::create(duration,frame->getColor().r,frame->getColor().g,frame->getColor().b);

		//CCSpawn * actionSpawn = CCSpawn::create(action_1,action_2,action_3,action_4,action_5, NULL);
		//actionFrame->addObject( actionSpawn );
	}

	if (bloop)
	{
        CCActionInterval* actionInterval = dynamic_cast<CCActionInterval*>(CCSequence::create(actionFrame)); 
        if (actionInterval)
        {
            if (m_actionNode) {
				if (m_action!=NULL)
				{
					m_action->release();
				}
				m_action = CCRepeatForever::create(actionInterval);
				m_action->retain();
                
				runAction(m_action);
            }
        }
	}
	else
	{
        if (m_actionNode) {
			if (m_action!=NULL)
			{
				m_action->release();
			}
			m_action = CCSequence::create(actionFrame);
			m_action->retain();
            
			runAction(m_action);
        }
	}
}

void CocoActionNode::stopAction()
{
    if (m_actionNode) {
		stopAction(m_action);
    }
}

bool CocoActionNode::updateAction(float dt)
{
	currentFrameTime += dt;

	int frameNum = m_ActionNodeFrameList->count();
	if (currentFrameTime > frameNum * m_fUnitTime)
	{
		return false;
	}
	
	return true;
}
bool CocoActionNode::updateActionToTimeLine(float fTime)
{
	int frameNum = m_ActionNodeFrameList->count();
	for (int i = 0; i < frameNum; i++)
	{
		CocoActionFrame* frame = (CocoActionFrame*)m_ActionNodeFrameList->objectAtIndex(i);
		//float frameTimeLine = frame->getStartTime();
		//if ( frameTimeLine > fTime )
		//{
		//	float duration = frameTimeLine - fTime;
		//	
		//	break;
		//}
		
	}
	
	return false;
}

float CocoActionNode::getCurrrentTime()
{
	return currentFrameTime;
}

void CocoActionNode::runAction(CCAction* action)
{

}
void CocoActionNode::stopAction(CCAction* action)
{

}

void CocoActionNode::setVisiable(bool bAble)
{

}
NS_CC_EXT_END