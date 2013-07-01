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

#include "CocoGUIActionNode.h"
#include "CocoGUIActionFrame.h"
#include "../../JsonReader/DictionaryHelper.h"
#include "../System/UIHelper.h"

NS_CC_EXT_BEGIN

CocoGUIActionNode::CocoGUIActionNode()
{
	currentIndex = 0;
	m_actionNode = NULL;

	m_action = NULL;

	m_ActionFrameList = cocos2d::CCArray::create();
	m_ActionFrameList->retain();
}

CocoGUIActionNode::~CocoGUIActionNode()
{
	if (m_action != NULL)
	{
		m_action->release();
	}
	m_ActionFrameList->removeAllObjects();
	m_ActionFrameList->release();
}

void CocoGUIActionNode::initWithDictionary(cs::CSJsonDictionary *dic,UIWidget* root)
{
    this->setName(DICTOOL->getStringValue_json(dic, "name"));
    int actionFrameCount = DICTOOL->getArrayCount_json(dic, "actionframelist");
    this->SetActionNode(CCUIHELPER->seekWidgetByName(root, this->getName()));
    for (int i=0; i<actionFrameCount; i++) {
        CocoGUIActionFrame* actionFrame = new CocoGUIActionFrame();
        cs::CSJsonDictionary* actionFrameDic = DICTOOL->getDictionaryFromArray_json(dic, "actionframelist", i);
        actionFrame->initWithDictionary(actionFrameDic);
        this->m_ActionFrameList->addObject(actionFrame);
    }
}

void CocoGUIActionNode::SetActionNode(UIWidget* widget)
{
	m_actionNode = widget;

	//UpdateToFrameByIndex(currentIndex);
}
void CocoGUIActionNode::InsertFrame(int index, CocoGUIActionFrame* frame)
{
	m_ActionFrameList->insertObject(frame,index);
}

void CocoGUIActionNode::AddFrame(CocoGUIActionFrame* frame)
{
	m_ActionFrameList->addObject(frame);
}

void CocoGUIActionNode::DeleteFrame(CocoGUIActionFrame* frame)
{
	m_ActionFrameList->removeObject(frame);
}

void CocoGUIActionNode::ClearAllFrame()
{
	m_ActionFrameList->removeAllObjects();
}

void CocoGUIActionNode::UpdateToFrameByIndex(int index)
{
	currentIndex = index;

	int frameNum = m_ActionFrameList->count();

	bool bFindFrame = false;

	CocoGUIActionFrame* frame = NULL;
	for (int i = 0; i < frameNum; i++)
	{
		frame = (CocoGUIActionFrame*)m_ActionFrameList->objectAtIndex(index);
		if (frame->getFrameId() == index)
		{
			bFindFrame = true;
			UpdateToFrame(frame);
			break;
		}
	}

	if (!bFindFrame)
	{
		m_actionNode->setVisible(false);
	}
}

void CocoGUIActionNode::UpdateToFrame(CocoGUIActionFrame* frame)
{
	if ( m_actionNode == NULL || frame == NULL )
	{
		return;
	}

	m_actionNode->setRotation(frame->getRotation());
	m_actionNode->setScaleX(frame->getScaleX());
	m_actionNode->setScaleY(frame->getScaleY());
	m_actionNode->setPosition(frame->getPosition());
	m_actionNode->setOpacity(frame->getOpacity());
	m_actionNode->setColor(frame->getColor());

}

void CocoGUIActionNode::RunAction(float fUnitTime, bool bloop)
{
	int frameNum = m_ActionFrameList->count();

	if ( m_actionNode == NULL || frameNum <= 0 )
	{
		return;
	}	

	CCArray* actionFrame = new CCArray();

	for ( int i = 0; i < frameNum; i++ )
	{
		float duration;

		CocoGUIActionFrame* frame = (CocoGUIActionFrame*)m_ActionFrameList->objectAtIndex(i);

		if ( i == 0 )
		{
			//duration = frame->getFrameId() * fUnitTime;
			duration = 0.01f;
		}
		else
		{
			CocoGUIActionFrame* frame_pre = (CocoGUIActionFrame*)m_ActionFrameList->objectAtIndex(i-1);
			duration = (frame->getFrameId() - frame_pre->getFrameId()) * fUnitTime;
		}

		CCMoveTo* action_1 = CCMoveTo::create(duration,frame->getPosition());
		CCRotateTo* action_2 = CCRotateTo::create(duration,frame->getRotation());
		CCScaleTo* action_3 = CCScaleTo::create(duration,frame->getScaleX(),frame->getScaleY());
		CCFadeTo* action_4 = CCFadeTo::create(duration,frame->getOpacity());
		CCTintTo* action_5 = CCTintTo::create(duration,frame->getColor().r,frame->getColor().g,frame->getColor().r);

		CCSpawn * actionSpawn = CCSpawn::create(action_1,action_2,action_3,action_4,action_5, NULL);
		actionFrame->addObject( actionSpawn );
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
                m_actionNode->runAction(m_action);
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
            m_actionNode->runAction(m_action);
        }
	}
}

void CocoGUIActionNode::StopAction()
{
    if (m_actionNode) {
        m_actionNode->stopAction(m_action);
    }
}

//void CocoGUIActionNode::RunToFrameByIndex(int index)
//{
//	int frameNum = m_ActionFrameList->size();
//
//	if ( index < 0 || index >= frameNum )
//	{
//		if (m_actionNode != NULL)
//		{
//			m_actionNode->setVisible(false);
//		}
//
//		return;
//	}
//
//	CocoGUIActionFrame* frame = (CocoGUIActionFrame*)m_ActionFrameList->at(index);
//
//	RunToFrame(frame);
//}
//
//void CocoGUIActionNode::RunToFrame(CocoGUIActionFrame* frame)
//{
//	if ( m_actionNode == NULL || frame == NULL )
//	{
//		return;
//	}
//}

NS_CC_EXT_END