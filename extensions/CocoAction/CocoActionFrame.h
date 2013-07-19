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

#ifndef __CocoActionFRAME_H__
#define __CocoActionFRAME_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "../../CCArmature/external_tool/Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN

class CocoActionFrame:public cocos2d::CCObject
{
protected:
	int m_frameType;
	int m_easingType;
	float m_fTime;
public:
    CocoActionFrame();
    virtual ~CocoActionFrame();

	void setFrameTime(float fTime);
	float getFrameTime();

	void setFrameType(int frameType);
	int getFrameType();

	void setEasingType(int easingType);
	int getEasingType();

	virtual void initWithDictionary(cs::CSJsonDictionary* dic);

	virtual CCAction* getAction(float fDuration);
};

class CocoActionMoveFrame:public CocoActionFrame
{
protected:
	CCPoint m_position;
public:
	void setPosition(CCPoint pos);
	CCPoint getPosition();

	virtual CCAction* getAction(float fDuration);
};

class CocoActionScaleFrame:public CocoActionFrame
{
protected:
	float m_scaleX;
	float m_scaleY;
public:
	void setScaleX(float scaleX);
	float getScaleX();

	void setScaleY(float scaleY);
	float getScaleY();

	virtual CCAction* getAction(float fDuration);
};

class CocoActionRotationFrame:public CocoActionFrame
{
protected:
	float m_rotation;
public:
	void setRotation(float rotation);
	float getRotation();

	virtual CCAction* getAction(float fDuration);
};

class CocoActionFadeFrame:public CocoActionFrame
{
protected:
	float m_opacity;
public:
	void setOpacity(float opacity);
	float getOpacity();

	virtual CCAction* getAction(float fDuration);
};

class CocoActionTintFrame:public CocoActionFrame
{
protected:
	ccColor3B m_color;
public:
	void setColor(ccColor3B ccolor);
	ccColor3B getColor();

	virtual CCAction* getAction(float fDuration);
};

NS_CC_EXT_END

#endif
