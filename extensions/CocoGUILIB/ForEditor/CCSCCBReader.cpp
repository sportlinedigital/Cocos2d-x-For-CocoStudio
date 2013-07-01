
/* gui mark */

#include "CCSCCBReader.h"
#include "CocosGUI.h"
#include "../../JsonReader/DictionaryHelper.h"
#include <fstream>
#include <iostream>

NS_CC_EXT_BEGIN

using namespace cs;

static CCSCCBReader* sharedReader = NULL;

CCSCCBReader::CCSCCBReader()
: resolutionScale(0.0)
{
    
}

CCSCCBReader::~CCSCCBReader()
{
    
}

CCSCCBReader* CCSCCBReader::shareReader()
{
    if (!sharedReader)
    {
        sharedReader = new CCSCCBReader();
    }
    return sharedReader;
}

void CCSCCBReader::serializeJson(const char *ccbFilePath, const char *outFilePath)
{
    std::string json = jsonFromCCBFile(ccbFilePath);
    
    std::string jsonpath = outFilePath;
//    std::string jsonpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(outFilePath);
    FILE *f2 = fopen(jsonpath.c_str(), "w");
    if (f2)
    {
        fputs(json.c_str(), f2);
        fclose(f2);
    }
}

std::string CCSCCBReader::jsonFromCCBFile(const char *fileName)
{
    CSJsonDictionary* csJsonDictionary = csJsonDictionaryFromCCBFile(fileName);
    
    std::string json = csJsonDictionary->getDescription();
    return json;
}

/*
CSJsonDictionary* CCSCCBReader::csJsonDictionaryFromCCBFile(const char *fileName)
{
    CSJsonDictionary* csJsonDictionaryRoot = new CSJsonDictionary();
    
    CCDictionary* ccbRoot = CCDictionary::createWithContentsOfFile(fileName);
    
    
    // resolutions
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int index = (winSize.width < 1024) ? 0 : 1;
    
    CCArray* resolutions = dynamic_cast<CCArray*>(DICTOOL->getArrayValue(ccbRoot, "resolutions"));
    CCDictionary* item = dynamic_cast<CCDictionary*>(resolutions->objectAtIndex(index));
    int height = DICTOOL->getIntValue(item, "height");
    int width = DICTOOL->getIntValue(item, "width");
    
    resolutionScale = DICTOOL->getFloatValue(item, "scale");
    
    csJsonDictionaryRoot->insertItem("designHeight", height);
    csJsonDictionaryRoot->insertItem("designWidth", width);
    csJsonDictionaryRoot->insertItem("version", 1);
    
    // nodeGraph
    CCDictionary* nodeGraph = DICTOOL->getSubDictionary(ccbRoot, "nodeGraph");
    
    // csJsonDictionary
    CSJsonDictionary* widgetTree = new CSJsonDictionary();
    widgetTree = widgetTreeFromCCB(widgetTree, nodeGraph);
    csJsonDictionaryRoot->insertItem("widgetTree", widgetTree);
    
    return csJsonDictionaryRoot;
}
 */

/*
CSJsonDictionary* CCSCCBReader::widgetTreeFromCCB(CSJsonDictionary* widgetTree, cocos2d::CCDictionary *nodeGraph)
{
    const char* baseClass = DICTOOL->getStringValue(nodeGraph, "baseClass");
    
    CSJsonDictionary* options = new CSJsonDictionary();
    
    if (baseClass && strcmp(baseClass, "CCNode") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLayer") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        setOptionsForPanelCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLayerColor") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        setOptionsForPanelCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLayerGradient") == 0)
    {
        widgetTree->insertItem("classname", "GradientPanel");
        
        setOptionsForGradientPanelCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCScrollView") == 0)
    {
        widgetTree->insertItem("classname", "ScrollView");
        
        setOptionsForScrollViewCSJsonDictionaryFromCCB(options, widgetTree, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCMenu") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        setOptionsForPanelCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCSprite") == 0)
    {
        widgetTree->insertItem("classname", "ImageView");
        
        setOptionsForImageViewCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCScale9Sprite") == 0)
    {
        widgetTree->insertItem("classname", "ImageView");
        
        setOptionsForImageViewScale9CSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLabelTTF") == 0)
    {
        widgetTree->insertItem("classname", "TextArea");
        
        setOptionsForTextAreaCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLabelBMFont") == 0)
    {
        widgetTree->insertItem("classname", "LabelBMFont");
        
        setOptionsForLabelBMFontCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCControlButton") == 0)
    {
        widgetTree->insertItem("classname", "ControlButton");
        
        setOptionsForControlButtonCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCMenuItemImage") == 0)
    {
        widgetTree->insertItem("classname", "Button");
        
        setOptionsForButtonCSJsonDictionaryFromCCB(options, nodeGraph);
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCBFile") == 0)
    {
        CSJsonDictionary* child = new CSJsonDictionary();
        
        CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
        
        CCObject* obj = NULL;
        CCARRAY_FOREACH(properties, obj)
        {
            CCDictionary* item = static_cast<CCDictionary*>(obj);
            
            const char* name = DICTOOL->getStringValue(item, "name");
            if (strcmp(name, "ccbFile") == 0)
            {
                const char* value = DICTOOL->getStringValue(item, "value");
                
                CCDictionary* ccbRoot = CCDictionary::createWithContentsOfFile(value);
                // nodeGraph
                CCDictionary* nodeGraph = DICTOOL->getSubDictionary(ccbRoot, "nodeGraph");
                // child
                child = widgetTreeFromCCB(child, nodeGraph);
                widgetTree->insertItemToArray("children", child);
                
                break;
            }
        }
        
        CCARRAY_FOREACH(properties, obj)
        {
            CCDictionary* item = static_cast<CCDictionary*>(obj);
            
            const char* name = DICTOOL->getStringValue(item, "name");
            if (strcmp(name, "position") == 0)
            {
                CCArray* value = DICTOOL->getArrayValue(item, "value");
                
                float x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
                float y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
                int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
                
                child->insertItem("x", x);
                child->insertItem("y", y);
                
                //                    CCPoint pt = ccp(x, y);
                //                    pt = getAbsolutePosition(pt, type, parent);
//                 CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//                 if (pt.y >= winSize.height)
//                 {
//                 pt.setPoint(pt.x, pt.y - widget->getContainerNode()->getContentSize().height);
//                 }
            }
            else if (strcmp(name, "scale") == 0)
            {
                CCArray* value = DICTOOL->getArrayValue(item, "value");
                float scale_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
                float scale_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
                
                child->insertItem("scaleX", scale_x);
                child->insertItem("scaleY", scale_y);
            }
            else if (strcmp(name, "visible") == 0)
            {
                bool value = DICTOOL->getBooleanValue(item, "value");
                
                child->insertItem("visible", value);
            }
        }
    }
    else if (baseClass && strcmp(baseClass, "CCParticleSystemQuad") == 0)
    {
        int a = 0;
        a = 1;
    }
    
    // children
    CCArray* children = DICTOOL->getArrayValue(nodeGraph, "children");
    
    if (children)
    {
        for (int i = 0; i < children->count(); i++)
        {
            cocos2d::CCDictionary* subNodeGraph = (cocos2d::CCDictionary*)(children->objectAtIndex(i));
            
            CSJsonDictionary* child = new CSJsonDictionary();
            child = widgetTreeFromCCB(child, subNodeGraph);
            if (child)
            {
                widgetTree->insertItemToArray("children", child);
            }
        }
    }
    
    return widgetTree;
}
 */

