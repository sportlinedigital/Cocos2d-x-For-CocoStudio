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

#include "CocosGUI.h"
#include "../../JsonReader/DictionaryHelper.h"
#include "../Action/UIActionManager.h"
#include <fstream>
#include <iostream>


NS_CC_EXT_BEGIN
    
static CCSReader* sharedReader = NULL;

CCSReader::CCSReader():
m_strFilePath("")
{
    
}

CCSReader::~CCSReader()
{
    
}

CCSReader* CCSReader::shareReader()
{
    if (!sharedReader)
    {
        sharedReader = new CCSReader();
    }
    return sharedReader;
}

UIWidget* CCSReader::widgetFromJsonDictionary(cs::CSJsonDictionary* data)
{
    UIWidget* widget = NULL;
    const char* classname = DICTOOL->getStringValue_json(data, "classname");
    cs::CSJsonDictionary* uiOptions = DICTOOL->getSubDictionary_json(data, "options");
    if (classname && strcmp(classname, "Button") == 0)
    {
        widget = UIButton::create();
        this->setPropsForButtonFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "CheckBox") == 0)
    {
        widget = UICheckBox::create();
        this->setPropsForCheckBoxFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Label") == 0)
    {
        widget = UILabel::create();
        this->setPropsForLabelFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LabelAtlas") == 0)
    {
        widget = UILabelAtlas::create();
        this->setPropsForLabelAtlasFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LoadingBar") == 0)
    {
        widget = UILoadingBar::create();
        this->setPropsForLoadingBarFromJsonDictionary(widget, uiOptions);
    }else if (classname && strcmp(classname, "ScrollView") == 0){
        widget = UIScrollView::create();
        this->setPropsForScrollViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextArea") == 0)
    {
        widget = UITextArea::create();
        this->setPropsForTextAreaFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextButton") == 0)
    {
        widget = UITextButton::create();
        this->setPropsForTextButtonFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextField") == 0)
    {
        widget = UITextField::create();
        this->setPropsForTextFieldFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "ImageView") == 0)
    {
        widget = UIImageView::create();
        this->setPropsForImageViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Panel") == 0)
    {
        widget = UIPanel::create();
        this->setPropsForPanelFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Slider") == 0)
    {
        widget = UISlider::create();
        this->setPropsForSliderFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "ListView") == 0)
    {
        widget = UIListView::create();
        this->setPropsForListViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "PageView") == 0)
    {
        widget = UIPageView::create();
        this->setPropsForPageViewFromJsonDictionary(widget, uiOptions);
    }
    int childrenCount = DICTOOL->getArrayCount_json(data, "children");
    for (int i=0;i<childrenCount;i++)
    {
        cs::CSJsonDictionary* subData = DICTOOL->getDictionaryFromArray_json(data, "children", i);
        UIWidget* child = this->widgetFromJsonDictionary(subData);
        if (child)
        {
            widget->addChild(child);
        }
    }
    return widget;
}

UIWidget* CCSReader::widgetFromJsonFile(const char *fileName)
{
    const char *des = NULL;
    std::string jsonpath;
    cs::CSJsonDictionary *jsonDict = NULL;
    jsonpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    
    unsigned long size = 0;
    des = (char*)(cocos2d::CCFileUtils::sharedFileUtils()->getFileData(jsonpath.c_str(),"r" , &size));
    jsonDict = new cs::CSJsonDictionary();
    jsonDict->initWithDescription(des);
    if(NULL == des || strcmp(des, "") == 0)
    {
        printf("read json file[%s] error!\n", fileName);
    }
//        float fileVersion = DICTOOL->getFloatValue_json(jsonDict, "version");
//        if (fileVersion != kCCSVersion) {
//            printf("WARNING! Incompatible json file version (file: %f reader: %f)\n", fileVersion, kCCSVersion);
//            return NULL;
//        }
    int texturesCount = DICTOOL->getArrayCount_json(jsonDict, "textures");
    int pos = jsonpath.find_last_of('/');
	m_strFilePath = jsonpath.substr(0,pos+1);
    for (int i=0; i<texturesCount; i++)
    {
        const char* file = DICTOOL->getStringValueFromArray_json(jsonDict, "textures", i);
        std::string tp = m_strFilePath;
        tp.append(file);
        CCUIHELPER->addSpriteFrame(tp.c_str());
    }
    float fileDesignWidth = DICTOOL->getFloatValue_json(jsonDict, "designWidth");
    float fileDesignHeight = DICTOOL->getFloatValue_json(jsonDict, "designHeight");
    if (fileDesignWidth <= 0 || fileDesignHeight <= 0) {
        printf("Read design size error!\n");
        cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
        CCUIHELPER->setFileDesignWidth(winSize.width);
        CCUIHELPER->setFileDesignHeight(winSize.height);
    }
    else
    {
        CCUIHELPER->setFileDesignWidth(fileDesignWidth);
        CCUIHELPER->setFileDesignHeight(fileDesignHeight);
    }
    cs::CSJsonDictionary* widgetTree = DICTOOL->getSubDictionary_json(jsonDict, "widgetTree");
    UIWidget* widget = this->widgetFromJsonDictionary(widgetTree);
    widget->setFileDesignSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
    cs::CSJsonDictionary* actions = DICTOOL->getSubDictionary_json(jsonDict, "animation");
    UIActionManager::shareManager()->initWithDictionary(actions,widget);
    delete jsonDict;
    jsonDict = NULL;
    delete[] des;
    return widget;
}

void CCSReader::setPropsForWidgetFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary *options)
{
    widget->setWidgetTag(DICTOOL->getIntValue_json(options, "tag"));
    widget->setBeTouchEnable(DICTOOL->getBooleanValue_json(options, "touchAble"));
    const char* name = DICTOOL->getStringValue_json(options, "name");
    const char* widgetName = name?name:"default";
    widget->setName(widgetName);
    float x = DICTOOL->getFloatValue_json(options, "x");
    float y = DICTOOL->getFloatValue_json(options, "y");
    widget->setPosition(ccp(x,y));
    bool sx = DICTOOL->checkObjectExist_json(options, "scaleX");
    if (sx)
    {
        widget->setScaleX(DICTOOL->getFloatValue_json(options, "scaleX"));
    }
    bool sy = DICTOOL->checkObjectExist_json(options, "scaleY");
    if (sy)
    {
        widget->setScaleY(DICTOOL->getFloatValue_json(options, "scaleY"));
    }
    bool rt = DICTOOL->checkObjectExist_json(options, "rotation");
    if (rt)
    {
        widget->setRotation(DICTOOL->getFloatValue_json(options, "rotation"));
    }
    bool vb = DICTOOL->checkObjectExist_json(options, "visible");
    if (vb)
    {
        widget->setVisible(DICTOOL->getBooleanValue_json(options, "visible"));
    }
    widget->setUseMergedTexture(DICTOOL->getBooleanValue_json(options, "useMergedTexture"));
    int z = DICTOOL->getIntValue_json(options, "ZOrder");
    widget->setWidgetZOrder(z);
}

void CCSReader::setColorPropsForWidgetFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary *options)
{
    bool op = DICTOOL->checkObjectExist_json(options, "opacity");
    if (op)
    {
        widget->setOpacity(DICTOOL->getIntValue_json(options, "opacity"));
    }
    bool cr = DICTOOL->checkObjectExist_json(options, "colorR");
    bool cg = DICTOOL->checkObjectExist_json(options, "colorG");
    bool cb = DICTOOL->checkObjectExist_json(options, "colorB");
    int colorR = cr ? DICTOOL->getIntValue_json(options, "colorR") : 255;
    int colorG = cg ? DICTOOL->getIntValue_json(options, "colorG") : 255;
    int colorB = cb ? DICTOOL->getIntValue_json(options, "colorB") : 255;
    widget->setColor(ccc3(colorR, colorG, colorB));
    bool apx = DICTOOL->checkObjectExist_json(options, "anchorPointX");
    float apxf = apx ? DICTOOL->getFloatValue_json(options, "anchorPointX") : 0.5f;
    bool apy = DICTOOL->checkObjectExist_json(options, "anchorPointY");
    float apyf = apy ? DICTOOL->getFloatValue_json(options, "anchorPointY") : 0.5f;
    widget->setAnchorPoint(ccp(apxf, apyf));
}

void CCSReader::setPropsForButtonFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UIButton* button = (UIButton*)widget;
    bool scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    button->setScale9Enable(scale9Enable);
    
	std::string tp_n = m_strFilePath;
	std::string tp_p = m_strFilePath;
	std::string tp_d = m_strFilePath;

    const char* normalFileName = DICTOOL->getStringValue_json(options, "normal");
    const char* pressedFileName = DICTOOL->getStringValue_json(options, "pressed");
    const char* disabledFileName = DICTOOL->getStringValue_json(options, "disabled");

	const char* normalFileName_tp = normalFileName?tp_n.append(normalFileName).c_str():NULL;
	const char* pressedFileName_tp = pressedFileName?tp_p.append(pressedFileName).c_str():NULL;
	const char* disabledFileName_tp =  disabledFileName?tp_d.append(disabledFileName).c_str():NULL;
    
    if (scale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
		if (widget->getUseMergedTexture())
		{
			 button->setTexturesScale9(normalFileName, pressedFileName, disabledFileName, cocos2d::CCRect(cx, cy, cw, ch),true);
		}
		else
		{
			 button->setTexturesScale9(normalFileName_tp, pressedFileName_tp, disabledFileName_tp, cocos2d::CCRect(cx, cy, cw, ch));
		}
        bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
        bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
        if (sw && sh)
        {
            float swf = DICTOOL->getFloatValue_json(options, "scale9Width");
            float shf = DICTOOL->getFloatValue_json(options, "scale9Height");
            button->setScale9Size(swf, shf);
        }
    }
    else
    {
		if (widget->getUseMergedTexture())
		{
			 button->setTextures(normalFileName, pressedFileName, disabledFileName,true);
		}
		else
		{
			 button->setTextures(normalFileName_tp, pressedFileName_tp, disabledFileName_tp);
		}
    }
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForCheckBoxFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UICheckBox* checkBox = (UICheckBox*)widget;
    const char* backGroundFileName = DICTOOL->getStringValue_json(options, "backGroundBox");
    const char* backGroundSelectedFileName = DICTOOL->getStringValue_json(options, "backGroundBoxSelected");
    const char* frontCrossFileName = DICTOOL->getStringValue_json(options, "frontCross");
    const char* backGroundDisabledFileName = DICTOOL->getStringValue_json(options, "backGroundBoxDisabled");
    const char* frontCrossDisabledFileName = DICTOOL->getStringValue_json(options, "frontCrossDisabled");


	std::string tp_b = m_strFilePath;
	std::string tp_bs = m_strFilePath;
	std::string tp_c = m_strFilePath;
	std::string tp_bd = m_strFilePath;
	std::string tp_cd = m_strFilePath;

	const char* backGroundFileName_tp = backGroundFileName?tp_b.append(backGroundFileName).c_str():NULL;
	const char* backGroundSelectedFileName_tp = backGroundSelectedFileName?tp_bs.append(backGroundSelectedFileName).c_str():NULL;
	const char* frontCrossFileName_tp = frontCrossFileName?tp_c.append(frontCrossFileName).c_str():NULL;
	const char* backGroundDisabledFileName_tp = backGroundDisabledFileName?tp_bd.append(backGroundDisabledFileName).c_str():NULL;
	const char* frontCrossDisabledFileName_tp = frontCrossDisabledFileName?tp_cd.append(frontCrossDisabledFileName).c_str():NULL;

	if (widget->getUseMergedTexture())
	{
		checkBox->setTextures(backGroundFileName, backGroundSelectedFileName, frontCrossFileName,backGroundDisabledFileName,frontCrossDisabledFileName,true);
	}
	else
	{
		checkBox->setTextures(backGroundFileName_tp, backGroundSelectedFileName_tp, frontCrossFileName_tp,backGroundDisabledFileName_tp,frontCrossDisabledFileName_tp);
	}
	
    
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForImageViewFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    
    UIImageView* imageView = (UIImageView*)widget;
    const char* imageFileName = DICTOOL->getStringValue_json(options, "fileName");
    bool scale9EnableExist = DICTOOL->checkObjectExist_json(options, "scale9Enable");
    bool scale9Enable = false;
    if (scale9EnableExist)
    {
        scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    }
    imageView->setScale9Enable(scale9Enable);

	std::string tp_i = m_strFilePath;
	const char* imageFileName_tp = NULL;
	if (imageFileName)
	{
		imageFileName_tp = tp_i.append(imageFileName).c_str();
	}
	

    if (scale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        if (widget->getUseMergedTexture())
        {
			imageView->setTexture(imageFileName,true);
        }
		else
		{
			imageView->setTexture(imageFileName_tp);
		}
        
        
        imageView->setCapInset(cocos2d::CCRect(cx, cy, cw, ch));
        bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
        bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
        if (sw && sh)
        {
            float swf = DICTOOL->getFloatValue_json(options, "scale9Width");
            float shf = DICTOOL->getFloatValue_json(options, "scale9Height");
            imageView->setScale9Size(swf, shf);
        }
    }
    else
    {
		if (widget->getUseMergedTexture())
		{
			imageView->setTexture(imageFileName,true);
		}
		else
		{
			imageView->setTexture(imageFileName_tp);
		}
    }
    bool flipX = DICTOOL->getBooleanValue_json(options, "flipX");
    bool flipY = DICTOOL->getBooleanValue_json(options, "flipY");
    imageView->setFlipX(flipX);
    imageView->setFlipY(flipY);
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForLabelFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UILabel* label = (UILabel*)widget;
    bool touchScaleChangeAble = DICTOOL->getBooleanValue_json(options, "touchSacleEnable");
    label->setTouchScaleChangeAble(touchScaleChangeAble);
    const char* text = DICTOOL->getStringValue_json(options, "text");
    label->setText(text);
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        label->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        label->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    bool cro = DICTOOL->checkObjectExist_json(options, "colorR");
    bool cgo = DICTOOL->checkObjectExist_json(options, "colorG");
    bool cbo = DICTOOL->checkObjectExist_json(options, "colorB");
    int cr = cro?DICTOOL->getIntValue_json(options, "colorR"):255;
    int cg = cgo?DICTOOL->getIntValue_json(options, "colorG"):255;
    int cb = cbo?DICTOOL->getIntValue_json(options, "colorB"):255;
    ccColor3B tc = ccc3(cr, cg, cb);
    label->setColor(tc);
    label->setFlipX(DICTOOL->getBooleanValue_json(options, "flipX"));
    label->setFlipY(DICTOOL->getBooleanValue_json(options, "flipY"));
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
    int gravity = DICTOOL->getIntValue_json(options, "gravity");
    label->setGravity((LabelGravity)gravity);
}

void CCSReader::setPropsForLabelAtlasFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UILabelAtlas* labelAtlas = (UILabelAtlas*)widget;
    bool sv = DICTOOL->checkObjectExist_json(options, "stringValue");
    bool cmf = DICTOOL->checkObjectExist_json(options, "charMapFile");
    bool iw = DICTOOL->checkObjectExist_json(options, "itemWidth");
    bool ih = DICTOOL->checkObjectExist_json(options, "itemHeight");
    bool scm = DICTOOL->checkObjectExist_json(options, "startCharMap");
    if (sv && cmf && iw && ih && scm)
    {
		std::string tp_c = m_strFilePath;
		const char* cmf_tp = NULL;
		const char* cmft = DICTOOL->getStringValue_json(options, "charMapFile");
		cmf_tp = tp_c.append(cmft).c_str();

        labelAtlas->setProperty(DICTOOL->getStringValue_json(options, "stringValue"),cmf_tp,DICTOOL->getIntValue_json(options, "itemWidth"),DICTOOL->getIntValue_json(options,"itemHeight"),DICTOOL->getStringValue_json(options, "startCharMap"),widget->getUseMergedTexture());
    }
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForContainerWidgetFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary *options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UIContainerWidget* containerWidget = (UIContainerWidget*)widget;
    containerWidget->setClippingEnable(DICTOOL->getBooleanValue_json(options, "clipAble"));
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForPanelFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForContainerWidgetFromJsonDictionary(widget, options);
    UIPanel* panel = (UIPanel*)widget;
    bool backGroundScale9Enable = DICTOOL->getBooleanValue_json(options, "backGroundScale9Enable");
    panel->setBackGroundImageScale9Enable(backGroundScale9Enable);
    int cr = DICTOOL->getIntValue_json(options, "colorR");
    int cg = DICTOOL->getIntValue_json(options, "colorG");
    int cb = DICTOOL->getIntValue_json(options, "colorB");
    int co = DICTOOL->getIntValue_json(options, "colorO");
    float w = DICTOOL->getFloatValue_json(options, "width");
    float h = DICTOOL->getFloatValue_json(options, "height");
    if (co == 0)
    {
        co = 255;
    }
    panel->setColor(cocos2d::ccc3(cr, cg, cb));
    panel->setSize(CCSizeMake(w, h));

	std::string tp_b = m_strFilePath;
	const char* imageFileName = DICTOOL->getStringValue_json(options, "backGroundImage");
    const char* imageFileName_tp = imageFileName?tp_b.append(imageFileName).c_str():NULL;

    if (backGroundScale9Enable) {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
		if (widget->getUseMergedTexture())
		{
			panel->setBackGroundImageScale9(imageFileName, cocos2d::CCRect(cx, cy, cw, ch),true);
		}
		else
		{
            panel->setBackGroundImageScale9(imageFileName_tp, cocos2d::CCRect(cx, cy, cw, ch));
		}
    }
    else
    {

		if (widget->getUseMergedTexture())
		{
			panel->setBackGroundImage(imageFileName,true);
		}
		else
		{
			panel->setBackGroundImage(imageFileName_tp);
		}
    }
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForScrollViewFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForPanelFromJsonDictionary(widget, options);
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForSliderFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UISlider* slider = (UISlider*)widget;
    
    bool barTextureScale9Enable = DICTOOL->getBooleanValue_json(options, "barTextureScale9Enable");
    slider->setBarTextureScale9Enable(barTextureScale9Enable);
    bool bt = DICTOOL->checkObjectExist_json(options, "barFileName");
    float barLength = DICTOOL->getFloatValue_json(options, "length");
    if (bt)
    {
        if (barTextureScale9Enable)
        {
			std::string tp_b = m_strFilePath;
			const char*imageFileName =  DICTOOL->getStringValue_json(options, "barFileName");
            const char* imageFileName_tp = imageFileName?tp_b.append(imageFileName).c_str():NULL;
			if (widget->getUseMergedTexture())
			{
				 slider->setBarTextureScale9(imageFileName, 0, 0, 0, 0,true);
			}
			else
			{
				 slider->setBarTextureScale9(imageFileName_tp, 0, 0, 0, 0);
			}           
            slider->setBarLength(barLength);
        }
        else
        {
			std::string tp_b = m_strFilePath;
			const char*imageFileName =  DICTOOL->getStringValue_json(options, "barFileName");
            const char* imageFileName_tp = imageFileName?tp_b.append(imageFileName).c_str():NULL;
			if (widget->getUseMergedTexture())
			{
				slider->setBarTexture(imageFileName,true);
			}
			else
			{
				slider->setBarTexture(imageFileName_tp);
			}
        }
    }
	std::string tp_n = m_strFilePath;
	std::string tp_p = m_strFilePath;
	std::string tp_d = m_strFilePath;

	const char* normalFileName = DICTOOL->getStringValue_json(options, "ballNormal");
	const char* pressedFileName = DICTOOL->getStringValue_json(options, "ballPressed");
	const char* disabledFileName = DICTOOL->getStringValue_json(options, "ballDisabled");

	const char* normalFileName_tp = normalFileName?tp_n.append(normalFileName).c_str():NULL;
	const char* pressedFileName_tp = pressedFileName?tp_p.append(pressedFileName).c_str():NULL;
	const char* disabledFileName_tp =  disabledFileName?tp_d.append(disabledFileName).c_str():NULL;
	if (widget->getUseMergedTexture())
	{
		slider->setSlidBallTextures(normalFileName,pressedFileName,disabledFileName,true);
	}
	else
	{
		slider->setSlidBallTextures(normalFileName_tp,pressedFileName_tp,disabledFileName_tp);
	}
    slider->setSlidBallPercent(DICTOOL->getIntValue_json(options, "percent"));
    bool showProgressBarExist = DICTOOL->checkObjectExist_json(options, "showProgressBar");
    bool showProgressBar = false;
    if (showProgressBarExist)
    {
        showProgressBar = DICTOOL->getBooleanValue_json(options, "showProgressBar");
    }
    if (showProgressBar)
    {
        slider->setShowProgressBar(showProgressBar);
		std::string tp_b = m_strFilePath;
		const char*imageFileName =  DICTOOL->getStringValue_json(options, "progressBarFileName");
        const char* imageFileName_tp = imageFileName?tp_b.append(imageFileName).c_str():NULL;
		if (widget->getUseMergedTexture())
		{
			slider->setProgressBarTextureScale9(imageFileName, 0, 0, 0, 0, true);
		}
		else
		{
			slider->setProgressBarTextureScale9(imageFileName_tp, 0, 0, 0, 0);
		}
        slider->setProgressBarScale(barLength);
    }
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForTextAreaFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UITextArea* textArea = (UITextArea*)widget;
    textArea->setText(DICTOOL->getStringValue_json(options, "text"));
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        textArea->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    int cr = DICTOOL->getIntValue_json(options, "colorR");
    int cg = DICTOOL->getIntValue_json(options, "colorG");
    int cb = DICTOOL->getIntValue_json(options, "colorB");
    textArea->setTextColor(cr, cg, cb);
    textArea->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    bool aw = DICTOOL->checkObjectExist_json(options, "areaWidth");
    bool ah = DICTOOL->checkObjectExist_json(options, "areaHeight");
    if (aw && ah)
    {
        CCSize size = CCSize(DICTOOL->getFloatValue_json(options, "areaWidth"),DICTOOL->getFloatValue_json(options,"areaHeight"));
        textArea->setTextAreaSize(size);
    }
    bool ha = DICTOOL->checkObjectExist_json(options, "hAlignment");
    if (ha)
    {
        textArea->setTextHorizontalAlignment((cocos2d::CCTextAlignment)DICTOOL->getIntValue_json(options, "hAlignment"));
    }
    bool va = DICTOOL->checkObjectExist_json(options, "vAlignment");
    if (va)
    {
        textArea->setTextVerticalAlignment((cocos2d::CCVerticalTextAlignment)DICTOOL->getIntValue_json(options, "vAlignment"));
    }
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForTextButtonFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForButtonFromJsonDictionary(widget, options);
    UITextButton* textButton = (UITextButton*)widget;
    textButton->setText(DICTOOL->getStringValue_json(options, "text"));
    textButton->setFlipX(DICTOOL->getBooleanValue_json(options, "flipX"));
    textButton->setFlipY(DICTOOL->getBooleanValue_json(options, "flipY"));
    int cr = DICTOOL->getIntValue_json(options, "textColorR");
    int cg = DICTOOL->getIntValue_json(options, "textColorG");
    int cb = DICTOOL->getIntValue_json(options, "textColorB");
    textButton->setTextColor(cr,cg,cb);
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        textButton->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        textButton->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForTextFieldFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UITextField* textField = (UITextField*)widget;
    bool ph = DICTOOL->checkObjectExist_json(options, "placeHolder");
    if (ph)
    {
        textField->setPlaceHolder(DICTOOL->getStringValue_json(options, "placeHolder"));
    }
    textField->setText(DICTOOL->getStringValue_json(options, "text"));
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        textField->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool tsw = DICTOOL->checkObjectExist_json(options, "touchSizeWidth");
    bool tsh = DICTOOL->checkObjectExist_json(options, "touchSizeHeight");
    if (tsw && tsh)
    {
        textField->setTouchSize(DICTOOL->getFloatValue_json(options, "touchSizeWidth"), DICTOOL->getFloatValue_json(options,"touchSizeHeight"));
    }
    
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForLoadingBarFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary *options)
{
    this->setPropsForWidgetFromJsonDictionary(widget, options);
    UILoadingBar* loadingBar = (UILoadingBar*)widget;

	std::string tp_b = m_strFilePath;
	const char*imageFileName =  DICTOOL->getStringValue_json(options, "texture");
    const char* imageFileName_tp = imageFileName?tp_b.append(imageFileName).c_str():NULL;
	if (widget->getUseMergedTexture())
	{
		loadingBar->setTexture(imageFileName,true);
	}
	else
	{
		loadingBar->setTexture(imageFileName_tp);
	}
    loadingBar->setDirection(LoadingBarType(DICTOOL->getIntValue_json(options, "direction")));
    loadingBar->setPercent(DICTOOL->getIntValue_json(options, "percent"));
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void CCSReader::setPropsForListViewFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary *options)
{
    setPropsForScrollViewFromJsonDictionary(widget, options);
}

void CCSReader::setPropsForPageViewFromJsonDictionary(UIWidget*widget,cs::CSJsonDictionary* options)
{
    this->setPropsForPanelFromJsonDictionary(widget, options);
    this->setColorPropsForWidgetFromJsonDictionary(widget,options);
}

NS_CC_EXT_END