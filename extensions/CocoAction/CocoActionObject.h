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

#ifndef __CocoActionObject_H__
#define __CocoActionObject_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "CocoActionNode.h"
#include "../../CCArmature/external_tool/Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN

class CocoActionObject:public cocos2d::CCObject
{
protected:
	cocos2d::CCArray* m_ActionNodeList;/*actionnode*/

	const char* m_name;
	bool m_loop;
	bool m_bPause;
	float m_CurrentTime;

	CCScheduler *m_pScheduler;
	void scheduleUpdate();
	void unscheduleUpdate();

public:
    CocoActionObject();
    virtual ~CocoActionObject();

	void setName(const char* name);
	const char* getName();

	void setLoop(bool bLoop);
	bool getLoop();

	void play();
	void pause();
	void stop();

	void runSimulationAction();
	void simulationActionUpdate(float dt);
	void stopSimulationAction();

	void updateToFrameByTime(float fTime);

    void initWithDictionary(cs::CSJsonDictionary* dic,CCObject* root);
};

NS_CC_EXT_END

#endif