/*
void CCSCCBReader::setOptionsForWidgetCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    // widget initialize properties
    options->insertItem("__type", "ComGUIWidgetSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "Widget");
    options->insertItem("classname", "Widget");
    options->insertItem("ZOrder", 0);
    options->insertItem("colorB", 255);
    options->insertItem("colorG", 255);
    options->insertItem("colorR", 255);
    options->insertItem("opacity", 255);
    options->insertItem("flipX", false);
    options->insertItem("flipY", false);
    options->insertItem("useMergedTexture", false);
    options->insertItem("visible", true);
    options->insertItem("xProportion", 0);
    options->insertItem("yProportion", 0);
    options->insertItem("anchorPointX", 0.5);
    options->insertItem("anchorPointY", 0.5);
    
    // memberVarAssignmentName (widget name)
    const char* memberVarAssignmentName = DICTOOL->getStringValue(nodeGraph, "memberVarAssignmentName");
    const char* widgetName = (strcmp(memberVarAssignmentName, "") == 0) ? "default" : memberVarAssignmentName;
    
    options->insertItem("name", widgetName);
    
    // properties
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "position") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            float x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("x", x);
            options->insertItem("y", y);
            //
//             CCPoint pt = ccp(x, y);
//             pt = getAbsolutePosition(pt, type, parent);
             //
            
            //
//             std::string parentName = parent->getBassClass();
//             if (parentName.compare("CCSprite") == 0)
//             {
//             pt = ccp(pt.x + (-parent->getRect().size.width / 2), pt.y + (-parent->getRect().size.height / 2));
//             }
             //
        }
        else if (strcmp(name, "contentSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("width", width);
            options->insertItem("height", height);
            
            //                 CCSize size = parsePropTypeSize(width, height, type, parent);
        }
        else if (strcmp(name, "anchorPoint") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float anchorpoint_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float anchorpoint_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            options->insertItem("anchorPointX", anchorpoint_x);
            options->insertItem("anchorPointY", anchorpoint_y);
        }
        else if (strcmp(name, "scale") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* str = dynamic_cast<CCString*>(value);
            if (str)
            {
                float scale = str->floatValue();
                
                options->insertItem("scaleX", scale);
                options->insertItem("scaleY", scale);
                
                continue;
            }
            
            float scale_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float scale_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            options->insertItem("scaleX", scale_x);
            options->insertItem("scaleY", scale_y);
        }
        else if (strcmp(name, "rotation") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            options->insertItem("rotation", value);
        }
        else if (strcmp(name, "skew") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float skew_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float skew_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            options->insertItem("skewX", skew_x);
            options->insertItem("skewY", skew_y);
        }
        else if (strcmp(name, "tag") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("widgetTag", value);
        }
        else if (strcmp(name, "ignoreAnchorPointForPosition") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("ignoreAnchorPointForPosition", value);
        }
        else if (strcmp(name, "touchEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "isTouchEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "accelerometerEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isAccelerometerEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "mouseEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isMouseEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "keyboardEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isKeyboardEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "visible") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("visible", value);
        }
        else if (strcmp(name, "seqExpanded") == 0)
        {
            
        }
    }
}

void CCSCCBReader::setColorOptionsForWidgetCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    
}

void CCSCCBReader::setOptionsForButtonCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIButtonSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "Button");
    options->insertItem("classname", "Button");
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "block") == 0)
        {
            //                CCArray* value = DICTOOL->getArrayValue(item, "value");
        }
        else if (strcmp(name, "isEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "isEnabled");
            
            options->insertItem("touchAble", value);
            //                button->setBeTouchAble(true);
            //                button->setBeTouchAble(value);
        }
        else if (strcmp(name, "normalSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            
            options->insertItem("normal", flieName->getCString());
        }
        else if (strcmp(name, "selectedSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            
            options->insertItem("pressed", flieName->getCString());
        }
        else if (strcmp(name, "disabledSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            
            options->insertItem("disabled", flieName->getCString());
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setOptionsForImageViewCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIImageViewSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ImageView");
    options->insertItem("classname", "ImageView");
    
    options->insertItem("scale9Enable", false);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "displayFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* spriteSheetFileName = static_cast<CCString*>(value->objectAtIndex(0));
            std::string spriteSheet = spriteSheetFileName->getCString();
            
            CCString* spriteFlieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = spriteFlieName->getCString();
            
            options->insertItem("fileName", spriteFlieName->getCString());
            
            //
//             CCSpriteFrame *spriteFrame = NULL;
//             
//             if (spriteFile.length() != 0)
//             {
//             if (spriteSheet.length() == 0)
//             {
//             CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
//             CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
//             spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
//             }
//             else
//             {
//             CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
//             frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
//             
//             spriteFrame = frameCache->spriteFrameByName(spriteFile.c_str());
//             }
//             }
//             
//             imageView->setDisplayFrame(spriteFrame);
             //
        }
        else if (strcmp(name, "flip") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            bool flipX = dynamic_cast<CCString*>(value->objectAtIndex(0))->boolValue();
            bool flipY = dynamic_cast<CCString*>(value->objectAtIndex(1))->boolValue();
            
            options->insertItem("flipX", flipX);
            options->insertItem("flipX", flipY);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setOptionsForImageViewScale9CSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIImageViewSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ImageView");
    options->insertItem("classname", "ImageView");
    
    options->insertItem("scale9Enable", true);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "spriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* spriteSheetFileName = static_cast<CCString*>(value->objectAtIndex(0));
            std::string spriteSheet = spriteSheetFileName->getCString();
            
            CCString* spriteFlieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = spriteFlieName->getCString();
            
            options->insertItem("fileName", spriteFlieName->getCString());
            
            //
//             CCSpriteFrame *spriteFrame = NULL;
//             
//             if (spriteFile.length() != 0)
//             {
//             if (spriteSheet.length() == 0)
//             {
//             CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
//             CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
//             spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
//             }
//             else
//             {
//             CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
//             frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
//             
//             spriteFrame = frameCache->spriteFrameByName(spriteFile.c_str());
//             }
//             }
//             
//             //                 imageView->setSpriteFrame(spriteFrame);
             //
        }
        else if (strcmp(name, "preferedSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("width", width);
            options->insertItem("height", height);
            
            //                CCSize size = parsePropTypeSize(width, height, type, parent);
            
        }
        else if (strcmp(name, "insetLeft") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            options->insertItem("insetLeft", value);
        }
        else if (strcmp(name, "insetTop") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            options->insertItem("insetTop", value);
        }
        else if (strcmp(name, "insetRight") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            options->insertItem("insetRight", value);
        }
        else if (strcmp(name, "insetBottom") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            options->insertItem("insetBottom", value);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setOptionsForContainerWidgetCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
}

void CCSCCBReader::setOptionsForPanelCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForContainerWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIPanelSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "Panel");
    options->insertItem("classname", "Panel");
    
    options->insertItem("clipAble", false);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "contentSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("width", width);
            options->insertItem("height", height);
            
            //                CCSize size = parsePropTypeSize(width, height, type, parent);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
    
    options->insertItem("backGroundColorEnable", true);
}

void CCSCCBReader::setOptionsForGradientPanelCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForPanelCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIGradientPanelSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "GradientPanel");
    options->insertItem("classname", "GradientPanel");
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "startColor") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("startColorB", blue);
            options->insertItem("startColorG", green);
            options->insertItem("startColorR", red);
        }
        else if (strcmp(name, "startOpacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("startOpacity", value);
        }
        else if (strcmp(name, "endColor") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("endColorB", blue);
            options->insertItem("endColorG", green);
            options->insertItem("endColorR", red);
            
        }
        else if (strcmp(name, "endOpacity") == 0)
        {
            GLubyte value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("endOpacity", value);
        }
        else if (strcmp(name, "vector") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float vector_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float vector_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            options->insertItem("vectorX", vector_x);
            options->insertItem("vectorY", vector_y);
        }
    }
}

void CCSCCBReader::setOptionsForScrollViewCSJsonDictionaryFromCCB(CSJsonDictionary* options, CSJsonDictionary* widgetTree, CCDictionary* nodeGraph)
{
    setOptionsForPanelCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIScrollViewSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ScrollView");
    options->insertItem("classname", "ScrollView");
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "container") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            CCDictionary* ccbRoot = CCDictionary::createWithContentsOfFile(value);
            // nodeGraph
            CCDictionary* nodeGraph = DICTOOL->getSubDictionary(ccbRoot, "nodeGraph");
            // child
            CSJsonDictionary* child = new CSJsonDictionary();
            child = widgetTreeFromCCB(child, nodeGraph, NULL);
            widgetTree->insertItemToArray("children", child);
        }
        else if (strcmp(name, "direction") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            int direction = -1;
            
            switch (value)
            {
                case 0: // horizontal
                    direction = 0;
                    break;
                    
                case 1: // vertical
                    direction = 1;
                    break;
                    
                default:
                    break;
            }
            
            options->insertItem("childrenDirection", direction);
        }
        else if (strcmp(name, "clipsToBounds") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("clipAble", value);
        }
        else if (strcmp(name, "bounces") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            if (value)
            {
                options->insertItem("bounces", value);
                options->insertItem("updateEnable", true);
                options->insertItem("moveMode", 2);
            }
        }
    }
}

void CCSCCBReader::setOptionsForTextAreaCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUITextAreaSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "TextArea");
    options->insertItem("classname", "TextArea");
    
    options->insertItem("touchSacleEnable", false);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "fontName") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            options->insertItem("fontName", value);
        }
        else if (strcmp(name, "fontSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float fontSize = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            //                float unknown = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            options->insertItem("fontSize", fontSize);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "dimensions") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("width", width);
            options->insertItem("height", height);
            
            //                CCSize size = parsePropTypeSize(width, height, type, parent);
        }
        else if (strcmp(name, "horizontalAlignment") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("horizontalAlignment", value);
        }
        else if (strcmp(name, "verticalAlignment") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("verticalAlignment", value);
        }
        else if (strcmp(name, "string") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            options->insertItem("text", value);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setOptionsForLabelBMFontCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUILabelBMFontSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "LabelBMFont");
    options->insertItem("classname", "LabelBMFont");
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "fntFile") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            options->insertItem("fntFile", value);
        }
        else if (strcmp(name, "string") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            options->insertItem("text", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setOptionsForControlButtonCSJsonDictionaryFromCCB(CSJsonDictionary* options, CCDictionary* nodeGraph)
{
    setOptionsForWidgetCSJsonDictionaryFromCCB(options, nodeGraph);
    
    options->insertItem("__type", "ComGUIControlButtonSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ControlButton");
    options->insertItem("classname", "ControlButton");
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "ccControl") == 0)
        {
            //                bool value = DICTOOL->getBooleanValue(item, "value");
        }
        else if (strcmp(name, "enabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "title|1") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            options->insertItem("normalTitle", value);
            options->insertItem("pressedTitle", value);
            options->insertItem("disabledTitle", value);
        }
        else if (strcmp(name, "titleTTF|1") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            options->insertItem("normalTitleTTFFontName", value);
            options->insertItem("pressedTitleTTFFontName", value);
            options->insertItem("disabledTitleTTFFontName", value);
        }
        else if (strcmp(name, "titleTTFSize|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float fontSize = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            if(type == 1) // kCCBScaleTypeMultiplyResolution
            {
                //                    fontSize *= resolutionScale;
            }
            
            options->insertItem("normalTitleTTFFontSize", fontSize);
            options->insertItem("pressedTitleTTFFontSize", fontSize);
            options->insertItem("pressedTitleTTFFontSize", fontSize);
        }
        else if (strcmp(name, "labelAnchorPoint") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float anchor_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float anchor_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            options->insertItem("labelAnchorPointX", anchor_x);
            options->insertItem("labelAnchorPointY", anchor_y);
        }
        else if (strcmp(name, "preferedSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("width", width);
            options->insertItem("height", height);
            
            //                CCSize size = parsePropTypeSize(width, height, type, parent);
        }
        else if (strcmp(name, "zoomOnTouchDown") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            options->insertItem("zoomOnTouchDown", value);
        }
        else if (strcmp(name, "backgroundSpriteFrame|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            
            options->insertItem("normal", flieName);
        }
        else if (strcmp(name, "titleColor|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("normalTitleColorB", blue);
            options->insertItem("normalTitleColorG", green);
            options->insertItem("normalTitleColorR", red);
        }
        else if (strcmp(name, "backgroundSpriteFrame|2") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            
            options->insertItem("pressed", flieName);
        }
        else if (strcmp(name, "titleColor|2") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("pressedTitleColorB", blue);
            options->insertItem("pressedTitleColorG", green);
            options->insertItem("pressedTitleColorR", red);
        }
        else if (strcmp(name, "backgroundSpriteFrame|3") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            
            options->insertItem("disabled", flieName);
        }
        else if (strcmp(name, "titleColor|3") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("disabledTitleColorB", blue);
            options->insertItem("disabledTitleColorG", green);
            options->insertItem("disabledTitleColorR", red);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}
*/

