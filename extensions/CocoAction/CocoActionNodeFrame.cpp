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

#include "CocoActionFrame.h"
#include "CocoActionNodeFrame.h"
#include "../../CocostudioReader/DictionaryHelper.h"

NS_CC_EXT_BEGIN

CocoActionNodeFrame::CocoActionNodeFrame()
{

}
CocoActionNodeFrame::~CocoActionNodeFrame()
{

}

void CocoActionNodeFrame::setFrameId(int frameId)
{
	m_frameId = frameId;
}
int CocoActionNodeFrame::getFrameId()
{
	return m_frameId;
}

void CocoActionNodeFrame::initWithDictionary(cs::CSJsonDictionary *dic)
{
    setFrameId(DICTOOL->getIntValue_json(dic, "frameid"));
	int actionFrameCount = DICTOOL->getArrayCount_json(dic, "actionframelist");
	for (int i=0; i<actionFrameCount; i++) {
		
		cs::CSJsonDictionary* actionFrameDic = DICTOOL->getDictionaryFromArray_json(dic, "actionframelist", i);
		FrameType frameType = (FrameType)DICTOOL->getIntValue_json(actionFrameDic, "frametype");
		switch (frameType)
		{
		case kKeyframeMove:
			{
				CocoActionMoveFrame* actionFrame = new CocoActionMoveFrame();
				actionFrame->setPosition(ccp(DICTOOL->getFloatValue_json(dic, "positionx"), DICTOOL->getFloatValue_json(dic, "positiony")));
				m_ActionFrameList->addObject(actionFrame);
			}
			break;
		case kKeyframeScale:
			{
				CocoActionScaleFrame* actionFrame = new CocoActionScaleFrame();
				actionFrame->setScaleX(DICTOOL->getFloatValue_json(dic, "scalex"));
				actionFrame->setScaleY(DICTOOL->getFloatValue_json(dic, "scaley"));
				m_ActionFrameList->addObject(actionFrame);
			}
			break;
		case kKeyframeRotate:
			{
				CocoActionRotationFrame* actionFrame = new CocoActionRotationFrame();
				actionFrame->setRotation(DICTOOL->getFloatValue_json(dic, "rotation"));
			}			
			break;
		case kKeyframeTint:
			{
				CocoActionFadeFrame* actionFrame = new CocoActionFadeFrame();
				actionFrame->setOpacity(DICTOOL->getIntValue_json(dic, "opacity"));
			}
			break;
		case kKeyframeFade:
			{
				CocoActionTintFrame* actionFrame = new CocoActionTintFrame();
				actionFrame->setColor(ccc3(DICTOOL->getIntValue_json(dic, "colorr"), DICTOOL->getIntValue_json(dic, "colorg"), DICTOOL->getIntValue_json(dic, "colorb")));
			}
			break;
		}
		
	}
}

NS_CC_EXT_END