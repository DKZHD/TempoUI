#pragma once

// IWYU pragma: begin_keep
#include "Animation/Animation.h"
#include "AnimationManager/AnimationManager.h"

// Base Element
#include "Elements/ElementBase/ElementBase.h"

// Container Elements
#include "Elements/Canvas/Canvas.h"
#include "Elements/BorderedBox/BorderedBox.h"
#include "Elements/HorizontalBox/HorizontalBox.h"
#include "Elements/VerticalBox/VerticalBox.h"
#include "Elements/ScrollBox/ScrollBox.h"
#include "Elements/Wrapbox/Wrapbox.h"
#include "Elements/Combobox/Combobox.h"

// Interactive Elements
#include "Elements/Button/Button.h"
#include "Elements/Checkbox/Checkbox.h"
#include "Elements/Slider/Slider.h"
#include "Elements/ToggleSlider/ToggleSlider.h"
#include "Elements/TextArea/TextArea.h"
#include "Elements/InputBox/InputBox.h"
#include "Elements/ColorPicker/ColorPicker.h"

// Monitoring Elements
#include "Elements/ProgressBar/ProgressBar.h"
#include "Elements/GraphView/GraphView.h"

// Other Elements
#include "Elements/Image/Image.h"
#include "Elements/Text/Text.h"

#include "EventManager/EventManager.h"

#ifdef LAYOUT_LOADER
#include "LayoutLoader/LayoutLoader.h"
#endif

#ifdef NETWORK
#include "NetworkManager/NetworkManager.h"
#endif

#include "ThemeManager/ThemeManager.h"
#include "UIInputHandler/UIInputHandler.h"
#include "ImageHandler/ImageHandler.h"
#include "TextHandler/TextHandler.h"
#include "UIRenderer/UIRenderer.h"

// IWYU pragma: begin_end
