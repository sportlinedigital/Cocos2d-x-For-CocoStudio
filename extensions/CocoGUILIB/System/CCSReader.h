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

#ifndef __CCSREADER_H__
#define __CCSREADER_H__

#include "../BaseClasses/CocoWidget.h"
#include "../../CCArmature/external_tool/Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN

#define kCCSVersion 1.0
class CCSReader : cocos2d::CCObject
{
public:
    CCSReader();
    ~CCSReader();
    static CCSReader* shareReader();
    
    CocoWidget* widgetFromJsonFile(const char* fileName);
    CocoWidget* widgetFromJsonDictionary(cs::CSJsonDictionary* data);
    
    void setPropsForWidgetFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setColorPropsForWidgetFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForButtonFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForCheckBoxFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForImageViewFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForLabelFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForLabelAtlasFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForContainerWidgetFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForPanelFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForScrollViewFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForSliderFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForTextAreaFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForTextButtonFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForTextFieldFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForLoadingBarFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    void setPropsForImageButtonFromJsonDictionary(CocoWidget* widget, cs::CSJsonDictionary* options);
    void setPropsForListViewFromJsonDictionary(CocoWidget* widget, cs::CSJsonDictionary* options);
    void setPropsForPageViewFromJsonDictionary(CocoWidget*widget,cs::CSJsonDictionary* options);
    
protected:
    std::string m_strFilePath;
};

NS_CC_EXT_END


#endif /* defined(__CocosGUITest__CCSReader__) */