CSJsonDictionary* CCSCCBReader::csJsonDictionaryFromCCBFile(const char *fileName)
{
    CSJsonDictionary* csJsonDictionaryRoot = new CSJsonDictionary();
    
    CCDictionary* ccbRoot = CCDictionary::createWithContentsOfFile(fileName);
    
    
    // resolutions
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int index = (winSize.width < 1024) ? 0 : 1;
    
    CCArray* resolutions = dynamic_cast<CCArray*>(DICTOOL->getArrayValue(ccbRoot, "resolutions"));
    CCDictionary* item = dynamic_cast<CCDictionary*>(resolutions->objectAtIndex(index));
    int height = DICTOOL->getIntValue(item, "height");
    int width = DICTOOL->getIntValue(item, "width");
    
    resolutionScale = DICTOOL->getFloatValue(item, "scale");
    
    csJsonDictionaryRoot->insertItem("designHeight", height);
    csJsonDictionaryRoot->insertItem("designWidth", width);
    csJsonDictionaryRoot->insertItem("version", 1);
    
    // nodeGraph
    CCDictionary* nodeGraph = DICTOOL->getSubDictionary(ccbRoot, "nodeGraph");
    
    // csJsonDictionary
    CSJsonDictionary* widgetTree = new CSJsonDictionary();
    widgetTree = widgetTreeFromCCB(widgetTree, nodeGraph, NULL);
    csJsonDictionaryRoot->insertItem("widgetTree", widgetTree);    
    
    return csJsonDictionaryRoot;
}

