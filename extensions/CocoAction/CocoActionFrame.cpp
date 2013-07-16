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
#include "../../CocostudioReader/DictionaryHelper.h"

NS_CC_EXT_BEGIN

CocoActionFrame::CocoActionFrame()
{

}
CocoActionFrame::~CocoActionFrame()
{

}
void CocoActionFrame::setFrameTime(float fTime)
{
	m_fTime = fTime;
}
float CocoActionFrame::getFrameTime()
{
	return m_fTime;
}

void CocoActionFrame::setFrameType(int frameType)
{
	m_frameType = frameType;
}
int CocoActionFrame::getFrameType()
{
	return m_frameType;
}

void CocoActionFrame::setEasingType(int easingType)
{
	m_easingType = easingType;
}
int CocoActionFrame::getEasingType()
{
	return m_easingType;
}

void CocoActionFrame::initWithDictionary(cs::CSJsonDictionary *dic)
{
}

CCAction* CocoActionFrame::getAction(float fDuration)
{
	return NULL;
}
//////////////////////////////////////////////////////////////////////////

void CocoActionMoveFrame::setPosition(CCPoint pos)
{
	m_position = pos;
}
CCPoint CocoActionMoveFrame::getPosition()
{
	return m_position;
}
CCAction* CocoActionMoveFrame::getAction(float fDuration)
{
	return CCMoveTo::create(fDuration,m_position);
}
//////////////////////////////////////////////////////////////////////////

void CocoActionScaleFrame::setScaleX(float scaleX)
{
	m_scaleX = scaleX;
}
float CocoActionScaleFrame::getScaleX()
{
	return m_scaleX;
}
void CocoActionScaleFrame::setScaleY(float scaleY)
{
	m_scaleY = scaleY;
}
float CocoActionScaleFrame::getScaleY()
{
	return m_scaleY;
}
CCAction* CocoActionScaleFrame::getAction(float fDuration)
{
	return CCScaleTo::create(fDuration,m_scaleX,m_scaleY);
}
//////////////////////////////////////////////////////////////////////////

void CocoActionRotationFrame::setRotation(float rotation)
{
	m_rotation = rotation;
}
float CocoActionRotationFrame::getRotation()
{
	return m_rotation;
}
CCAction* CocoActionRotationFrame::getAction(float fDuration)
{
	return CCRotateTo::create(fDuration,m_rotation);
}
//////////////////////////////////////////////////////////////////////////

void CocoActionFadeFrame::setOpacity(float opacity)
{
	m_opacity = opacity;
}
float CocoActionFadeFrame::getOpacity()
{
	return m_opacity;
}
CCAction* CocoActionFadeFrame::getAction(float fDuration)
{
	return CCFadeTo::create(fDuration,m_opacity);
}
//////////////////////////////////////////////////////////////////////////

void CocoActionTintFrame::setColor(ccColor3B ccolor)
{
	m_color = ccolor;
}
ccColor3B CocoActionTintFrame::getColor()
{
	return m_color;
}
CCAction* CocoActionTintFrame::getAction(float fDuration)
{
	return CCTintTo::create(fDuration,m_color.r,m_color.g,m_color.b);
}
//////////////////////////////////////////////////////////////////////////
NS_CC_EXT_END