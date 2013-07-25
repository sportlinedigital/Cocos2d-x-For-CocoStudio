//
//  UILayoutDefine.cpp
//  TestCpp
//
//  Created by cai wenzhi on 13-7-24.
//
//

#include "UILayoutDefine.h"

NS_CC_EXT_BEGIN


UIMargin::UIMargin(void) : left(0), top(0), right(0), bottom(0)
{
}

UIMargin::UIMargin(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b)
{
}

UIMargin::UIMargin(const UIMargin& other) : left(other.left), top(other.top), right(other.right), bottom(other.bottom)
{
}

UIMargin& UIMargin::operator= (const UIMargin& other)
{
    setMargin(other.left, other.top, other.right, other.bottom);
    return *this;
}

void UIMargin::setMargin(float l, float t, float r, float b)
{
    left = l;
    top = t;
    right = r;
    bottom = b;
}

bool UIMargin::equals(const cocos2d::extension::UIMargin &target) const
{
    return (left == target.left && top == target.top && right == target.right && bottom == target.bottom);
}

NS_CC_EXT_END