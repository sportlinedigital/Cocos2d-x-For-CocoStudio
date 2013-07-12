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

#include "CocoActionObject.h"
#include "CocoActionNode.h"
#include "../../JsonReader/DictionaryHelper.h"


NS_CC_EXT_BEGIN

CocoActionObject::CocoActionObject()
{
	m_ActionNodeList = cocos2d::CCArray::create();
	m_ActionNodeList->retain();
}

CocoActionObject::~CocoActionObject()
{
	m_ActionNodeList->removeAllObjects();
	m_ActionNodeList->release();
}

void CocoActionObject::initWithDictionary(cs::CSJsonDictionary *dic,CCObject* root)
{
    setName(DICTOOL->getStringValue_json(dic, "name"));
    setLoop(DICTOOL->getBooleanValue_json(dic, "loop"));
    setUnitTime(DICTOOL->getFloatValue_json(dic, "unittime"));
    int actionNodeCount = DICTOOL->getArrayCount_json(dic, "actionnodelist");
    for (int i=0; i<actionNodeCount; i++) {
        CocoActionNode* actionNode = new CocoActionNode();
        cs::CSJsonDictionary* actionNodeDic = DICTOOL->getDictionaryFromArray_json(dic, "actionnodelist", i);
        actionNode->initWithDictionary(actionNodeDic,root);
        m_ActionNodeList->addObject(actionNode);
    }
}

void CocoActionObject::play()
{
    stop();
	int frameNum = m_ActionNodeList->count();
	for ( int i = 0; i < frameNum; i++ )
	{
		CocoActionNode* actionNode = (CocoActionNode*)m_ActionNodeList->objectAtIndex(i);
		actionNode->runAction( getUnitTime(),getLoop() );
	}
}

void CocoActionObject::pause()
{
	m_bPause = true;
}

void CocoActionObject::stop()
{
	int frameNum = m_ActionNodeList->count();

	for ( int i = 0; i < frameNum; i++ )
	{
		CocoActionNode* actionNode = (CocoActionNode*)m_ActionNodeList->objectAtIndex(i);
		actionNode->stopAction();
	}
}

void CocoActionObject::updateToFrameByIndex(int index)
{
	int frameNum = m_ActionNodeList->count();

	for ( int i = 0; i < frameNum; i++ )
	{
		CocoActionNode* actionNode = (CocoActionNode*)m_ActionNodeList->objectAtIndex(i);

		actionNode->updateToFrameByIndex(index);
	}
}

void CocoActionObject::scheduleUpdate()
{
	m_pScheduler->scheduleUpdateForTarget(this,0,m_bPause);
}
void CocoActionObject::unscheduleUpdate()
{
	m_pScheduler->unscheduleUpdateForTarget(this);
}

void CocoActionObject::runSimulationAction()
{
	m_pScheduler->scheduleSelector(schedule_selector(CocoActionObject::simulationActionUpdate), this, 0.0f , kCCRepeatForever, 0.0f, m_bPause);
}
void CocoActionObject::simulationActionUpdate(float dt)
{
	m_CurrentTime += dt;

	int frameNum = m_ActionNodeList->count();
	bool bEnd = true;
	for (int i = 0; i < frameNum; i++)
	{
		CocoActionNode* actionNode = (CocoActionNode*)m_ActionNodeList->objectAtIndex(i);
		if (actionNode->updateAction(dt))
		{
			bEnd = false;
		}
	}

	if (bEnd)
	{
		stopSimulationAction();
	}
	
}
void CocoActionObject::stopSimulationAction()
{
	m_pScheduler->unscheduleSelector(schedule_selector(CocoActionObject::simulationActionUpdate), this);
}


//void CocoActionNode::runSimulationAction(float fUnitTime, bool bloop)
//{
//	currentFrameTime = 0.0f;
//	m_fUnitTime = fUnitTime;
//	int frameNum = m_ActionFrameList->count();
//	if ( m_actionNode == NULL || frameNum <= 0 )
//	{
//		return;
//	}
//	currentFrameIndex = 0;
//	destFrameIndex = 1;
//	this->schedule( schedule_selector(simulationActionUpdate));
//}
//void CocoActionNode::simulationActionUpdate(float dt)
//{
//	if (currentFrameTime > destFrameIndex * m_fUnitTime)
//	{
//		destFrameIndex++;
//	}
//	currentFrameTime += dt;
//
//}
//void CocoActionNode::stopSimulationAction()
//{
//	this->unschedule( schedule_selector(simulationActionUpdate));
//}
NS_CC_EXT_END