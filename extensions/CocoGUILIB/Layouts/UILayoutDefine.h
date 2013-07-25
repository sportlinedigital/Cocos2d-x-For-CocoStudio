//
//  UILayoutDefine.h
//  TestCpp
//
//  Created by cai wenzhi on 13-7-24.
//
//

#ifndef __UILAYOUTDEFINE_H__
#define __UILAYOUTDEFINE_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class UIMargin
{
public:
    float left;
    float top;
    float right;
    float bottom;
    
public:
    UIMargin();
    UIMargin(float l, float t, float r, float b);
    UIMargin(const UIMargin& other);
    UIMargin& operator= (const UIMargin& other);
    void setMargin(float l, float t, float r, float b);
    bool equals(const UIMargin& target) const;
};

NS_CC_EXT_END

#endif /* defined(__UILayoutDefine__) */
