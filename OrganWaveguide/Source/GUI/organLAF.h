/*
 ; Look and Feel for Organ Waveguide
 ; Started from Foleys Look and Feel and uses parts of Jatin's code
 ; https://github.com/Chowdhury-DSP/chowdsp_utils/tree/59d5d29ba085c99a0b26304131ede9661d0de770
 ;  Champ Darabundit 05/31
 */

#include <JuceHeader.h>
#pragma once


class organLAF : public juce::LookAndFeel_V4
{
public:
    organLAF();
    
    juce::Typeface::Ptr getTypefaceForFont (const juce::Font&) override;
    
    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;
    
    //==============================================================================
    
    void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox&) override;
    
    void positionComboBoxText (juce::ComboBox&, juce::Label& labelToPosition) override;
    
    //==============================================================================
    
    void drawTabButton (juce::TabBarButton&, juce::Graphics&, bool isMouseOver, bool isMouseDown) override;
protected:
    // Get our knob image
    std::unique_ptr<juce::Drawable> knob = juce::Drawable::createFromImageData (BinaryData::knob_white_svg, BinaryData::knob_white_svgSize);
    // Load our typeface
    juce::Typeface::Ptr sarabunLight;
    juce::Typeface::Ptr sarabunReg;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (organLAF)
};