CSJsonDictionary* CCSCCBReader::widgetTreeFromCCB(CSJsonDictionary* widgetTree, CCDictionary *nodeGraph, UIWidget* parent)
{
    using namespace cs;
    
    UIWidget* widget = NULL;
    
    CSJsonDictionary* options = new CSJsonDictionary();
    
    const char* baseClass = DICTOOL->getStringValue(nodeGraph, "baseClass");        
    
    if (baseClass && strcmp(baseClass, "CCNode") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLayer") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLayerColor") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLayerGradient") == 0)
    {
        return NULL;
        
//        widgetTree->insertItem("classname", "Panel");
        
//        widget = CocoGradientPanel::create();
//        setPropsForGradientPanelFromCCB(widget, parent, nodeGraph, options);
//        
//        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCScrollView") == 0)
    {
        return NULL;
        
//        widgetTree->insertItem("classname", "ScrollView");
//        
//        widget = CocoScrollView::create();
//        setPropsForScrollViewFromCCB(widget, parent, nodeGraph, options, widgetTree);
//        
//        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCMenu") == 0)
    {
        widgetTree->insertItem("classname", "Panel");
        
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCSprite") == 0)
    {
        widgetTree->insertItem("classname", "ImageView");
        
        widget = UIImageView::create();
        setPropsForImageViewFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCScale9Sprite") == 0)
    {
        return NULL;
//        widgetTree->insertItem("classname", "ImageView");
//        
//        widget = CocoImageView::create();
//        setPropsForImageViewScale9FromCCB(widget, parent, nodeGraph, options);
//        
//        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLabelTTF") == 0)
    {
        widgetTree->insertItem("classname", "TextArea");
        
        widget = UITextArea::create();
        setPropsForTextAreaFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCLabelBMFont") == 0)
    {
        return NULL;
//        widgetTree->insertItem("classname", "LabelBMFont");
//        
//        widget = CocoLabelBMFont::create();
//        setPropsForLabelBMFontFromCCB(widget, parent, nodeGraph, options);
//        
//        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCControlButton") == 0)
    {
        return NULL;
//        widgetTree->insertItem("classname", "ControlButton");
//        
//        widget = CocoControlButton::create();
//        setPropsForControlButtonFromCCB(widget, parent, nodeGraph, options);
    }
    else if (baseClass && strcmp(baseClass, "CCMenuItemImage") == 0)
    {
        widgetTree->insertItem("classname", "Button");
        
        widget = UIButton::create();
        setPropsForButtonFromCCB(widget, parent, nodeGraph, options);
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCBFile") == 0)
    {
        CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");                
        
        CCObject* obj = NULL;
        CCARRAY_FOREACH(properties, obj)
        {
            CCDictionary* item = static_cast<CCDictionary*>(obj);
            
            const char* name = DICTOOL->getStringValue(item, "name");
            if (strcmp(name, "ccbFile") == 0)
            {
                const char* value = DICTOOL->getStringValue(item, "value");
                
                CCDictionary* root = CCDictionary::createWithContentsOfFile(value);
                // nodeGraph
                CCDictionary* nodeGraph = dynamic_cast<CCDictionary*>(DICTOOL->getArrayValue(root, "nodeGraph"));
                // widget
                widget = widgetFromCCB(nodeGraph, parent);
                
                // childWidgetTree
                widgetTree = widgetTreeFromCCB(widgetTree, nodeGraph, parent);
                
                break;
            }
        }                
        
        CSJsonDictionary* options = widgetTree->getSubDictionary("options");
        
        CCARRAY_FOREACH(properties, obj)
        {
            CCDictionary* item = static_cast<CCDictionary*>(obj);
            
            const char* name = DICTOOL->getStringValue(item, "name");
            if (strcmp(name, "position") == 0)
            {
                CCArray* value = DICTOOL->getArrayValue(item, "value");
                
                float x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
                float y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
                int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
                CCPoint pt = ccp(x, y);
                pt = getAbsolutePosition(pt, type, parent);
                
                //
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                if (pt.y >= winSize.height)
                {
                    CSJsonDictionary* options = widgetTree->getSubDictionary("options");
                    float height = DICTOOL->getFloatValue_json(options, "height");                                        
                    
//                    pt.setPoint(pt.x, pt.y - height);
                    pt.setPoint(pt.x, pt.y - widget->getContainerNode()->getContentSize().height);
                }
                //
                
                widget->setPosition(pt);
                                
                options->insertItem("x", pt.x);
                options->insertItem("y", pt.y);                                
            }
            else if (strcmp(name, "scale") == 0)
            {
                CCArray* value = DICTOOL->getArrayValue(item, "value");
                float scale_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
                float scale_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
                
                widget->setScaleX(scale_x);
                widget->setScaleY(scale_y);
                
                options->insertItem("scaleX", scale_x);
                options->insertItem("scaleY", scale_y);
            }
            else if (strcmp(name, "visible") == 0)
            {
                bool value = DICTOOL->getBooleanValue(item, "value");                
                
                widget->setVisible(value);
                
                options->insertItem("visible", value);                                
            }
        }
        
        widgetTree->insertItem("options", options);
    }
    else if (baseClass && strcmp(baseClass, "CCParticleSystemQuad") == 0)
    {
        return NULL;        
    }
    
    CCArray* children = DICTOOL->getArrayValue(nodeGraph, "children");
    if (children)
    {
        for (int i = 0; i < children->count(); i++)
        {
            cocos2d::CCDictionary* subNodeGraph = (cocos2d::CCDictionary*)(children->objectAtIndex(i));
            
            UIWidget* child = widgetFromCCB(subNodeGraph, widget);
            if (child)
            {
                widget->addChild(child);
            }
            
            CSJsonDictionary* childWidgetTree = new CSJsonDictionary();
            childWidgetTree = widgetTreeFromCCB(childWidgetTree, subNodeGraph, widget);
            if (childWidgetTree)
            {
                widgetTree->insertItemToArray("children", childWidgetTree);
            }
        }
    }
    
    if (widget)
    {
        widget->removeFromParentAndCleanup(true);
    }

    return widgetTree;
//    return widget;
}

void CCSCCBReader::setPropsForWidgetFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    options->insertItem("__type", "ComGUIWidgetSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "Widget");
    options->insertItem("classname", "Widget");
//    options->insertItem("colorB", 255);
//    options->insertItem("colorG", 255);
//    options->insertItem("colorR", 255);
//    options->insertItem("opacity", 255);
    options->insertItem("flipX", false);
    options->insertItem("flipY", false);
    options->insertItem("useMergedTexture", false);
    options->insertItem("visible", true);
    options->insertItem("xProportion", 0);
    options->insertItem("yProportion", 0);
    options->insertItem("anchorPointX", 0.5);
    options->insertItem("anchorPointY", 0.5);
    
    // memberVarAssignmentName (widget name)
    const char* memberVarAssignmentName = DICTOOL->getStringValue(nodeGraph, "memberVarAssignmentName");
    const char* widgetName = (strcmp(memberVarAssignmentName, "") == 0) ? "default" : memberVarAssignmentName;
    
    widget->setName(widgetName);
    
    options->insertItem("name", widgetName);
    
    // widget initialize properties
    widget->setVisible(true);
    widget->setAnchorPoint(ccp(0.5, 0.5));
    
    // properties
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "position") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            float x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCPoint pt = ccp(x, y);
            pt = getAbsolutePosition(pt, type, parent);
            
            WidgetName widgetName = parent->getWidgetName();
            if (widgetName == WIDGET_IMAGEVIEW)
            {
                pt = ccp(pt.x + (-parent->getRect().size.width / 2), pt.y + (-parent->getRect().size.height / 2));
            }
            
            widget->setPosition(pt);
            
            options->insertItem("x", pt.x);
            options->insertItem("y", pt.y);
        }
        else if (strcmp(name, "contentSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            options->insertItem("width", size.width);
            options->insertItem("height", size.height);
        }
        else if (strcmp(name, "anchorPoint") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float anchorpoint_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float anchorpoint_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            widget->setAnchorPoint(ccp(anchorpoint_x, anchorpoint_y));
            
            options->insertItem("anchorPointX", anchorpoint_x);
            options->insertItem("anchorPointY", anchorpoint_y);
        }
        else if (strcmp(name, "scale") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* str = dynamic_cast<CCString*>(value);
            if (str)
            {
                float scale = str->floatValue();
                
                widget->setScale(scale);
                
                options->insertItem("scaleX", scale);
                options->insertItem("scaleY", scale);
                
                continue;
            }
            
            float scale_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float scale_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            widget->setScaleX(scale_x);
            widget->setScaleY(scale_y);
            
            options->insertItem("scaleX", scale_x);
            options->insertItem("scaleY", scale_y);
        }
        else if (strcmp(name, "rotation") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            widget->setRotation(value);
            
            options->insertItem("rotation", value);
        }
        else if (strcmp(name, "skew") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float skew_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float skew_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            widget->setSkewX(skew_x);
            widget->setSkewY(skew_y);
            
            options->insertItem("skewX", skew_x);
            options->insertItem("skewY", skew_y);
        }
        else if (strcmp(name, "tag") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setWidgetTag(value);
            
            options->insertItem("widgetTag", value);
        }
        else if (strcmp(name, "ignoreAnchorPointForPosition") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->ignoreAnchorPointForPosition(value);
            
            options->insertItem("ignoreAnchorPointForPosition", value);
        }
        else if (strcmp(name, "touchEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->setBeTouchEnable(value);
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "isTouchEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->setBeTouchEnable(value);
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "accelerometerEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isAccelerometerEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "mouseEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isMouseEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "keyboardEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isKeyboardEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "visible") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->setVisible(value);
            
            options->insertItem("visible", value);
        }        
    }
}

void CCSCCBReader::setColorPropsForWidgetFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    
}

void CCSCCBReader::setPropsForButtonFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIButtonSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "Button");
    options->insertItem("classname", "Button");
    
    UIButton* button = static_cast<UIButton*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "block") == 0)
        {
//            CCArray* value = DICTOOL->getArrayValue(item, "value");
        }
        else if (strcmp(name, "isEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "isEnabled");
            
            button->setBeTouchEnable(true);
//            button->setBeTouchAble(value);
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "normalSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            button->setNormalSpriteFrame(spriteFrame);
            
            options->insertItem("normal", flieName->getCString());
        }
        else if (strcmp(name, "selectedSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            button->setPressedSpriteFrame(spriteFrame);
            
            options->insertItem("pressed", flieName->getCString());
        }
        else if (strcmp(name, "disabledSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            button->setDisabledSpriteFrame(spriteFrame);
            
            options->insertItem("disabled", flieName->getCString());
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setPropsForImageViewFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIImageViewSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ImageView");
    options->insertItem("classname", "ImageView");
    
    options->insertItem("scale9Enable", false);
    
    UIImageView* imageView = static_cast<UIImageView*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "displayFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* spriteSheetFileName = static_cast<CCString*>(value->objectAtIndex(0));
            std::string spriteSheet = spriteSheetFileName->getCString();
            
            CCString* spriteFlieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = spriteFlieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                if (spriteSheet.length() == 0)
                {
                    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                    CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                    spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
                }
                else
                {
                    CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
                    frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                    
                    spriteFrame = frameCache->spriteFrameByName(spriteFile.c_str());
                }
            }
            
            imageView->setDisplayFrame(spriteFrame);
            
            options->insertItem("fileName", spriteFlieName->getCString());
        }
        else if (strcmp(name, "flip") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            bool flipX = dynamic_cast<CCString*>(value->objectAtIndex(0))->boolValue();
            bool flipY = dynamic_cast<CCString*>(value->objectAtIndex(1))->boolValue();
            
            imageView->setFlipX(flipX);
            imageView->setFlipY(flipY);
            
            options->insertItem("flipX", flipX);
            options->insertItem("flipX", flipY);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            imageView->setOpacity(value);
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            imageView->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            imageView->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setPropsForImageViewScale9FromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIImageViewSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ImageView");
    options->insertItem("classname", "ImageView");
    
    UIImageView* imageView = static_cast<UIImageView*>(widget);
    
    imageView->setScale9Enable(false);
    
    options->insertItem("scale9Enable", false);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "spriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* spriteSheetFileName = static_cast<CCString*>(value->objectAtIndex(0));
            std::string spriteSheet = spriteSheetFileName->getCString();
            
            CCString* spriteFlieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = spriteFlieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                if (spriteSheet.length() == 0)
                {
                    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                    CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                    spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
                }
                else
                {
                    CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
                    frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                    
                    spriteFrame = frameCache->spriteFrameByName(spriteFile.c_str());
                }
            }
            
            imageView->setSpriteFrame(spriteFrame);
            
            options->insertItem("fileName", spriteFlieName->getCString());
        }
        else if (strcmp(name, "preferedSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            imageView->setPreferredSize(size);
            
            options->insertItem("width", width);
            options->insertItem("height", height);
        }
        else if (strcmp(name, "insetLeft") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetLeft(value);
            
            options->insertItem("insetLeft", value);
        }
        else if (strcmp(name, "insetTop") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetTop(value);
            
            options->insertItem("insetTop", value);
        }
        else if (strcmp(name, "insetRight") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetRight(value);
            
            options->insertItem("insetRight", value);
        }
        else if (strcmp(name, "insetBottom") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetBottom(value);
            
            options->insertItem("insetBottom", value);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            imageView->setOpacity(value);
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            imageView->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            imageView->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setPropsForContainerWidgetFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    UIContainerWidget* containerWidget = (UIContainerWidget*)widget;
    
    // ContainerWidget unique properties
    containerWidget->setClipAble(false);
}

void CCSCCBReader::setPropsForPanelFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForContainerWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIPanelSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "Panel");
    options->insertItem("classname", "Panel");
    
    options->insertItem("clipAble", false);
    
    UIPanel* panel = static_cast<UIPanel*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "contentSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            panel->setSize(size);
            
            options->insertItem("width", size.width);
            options->insertItem("height", size.height);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
            
            options->insertItem("colorO", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
    
    
}

void CCSCCBReader::setPropsForGradientPanelFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForPanelFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIGradientPanelSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "GradientPanel");
    options->insertItem("classname", "GradientPanel");
    
    UIGradientPanel* gradientPanel = static_cast<UIGradientPanel*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "startColor") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            gradientPanel->setStartColor(color);
            
            options->insertItem("startColorB", blue);
            options->insertItem("startColorG", green);
            options->insertItem("startColorR", red);
        }
        else if (strcmp(name, "startOpacity") == 0)
        {
            GLubyte value = DICTOOL->getIntValue(item, "value");
            
            gradientPanel->setStartOpacity(value);
            
            options->insertItem("startOpacity", value);
        }
        else if (strcmp(name, "endColor") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            gradientPanel->setEndColor(color);
            
            options->insertItem("endColorB", blue);
            options->insertItem("endColorG", green);
            options->insertItem("endColorR", red);
        }
        else if (strcmp(name, "endOpacity") == 0)
        {
            GLubyte value = DICTOOL->getIntValue(item, "value");
            
            gradientPanel->setEndOpacity(value);
            
            options->insertItem("endOpacity", value);
        }
        else if (strcmp(name, "vector") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float vector_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float vector_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            gradientPanel->setVector(ccp(vector_x, vector_y));
            
            options->insertItem("vectorX", vector_x);
            options->insertItem("vectorY", vector_y);
        }
    }
}

