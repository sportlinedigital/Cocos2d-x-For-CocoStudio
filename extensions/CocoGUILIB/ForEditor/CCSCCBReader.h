
/* gui mark */


#ifndef __TestCpp__CCSCCBReader__
#define __TestCpp__CCSCCBReader__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "../BaseClasses/UIWidget.h"
#include "../../CCArmature/external_tool/Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN

#define kCCSCCBVersion 1.0

using namespace cs;

class CCSCCBReader : cocos2d::CCObject
{
    
public:
    CCSCCBReader();
    ~CCSCCBReader();
    static CCSCCBReader* shareReader();
    
    void serializeJson(const char* ccbFilePath, const char* outFilePath);
    std::string jsonFromCCBFile(const char* fileName);
//    CSJsonDictionary* csJsonDictionaryFromCCBFile(const char* fileName);
//    CSJsonDictionary* widgetTreeFromCCB(CSJsonDictionary* widgetTree, cocos2d::CCDictionary* nodeGraph);
    
    /*
    void setOptionsForWidgetCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setColorOptionsForWidgetCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForButtonCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForImageViewCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForImageViewScale9CSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForContainerWidgetCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForPanelCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForGradientPanelCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForScrollViewCSJsonDictionaryFromCCB(CSJsonDictionary* options, CSJsonDictionary* widgetTree, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForTextAreaCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForLabelBMFontCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
    void setOptionsForControlButtonCSJsonDictionaryFromCCB(CSJsonDictionary* options, cocos2d::CCDictionary* nodeGraph);
     */

    CSJsonDictionary* csJsonDictionaryFromCCBFile(const char* fileName);
    CSJsonDictionary* widgetTreeFromCCB(CSJsonDictionary* widgetTree, cocos2d::CCDictionary* nodeGraph, UIWidget* parent);
    
    void setPropsForWidgetFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setColorPropsForWidgetFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForButtonFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForImageViewFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForImageViewScale9FromCCB(UIWidget* widget, UIWidget* parent,  cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForContainerWidgetFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForPanelFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForGradientPanelFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForScrollViewFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options, CSJsonDictionary* widgetTree);
    void setPropsForTextAreaFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForLabelBMFontFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);
    void setPropsForControlButtonFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* nodeGraph, CSJsonDictionary* options);        
    
    
    UIWidget* widgetFromCCBFile(const char* fileName);
    UIWidget* widgetFromCCB(cocos2d::CCDictionary* nodeGraph, UIWidget* parent);
    
    void setPropsForWidgetFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setColorPropsForWidgetFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForButtonFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForImageViewFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForImageViewScale9FromCCB(UIWidget* widget, UIWidget* parent,  cocos2d::CCDictionary* options);
    void setPropsForContainerWidgetFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForPanelFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForGradientPanelFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForScrollViewFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForTextAreaFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForLabelBMFontFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    void setPropsForControlButtonFromCCB(UIWidget* widget, UIWidget* parent, cocos2d::CCDictionary* options);
    
    
    CCSize parsePropTypeSize(float width, float height, int type, UIWidget* parent);
    CCPoint getAbsolutePosition(const CCPoint &pt, int nType, UIWidget* parent);
    
    
protected:
    float resolutionScale;
};

NS_CC_EXT_END

#endif /* defined(__TestCpp__CCSCCBReader__) */

/**/