void CCSCCBReader::setPropsForScrollViewFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options, CSJsonDictionary* widgetTree)
{
    setPropsForPanelFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIScrollViewSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ScrollView");
    options->insertItem("classname", "ScrollView");
    
    UIScrollView* scrollView = static_cast<UIScrollView*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "container") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            CCDictionary* root = CCDictionary::createWithContentsOfFile(value);
            // nodeGraph
            CCDictionary* nodeGraph = dynamic_cast<CCDictionary*>(DICTOOL->getArrayValue(root, "nodeGraph"));
            
            // container of scrollview
            UIWidget* container = widgetFromCCB(nodeGraph, widget);
            widget->addChild(container);
            
            // child
            widgetTree = widgetTreeFromCCB(widgetTree, nodeGraph, parent);            
        }
        else if (strcmp(name, "direction") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            SCROLLVIEW_DIR eDirection = SCROLLVIEW_DIR_NONE;
            
            switch (value)
            {
                case 0: // horizontal
                    eDirection = SCROLLVIEW_DIR_HORIZONTAL;
                    break;
                    
                case 1: // vertical
                    eDirection = SCROLLVIEW_DIR_VERTICAL;
                    break;
                    
                default:
                    break;
            }
            
            scrollView->setDirection(eDirection);
            
            int direction = -1;
            
            switch (value)
            {
                case 0: // horizontal
                    direction = 0;
                    break;
                    
                case 1: // vertical
                    direction = 1;
                    break;
                    
                default:
                    break;
            }
            
            options->insertItem("childrenDirection", direction);
        }
        else if (strcmp(name, "clipsToBounds") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            scrollView->setClipAble(value);
            
            options->insertItem("clipAble", value);
        }
        else if (strcmp(name, "bounces") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            if (value)
            {
                scrollView->setUpdateEnable(true);
                scrollView->setMoveMode(SCROLLVIEW_MOVE_MODE_ACTION);
                
                options->insertItem("bounces", value);
                options->insertItem("updateEnable", true);
                options->insertItem("moveMode", 2);
            }
        }
    }
}

void CCSCCBReader::setPropsForTextAreaFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUITextAreaSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "TextArea");
    options->insertItem("classname", "TextArea");
    
    options->insertItem("touchSacleEnable", false);
    
    UITextArea* textArea = static_cast<UITextArea*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "fontName") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            textArea->setFontName(value);
            
            options->insertItem("fontName", value);
        }
        else if (strcmp(name, "fontSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float fontSize = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
//            float unknown = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            textArea->setFontSize(fontSize);
            
            options->insertItem("fontSize", fontSize);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            textArea->setTextColor(color.r, color.g, color.b);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "dimensions") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = dynamic_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            textArea->setTextAreaSize(size);
            
            CCLog("type = %d", type);
            
            options->insertItem("areaWidth", size.width);
            options->insertItem("areaHeight", size.height);
        }
        else if (strcmp(name, "horizontalAlignment") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            textArea->setTextHorizontalAlignment((CCTextAlignment)value);
            
            options->insertItem("horizontalAlignment", value);
        }
        else if (strcmp(name, "verticalAlignment") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            textArea->setTextVerticalAlignment((CCVerticalTextAlignment)value);
            
            options->insertItem("verticalAlignment", value);
        }
        else if (strcmp(name, "string") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            textArea->setText(value);
            
            options->insertItem("text", value);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setPropsForLabelBMFontFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUILabelBMFontSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "LabelBMFont");
    options->insertItem("classname", "LabelBMFont");
    
    UILabelBMFont* labelBMFont = static_cast<UILabelBMFont*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "fntFile") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            labelBMFont->setFntFile(value);
            
            options->insertItem("fntFile", value);
        }
        else if (strcmp(name, "string") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            labelBMFont->setText(value);
            
            options->insertItem("text", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            labelBMFont->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
            
            options->insertItem("opacity", value);
        }        
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

void CCSCCBReader::setPropsForControlButtonFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *nodeGraph, CSJsonDictionary* options)
{
    setPropsForWidgetFromCCB(widget, parent, nodeGraph, options);
    
    options->insertItem("__type", "ComGUIControlButtonSurrogate:#EditorCommon.JsonModel.Component.GUI");
    options->insertItem("classType", "ControlButton");
    options->insertItem("classname", "ControlButton");
    
    UIControlButton* controlButton = static_cast<UIControlButton*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "ccControl") == 0)
        {
//            bool value = DICTOOL->getBooleanValue(item, "value");
        }
        else if (strcmp(name, "enabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            controlButton->setBeTouchEnable(value);
            
            options->insertItem("touchAble", value);
        }
        else if (strcmp(name, "title|1") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            controlButton->setNormalTitle(value);
            controlButton->setPressedTitle(value);
            controlButton->setDisabledTitle(value);
            
            options->insertItem("normalTitle", value);
            options->insertItem("pressedTitle", value);
            options->insertItem("disabledTitle", value);
        }
        else if (strcmp(name, "titleTTF|1") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            controlButton->setNormalFontName(value);
            controlButton->setPressedFontName(value);
            controlButton->setDisabledFontName(value);
            
            options->insertItem("normalTitleTTFFontName", value);
            options->insertItem("pressedTitleTTFFontName", value);
            options->insertItem("disabledTitleTTFFontName", value);
        }
        else if (strcmp(name, "titleTTFSize|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float fontSize = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            if(type == 1) // kCCBScaleTypeMultiplyResolution
            {
                fontSize *= resolutionScale;
            }
            
            controlButton->setNormalFontSize(fontSize);
            controlButton->setPressedFontSize(fontSize);
            controlButton->setDisabledFontSize(fontSize);
            
            options->insertItem("normalTitleTTFFontSize", fontSize);
            options->insertItem("pressedTitleTTFFontSize", fontSize);
            options->insertItem("pressedTitleTTFFontSize", fontSize);
        }
        else if (strcmp(name, "labelAnchorPoint") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float anchor_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float anchor_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            controlButton->setLabelAnchorPoint(ccp(anchor_x, anchor_y));
            
            options->insertItem("labelAnchorPointX", anchor_x);
            options->insertItem("labelAnchorPointY", anchor_y);
        }
        else if (strcmp(name, "preferedSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            controlButton->setPreferredSize(size);
            
            options->insertItem("width", width);
            options->insertItem("height", height);
        }
        else if (strcmp(name, "zoomOnTouchDown") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            controlButton->setZoomOnTouchDown(value);
            
            options->insertItem("zoomOnTouchDown", value);
        }
        else if (strcmp(name, "backgroundSpriteFrame|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            controlButton->setNormalBackgroundSpriteFrame(spriteFrame);
            
            options->insertItem("normal", flieName);
        }
        else if (strcmp(name, "titleColor|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            controlButton->setNormalFontColor(color);
            
            options->insertItem("normalTitleColorB", blue);
            options->insertItem("normalTitleColorG", green);
            options->insertItem("normalTitleColorR", red);
        }
        else if (strcmp(name, "backgroundSpriteFrame|2") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            controlButton->setPressedBackgroundSpriteFrame(spriteFrame);
            
            options->insertItem("pressed", flieName);
        }
        else if (strcmp(name, "titleColor|2") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            controlButton->setPressedFontColor(color);
            
            options->insertItem("pressedTitleColorB", blue);
            options->insertItem("pressedTitleColorG", green);
            options->insertItem("pressedTitleColorR", red);
        }
        else if (strcmp(name, "backgroundSpriteFrame|3") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            controlButton->setDisabledBackgroundSpriteFrame(spriteFrame);
            
            options->insertItem("disabled", flieName);
        }
        else if (strcmp(name, "titleColor|3") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            controlButton->setDisalbedFontColor(color);
            
            options->insertItem("disabledTitleColorB", blue);
            options->insertItem("disabledTitleColorG", green);
            options->insertItem("disabledTitleColorR", red);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
            
            options->insertItem("opacity", value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
            
            options->insertItem("colorB", blue);
            options->insertItem("colorG", green);
            options->insertItem("colorR", red);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
            
            options->insertItem("blendFuncSource", source);
            options->insertItem("blendFuncDestination", destination);
        }
    }
}

UIWidget* CCSCCBReader::widgetFromCCBFile(const char *fileName)
{
    CCDictionary* root = CCDictionary::createWithContentsOfFile(fileName);
 
 
    // resolutions
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 
    int index = (winSize.width < 1024) ? 0 : 1;
 
    CCArray* resolutions = dynamic_cast<CCArray*>(DICTOOL->getArrayValue(root, "resolutions"));
    CCDictionary* item = dynamic_cast<CCDictionary*>(resolutions->objectAtIndex(index));
    //
     int height = DICTOOL->getIntValue(item, "height");
     int width = DICTOOL->getIntValue(item, "width");
 
    resolutionScale = DICTOOL->getFloatValue(item, "scale");
 
    // nodeGraph
    CCDictionary* nodeGraph = dynamic_cast<CCDictionary*>(DICTOOL->getArrayValue(root, "nodeGraph"));
 
    // widget
    UIWidget* widget = widgetFromCCB(nodeGraph, NULL);
 
    // sequences
 
    return widget;
} 

UIWidget* CCSCCBReader::widgetFromCCB(CCDictionary *nodeGraph, UIWidget* parent)
{
    using namespace cs;
 
    UIWidget* widget = NULL;
 
    const char* baseClass = DICTOOL->getStringValue(nodeGraph, "baseClass");
 
    if (baseClass && strcmp(baseClass, "CCNode") == 0)
    {
        widget = UIWidget::create();
        setPropsForWidgetFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCLayer") == 0)
    {
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCLayerColor") == 0)
    {
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCLayerGradient") == 0)
    {
        widget = UIGradientPanel::create();
        setPropsForGradientPanelFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCScrollView") == 0)
    {
        widget = UIScrollView::create();
        setPropsForScrollViewFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCMenu") == 0)
    {
        widget = UIPanel::create();
        setPropsForPanelFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCSprite") == 0)
    {
        widget = UIImageView::create();
        setPropsForImageViewFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCScale9Sprite") == 0)
    {
        widget = UIImageView::create();
        setPropsForImageViewScale9FromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCLabelTTF") == 0)
    {
        widget = UITextArea::create();
        setPropsForTextAreaFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCLabelBMFont") == 0)
    {
        widget = UILabelBMFont::create();
        setPropsForLabelBMFontFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCControlButton") == 0)
    {
        widget = UIControlButton::create();
        setPropsForControlButtonFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCMenuItemImage") == 0)
    {
        widget = UIButton::create();
        setPropsForButtonFromCCB(widget, parent, nodeGraph);
    }
    else if (baseClass && strcmp(baseClass, "CCBFile") == 0)
    {
        CCArray* properties = DICTOOL->getArrayValue(nodeGraph, "properties");
 
        CCObject* obj = NULL;
        CCARRAY_FOREACH(properties, obj)
        {
            CCDictionary* item = static_cast<CCDictionary*>(obj);
 
            const char* name = DICTOOL->getStringValue(item, "name");
            if (strcmp(name, "ccbFile") == 0)
            {
                const char* value = DICTOOL->getStringValue(item, "value");
 
                CCDictionary* root = CCDictionary::createWithContentsOfFile(value);
                // nodeGraph
                CCDictionary* nodeGraph = dynamic_cast<CCDictionary*>(DICTOOL->getArrayValue(root, "nodeGraph"));
                // widget
                widget = widgetFromCCB(nodeGraph, parent);
 
                break;
            }
        }
 
        CCARRAY_FOREACH(properties, obj)
        {
            CCDictionary* item = static_cast<CCDictionary*>(obj);
 
            const char* name = DICTOOL->getStringValue(item, "name");
            if (strcmp(name, "position") == 0)
            {
                CCArray* value = DICTOOL->getArrayValue(item, "value");
 
                float x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
                float y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
                int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
                CCPoint pt = ccp(x, y);
                pt = getAbsolutePosition(pt, type, parent);
                
                //
                 CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                 if (pt.y >= winSize.height)
                 {
                 pt.setPoint(pt.x, pt.y - widget->getContainerNode()->getContentSize().height);
                 }
                 //
                
                widget->setPosition(pt);
            }
            else if (strcmp(name, "scale") == 0)
            {
                CCArray* value = DICTOOL->getArrayValue(item, "value");
                float scale_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
                float scale_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
                
                widget->setScaleX(scale_x);
                widget->setScaleY(scale_y);
            }
            else if (strcmp(name, "visible") == 0)
            {
                bool value = DICTOOL->getBooleanValue(item, "value");
                
                widget->setVisible(value);
            }
        }
    }
    else if (baseClass && strcmp(baseClass, "CCParticleSystemQuad") == 0)
    {
        int a = 0;
        a = 1;
    }
    
    CCArray* children = DICTOOL->getArrayValue(nodeGraph, "children");
    if (children)
    {
        for (int i = 0; i < children->count(); i++)
        {
            cocos2d::CCDictionary* subNodeGraph = (cocos2d::CCDictionary*)(children->objectAtIndex(i));
            UIWidget* child = widgetFromCCB(subNodeGraph, widget);
            if (child)
            {
                widget->addChild(child);
            }
        }
    }
    
    // animation
    bool animatedProperties_exist = DICTOOL->checkObjectExist(nodeGraph, "animatedProperties");
    if (animatedProperties_exist)
    {
        //        CCArray* animatedProperties = DICTOOL->getArrayValue(nodeGraph, "animatedProperties");
    }
    
    return widget;
}

void CCSCCBReader::setPropsForWidgetFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    // memberVarAssignmentName (widget name)
    const char* memberVarAssignmentName = DICTOOL->getStringValue(options, "memberVarAssignmentName");
    const char* widgetName = (strcmp(memberVarAssignmentName, "") == 0) ? "default" : memberVarAssignmentName;
    widget->setName(widgetName);
    
    // widget initialize properties
    widget->setVisible(true);
    widget->setAnchorPoint(ccp(0.5, 0.5));
    
    // properties
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "position") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            float x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCPoint pt = ccp(x, y);
            pt = getAbsolutePosition(pt, type, parent);
            
            WidgetName widgetName = parent->getWidgetName();
            if (widgetName == WIDGET_IMAGEVIEW)
            {
                pt = ccp(pt.x + (-parent->getRect().size.width / 2), pt.y + (-parent->getRect().size.height / 2));
            }
                        
            widget->setPosition(pt);
        }
        else if (strcmp(name, "contentSize") == 0)
        {
//             CCArray* value = DICTOOL->getArrayValue(item, "value");
//             float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
//             float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
//             int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
//             CCSize size = parsePropTypeSize(width, height, type, parent);
        }
        else if (strcmp(name, "anchorPoint") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float anchorpoint_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float anchorpoint_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            widget->setAnchorPoint(ccp(anchorpoint_x, anchorpoint_y));
        }
        else if (strcmp(name, "scale") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* str = dynamic_cast<CCString*>(value);
            if (str)
            {
                float scale = str->floatValue();
                
                widget->setScale(scale);
                
                continue;
            }
            
            float scale_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float scale_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            widget->setScaleX(scale_x);
            widget->setScaleY(scale_y);
        }
        else if (strcmp(name, "rotation") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            widget->setRotation(value);
        }
        else if (strcmp(name, "skew") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float skew_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float skew_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            widget->setSkewX(skew_x);
            widget->setSkewY(skew_y);
        }
        else if (strcmp(name, "tag") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setWidgetTag(value);
        }
        else if (strcmp(name, "ignoreAnchorPointForPosition") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->ignoreAnchorPointForPosition(value);
        }
        else if (strcmp(name, "touchEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->setBeTouchEnable(value);
        }
        else if (strcmp(name, "isTouchEnabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->setBeTouchEnable(value);
        }
        else if (strcmp(name, "accelerometerEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isAccelerometerEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "mouseEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isMouseEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "keyboardEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "isKeyboardEnabled") == 0)
        {
            
        }
        else if (strcmp(name, "visible") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            widget->setVisible(value);
        }
        else if (strcmp(name, "seqExpanded") == 0)
        {
            
        }
    }
    
    //    bool seqExpanded = DICTOOL->getBooleanValue(options, "seqExpanded");
    
    // customProperties
}

void CCSCCBReader::setColorPropsForWidgetFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    
}

void CCSCCBReader::setPropsForButtonFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    
    UIButton* button = static_cast<UIButton*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "block") == 0)
        {
            //            CCArray* value = DICTOOL->getArrayValue(item, "value");
        }
        else if (strcmp(name, "isEnabled") == 0)
        {
            //            bool value = DICTOOL->getBooleanValue(item, "isEnabled");
            
            button->setBeTouchEnable(true);
            //            button->setBeTouchAble(value);
        }
        else if (strcmp(name, "normalSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            button->setNormalSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "selectedSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            button->setPressedSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "disabledSpriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            button->setDisabledSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
        }
    }
}

void CCSCCBReader::setPropsForImageViewFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    
    UIImageView* imageView = static_cast<UIImageView*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "displayFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* spriteSheetFileName = static_cast<CCString*>(value->objectAtIndex(0));
            std::string spriteSheet = spriteSheetFileName->getCString();
            
            CCString* spriteFlieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = spriteFlieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                if (spriteSheet.length() == 0)
                {
                    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                    CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                    spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
                }
                else
                {
                    CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
                    frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                    
                    spriteFrame = frameCache->spriteFrameByName(spriteFile.c_str());
                }
            }
            
            imageView->setDisplayFrame(spriteFrame);
        }
        else if (strcmp(name, "flip") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            bool flipX = dynamic_cast<CCString*>(value->objectAtIndex(0))->boolValue();
            bool flipY = dynamic_cast<CCString*>(value->objectAtIndex(1))->boolValue();
            
            imageView->setFlipX(flipX);
            imageView->setFlipY(flipY);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            imageView->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            imageView->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            imageView->setBlendFunc(blendFunc);
        }
    }
}

void CCSCCBReader::setPropsForImageViewScale9FromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    
    UIImageView* imageView = static_cast<UIImageView*>(widget);
    imageView->setScale9Enable(true);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "spriteFrame") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* spriteSheetFileName = static_cast<CCString*>(value->objectAtIndex(0));
            std::string spriteSheet = spriteSheetFileName->getCString();
            
            CCString* spriteFlieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = spriteFlieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                if (spriteSheet.length() == 0)
                {
                    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                    CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                    spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
                }
                else
                {
                    CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
                    frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                    
                    spriteFrame = frameCache->spriteFrameByName(spriteFile.c_str());
                }
            }
            
            imageView->setSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "preferedSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            imageView->setPreferredSize(size);
        }
        else if (strcmp(name, "insetLeft") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetLeft(value);
        }
        else if (strcmp(name, "insetTop") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetTop(value);
        }
        else if (strcmp(name, "insetRight") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetRight(value);
        }
        else if (strcmp(name, "insetBottom") == 0)
        {
            float value = DICTOOL->getFloatValue(item, "value");
            
            imageView->setInsetBottom(value);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            imageView->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            imageView->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            imageView->setBlendFunc(blendFunc);
        }
    }
}

void CCSCCBReader::setPropsForContainerWidgetFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    UIContainerWidget* containerWidget = (UIContainerWidget*)widget;
    
    // ContainerWidget unique properties
    containerWidget->setClipAble(false);
}

void CCSCCBReader::setPropsForPanelFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForContainerWidgetFromCCB(widget, parent, options);
    
    UIPanel* panel = static_cast<UIPanel*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "contentSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            panel->setSize(size);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
        }
    }    
}

void CCSCCBReader::setPropsForGradientPanelFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForPanelFromCCB(widget, parent, options);
    
    UIGradientPanel* gradientPanel = static_cast<UIGradientPanel*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "startColor") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            gradientPanel->setStartColor(color);
        }
        else if (strcmp(name, "startOpacity") == 0)
        {
            GLubyte value = DICTOOL->getIntValue(item, "value");
            
            gradientPanel->setStartOpacity(value);
        }
        else if (strcmp(name, "endColor") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            gradientPanel->setEndColor(color);
        }
        else if (strcmp(name, "endOpacity") == 0)
        {
            GLubyte value = DICTOOL->getIntValue(item, "value");
            
            gradientPanel->setEndOpacity(value);
        }
        else if (strcmp(name, "vector") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float vector_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float vector_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            gradientPanel->setVector(ccp(vector_x, vector_y));
        }
    }
}

void CCSCCBReader::setPropsForScrollViewFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForPanelFromCCB(widget, parent, options);
    
    UIScrollView* scrollView = static_cast<UIScrollView*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        
        const char* name = DICTOOL->getStringValue(item, "name");
        if (strcmp(name, "container") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            CCDictionary* root = CCDictionary::createWithContentsOfFile(value);
            // nodeGraph
            CCDictionary* nodeGraph = dynamic_cast<CCDictionary*>(DICTOOL->getArrayValue(root, "nodeGraph"));
            // container of scrollview
            UIWidget* container = widgetFromCCB(nodeGraph, widget);
            widget->addChild(container);
        }
        else if (strcmp(name, "direction") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            SCROLLVIEW_DIR eDirection = SCROLLVIEW_DIR_NONE;
            
            switch (value)
            {
                case 0: // horizontal
                    eDirection = SCROLLVIEW_DIR_HORIZONTAL;
                    break;
                    
                case 1: // vertical
                    eDirection = SCROLLVIEW_DIR_VERTICAL;
                    break;
                    
                default:
                    break;
            }
            
            scrollView->setDirection(eDirection);
        }
        else if (strcmp(name, "clipsToBounds") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            scrollView->setClipAble(value);
        }
        else if (strcmp(name, "bounces") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            if (value)
            {
                scrollView->setUpdateEnable(true);
                scrollView->setMoveMode(SCROLLVIEW_MOVE_MODE_ACTION);
            }
        }
    }
}

void CCSCCBReader::setPropsForTextAreaFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    
    UITextArea* textArea = static_cast<UITextArea*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "fontName") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            textArea->setFontName(value);
        }
        else if (strcmp(name, "fontSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float fontSize = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            //            float unknown = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            textArea->setFontSize(fontSize);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            textArea->setTextColor(color.r, color.g, color.b);
        }
        else if (strcmp(name, "dimensions") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            textArea->setTextAreaSize(size);
        }
        else if (strcmp(name, "horizontalAlignment") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            textArea->setTextHorizontalAlignment((CCTextAlignment)value);
        }
        else if (strcmp(name, "verticalAlignment") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            textArea->setTextVerticalAlignment((CCVerticalTextAlignment)value);
        }
        else if (strcmp(name, "string") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            textArea->setText(value);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
        }
    }
}

void CCSCCBReader::setPropsForLabelBMFontFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    
    UILabelBMFont* labelBMFont = static_cast<UILabelBMFont*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "fntFile") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            labelBMFont->setFntFile(value);
        }
        else if (strcmp(name, "string") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            labelBMFont->setText(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            labelBMFont->setColor(color);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
        }
    }
}

void CCSCCBReader::setPropsForControlButtonFromCCB(UIWidget *widget, UIWidget* parent, CCDictionary *options)
{
    setPropsForWidgetFromCCB(widget, parent, options);
    
    UIControlButton* controlButton = static_cast<UIControlButton*>(widget);
    
    CCArray* properties = DICTOOL->getArrayValue(options, "properties");
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(properties, obj)
    {
        CCDictionary* item = static_cast<CCDictionary*>(obj);
        const char* name = DICTOOL->getStringValue(item, "name");
        
        if (strcmp(name, "ccControl") == 0)
        {
            //            bool value = DICTOOL->getBooleanValue(item, "value");
        }
        else if (strcmp(name, "enabled") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            controlButton->setBeTouchEnable(value);
        }
        else if (strcmp(name, "title|1") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            controlButton->setNormalTitle(value);
            controlButton->setPressedTitle(value);
            controlButton->setDisabledTitle(value);
        }
        else if (strcmp(name, "titleTTF|1") == 0)
        {
            const char* value = DICTOOL->getStringValue(item, "value");
            
            controlButton->setNormalFontName(value);
            controlButton->setPressedFontName(value);
            controlButton->setDisabledFontName(value);
        }
        else if (strcmp(name, "titleTTFSize|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float fontSize = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            
            if(type == 1) // kCCBScaleTypeMultiplyResolution
            {
                fontSize *= resolutionScale;
            }
            
            controlButton->setNormalFontSize(fontSize);
            controlButton->setPressedFontSize(fontSize);
            controlButton->setDisabledFontSize(fontSize);
        }
        else if (strcmp(name, "labelAnchorPoint") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float anchor_x = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float anchor_y = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            
            controlButton->setLabelAnchorPoint(ccp(anchor_x, anchor_y));
        }
        else if (strcmp(name, "preferedSize") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            float width = static_cast<CCString*>(value->objectAtIndex(0))->floatValue();
            float height = static_cast<CCString*>(value->objectAtIndex(1))->floatValue();
            int type = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            CCSize size = parsePropTypeSize(width, height, type, parent);
            
            controlButton->setPreferredSize(size);
        }
        else if (strcmp(name, "zoomOnTouchDown") == 0)
        {
            bool value = DICTOOL->getBooleanValue(item, "value");
            
            controlButton->setZoomOnTouchDown(value);
        }
        else if (strcmp(name, "backgroundSpriteFrame|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            controlButton->setNormalBackgroundSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "titleColor|1") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            controlButton->setNormalFontColor(color);
        }
        else if (strcmp(name, "backgroundSpriteFrame|2") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            controlButton->setPressedBackgroundSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "titleColor|2") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            controlButton->setPressedFontColor(color);
        }
        else if (strcmp(name, "backgroundSpriteFrame|3") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            CCString* flieName = static_cast<CCString*>(value->objectAtIndex(1));
            std::string spriteFile = flieName->getCString();
            
            CCSpriteFrame *spriteFrame = NULL;
            
            if (spriteFile.length() != 0)
            {
                CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(spriteFile.c_str());
                CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, bounds);
            }
            
            controlButton->setDisabledBackgroundSpriteFrame(spriteFrame);
        }
        else if (strcmp(name, "titleColor|3") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            GLubyte red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            GLubyte green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            GLubyte blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            controlButton->setDisalbedFontColor(color);
        }
        else if (strcmp(name, "opacity") == 0)
        {
            int value = DICTOOL->getIntValue(item, "value");
            
            widget->setOpacity(value);
        }
        else if (strcmp(name, "color") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int red = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int green = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            int blue = static_cast<CCString*>(value->objectAtIndex(2))->intValue();
            ccColor3B color = { red, green, blue };
            
            widget->setColor(color);
        }
        else if (strcmp(name, "blendFunc") == 0)
        {
            CCArray* value = DICTOOL->getArrayValue(item, "value");
            
            int source = static_cast<CCString*>(value->objectAtIndex(0))->intValue();
            int destination = static_cast<CCString*>(value->objectAtIndex(1))->intValue();
            ccBlendFunc blendFunc;
            blendFunc.src = source;
            blendFunc.dst = destination;
            
            widget->setBlendFunc(blendFunc);
        }
    }
}

CCSize CCSCCBReader::parsePropTypeSize(float width, float height, int type, UIWidget* parent)
{
    if (parent)
    {
        CCSize containerSize = parent->getContainerNode()->getContentSize();
        switch (type)
        {
            case 0: // kCCBSizeTypeAbsolute
            {
                // Nothing.
                break;
            }
            case 1: // kCCBSizeTypePercent
            {
                width = (int)(containerSize.width * width / 100.0f);
                height = (int)(containerSize.height * height / 100.0f);
                break;
            }
            case 2: // kCCBSizeTypeRelativeContainer
            {
                width = containerSize.width - width;
                height = containerSize.height - height;
                break;
            }
            case 3: // kCCBSizeTypeHorizontalPercent
            {
                width = (int)(containerSize.width * width / 100.0f);
                break;
            }
            case 4: // kCCBSizeTypeVerticalPercent
            {
                height = (int)(containerSize.height * height / 100.0f);
                break;
            }
            case 5: // kCCBSizeTypeMultiplyResolution
            {
                width *= resolutionScale;
                height *= resolutionScale;
                break;
            }
            default:
            {
//                CCLog("Unknown CCB type.");
            }
                break;
        }
    }
    else
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        width = winSize.width;
        height = winSize.height;
    }
    
    return CCSizeMake(width, height);
}

CCPoint CCSCCBReader::getAbsolutePosition(const cocos2d::CCPoint &pt, int nType, UIWidget *parent)
{
    CCPoint absPt = ccp(0,0);
    
    if (parent)
    {
        CCSize containerSize = parent->getContainerNode()->getContentSize();
        
        if (nType == 0) // kCCBPositionTypeRelativeBottomLeft
        {
            absPt = pt;
        }
        else if (nType == 1) // kCCBPositionTypeRelativeTopLeft
        {
            absPt.x = pt.x;
            absPt.y = containerSize.height - pt.y;
        }
        else if (nType == 2) // kCCBPositionTypeRelativeTopRight
        {
            absPt.x = containerSize.width - pt.x;
            absPt.y = containerSize.height - pt.y;
        }
        else if (nType == 3) // kCCBPositionTypeRelativeBottomRight
        {
            absPt.x = containerSize.width - pt.x;
            absPt.y = pt.y;
        }
        else if (nType == 4) // kCCBPositionTypePercent
        {
            absPt.x = (int)(containerSize.width * pt.x / 100.0f);
            absPt.y = (int)(containerSize.height * pt.y / 100.0f);
        }
        else if (nType == 5) // kCCBPositionTypeMultiplyResolution
        {
            absPt.x = pt.x * resolutionScale;
            absPt.y = pt.y * resolutionScale;
        }
    }
    else
    {
        absPt = pt;
    }
    
    return absPt;
}

NS_CC_EXT_END

/**/
