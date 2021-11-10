
/*
 Look and Feel for Organ Waveguide
 Based off of Foleys template and Jatin's code
 https://github.com/Chowdhury-DSP/chowdsp_utils/tree/59d5d29ba085c99a0b26304131ede9661d0de770
 Champ Darabundit 05/31
 */

#include "organLAF.h"

// Init typefaces from Binary data
organLAF::organLAF()
{
    sarabunLight = juce::Typeface::createSystemTypefaceFor(BinaryData::SarabunLight_ttf, BinaryData::SarabunLight_ttfSize);
    sarabunReg = juce::Typeface::createSystemTypefaceFor(BinaryData::SarabunRegular_ttf, BinaryData::SarabunRegular_ttfSize);
}

// Return require typeface
juce::Typeface::Ptr organLAF::getTypefaceForFont (const juce::Font& font)
{
    return font.isBold() ? sarabunReg : sarabunLight;
}

// Main custom code
void organLAF::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                 const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    // Get diameter
    int diameter = (width > height) ? height: width;
    if (diameter < 16) // Don't try to draw
        return;
    
    // Get bounds
    juce::Point<float> centre (x + width * 0.5f, y + height * 0.5f);
    float radius = diameter * 0.5f;
    x = int (centre.x - radius);
    y = int (centre.y - radius);
    const auto bounds = juce::Rectangle<int> (x, y, diameter, diameter).toFloat();
    
    // Get SVG dimensions and apply transformation
    auto b = knob->getBounds().toFloat();
    knob->setTransform( juce::AffineTransform::rotation ( juce::MathConstants<float>::twoPi * ((sliderPos - 0.5f) * 300.0f / 360.0f), b.getCentreX(), b.getCentreY()));
    
    // Bounds needed to correctly draw our objects
    auto knobBounds = (bounds * 0.9).withCentre (centre);
    auto meterBounds = (bounds * 0.9 * 0.5).withCentre(centre);
    auto meterBackBounds = (bounds * 0.89).withCentre(centre);
    
    // Start and eng angle
    const auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    // Used to draw thick slider
    constexpr float arcFactor = 1.6f;
    
    // Draw the background of the slider arc, just a circle
    juce::Path valueArc;
    valueArc.addEllipse (meterBackBounds);
    g.setColour (slider.findColour (juce::Slider::trackColourId).withMultipliedAlpha (1.0));
    g.fillPath (valueArc);
    valueArc.clear();
    
    // Draw the level arc
    valueArc.addPieSegment (meterBounds, rotaryStartAngle, toAngle, arcFactor);
    g.setColour (slider.findColour (juce::Slider::thumbColourId).withMultipliedAlpha (1.0));
    g.fillPath (valueArc);
    
    // Draw a little dot to match the slider color
    auto dotW = radius * 0.28f;
    juce::Point<float> thumbPoint (centre.x + radius * 0.77 * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                                   centre.y + radius * 0.77 * std::sin (toAngle - juce::MathConstants<float>::halfPi));
    
    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillEllipse (juce::Rectangle<float> (dotW, dotW).withCentre (thumbPoint));
    
    // Finally draw the knob image to mask everything
    knob->drawWithin(g, knobBounds, juce::RectanglePlacement::stretchToFit, 1.0);
    
}

//==============================================================================

void organLAF::drawComboBox (juce::Graphics& g, int width, int height, bool,
                             int, int, int, int, juce::ComboBox& box)
{
    auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    juce::Rectangle<int> boxBounds (0, 0, width, height);
    
    g.setColour (box.findColour (juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);
    
    g.setColour (box.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);
}

void organLAF::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setBounds (1, 1,
                     box.getWidth() - 2,
                     box.getHeight() - 2);
    
    label.setFont (getComboBoxFont (box));
}

//==============================================================================

void organLAF::drawTabButton (juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown)
{
    const auto activeArea = button.getActiveArea();
    
    const auto o = button.getTabbedButtonBar().getOrientation();
    
    const auto bkg = button.getTabBackgroundColour();
    
    juce::Point<int> p1, p2;
    
    switch (o)
    {
        case juce::TabbedButtonBar::TabsAtBottom:   p1 = activeArea.getBottomLeft(); p2 = activeArea.getTopLeft();    break;
        case juce::TabbedButtonBar::TabsAtTop:      p1 = activeArea.getTopLeft();    p2 = activeArea.getBottomLeft(); break;
        case juce::TabbedButtonBar::TabsAtRight:    p1 = activeArea.getTopRight();   p2 = activeArea.getTopLeft();    break;
        case juce::TabbedButtonBar::TabsAtLeft:     p1 = activeArea.getTopLeft();    p2 = activeArea.getTopRight();   break;
        default:                              jassertfalse; break;
    }
    
    if (button.getToggleState())
    {
        g.setGradientFill (juce::ColourGradient (bkg.brighter (0.1f), p1.toFloat(),
                                                 bkg.darker   (0.2f), p2.toFloat(), false));
    }
    else
    {
        g.setGradientFill (juce::ColourGradient (bkg.darker (0.2f), p1.toFloat(),
                                                 bkg.darker (0.5f), p2.toFloat(), false));
    }
    
    g.fillRect (activeArea);
    
    g.setColour (button.findColour (juce::TabbedButtonBar::tabOutlineColourId));
    
    auto r = activeArea;
    
    if (o != juce::TabbedButtonBar::TabsAtBottom)   g.fillRect (r.removeFromTop (1));
    if (o != juce::TabbedButtonBar::TabsAtTop)      g.fillRect (r.removeFromBottom (1));
    if (o != juce::TabbedButtonBar::TabsAtRight)    g.fillRect (r.removeFromLeft (1));
    if (o != juce::TabbedButtonBar::TabsAtLeft)     g.fillRect (r.removeFromRight (1));
    
    const auto alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;
    
    auto col = bkg.contrasting().withMultipliedAlpha (alpha);
    
    if (juce::TabbedButtonBar* bar = button.findParentComponentOfClass<juce::TabbedButtonBar>())
    {
        juce::TabbedButtonBar::ColourIds colID = button.isFrontTab() ? juce::TabbedButtonBar::frontTextColourId
        : juce::TabbedButtonBar::tabTextColourId;
        
        if (bar->isColourSpecified (colID))
            col = bar->findColour (colID);
        else if (isColourSpecified (colID))
            col = findColour (colID);
    }
    
    const auto area = button.getTextArea().toFloat();
    
    float length = area.getWidth();
    float depth  = area.getHeight();
    
    if (button.getTabbedButtonBar().isVertical())
        std::swap (length, depth);
    
    juce::TextLayout textLayout;
    createTabTextLayout (button, length, depth, col, textLayout);
    
    juce::AffineTransform t;
    
    switch (o)
    {
        case juce::TabbedButtonBar::TabsAtLeft:   t = t.rotated (juce::MathConstants<float>::pi * -0.5f).translated (area.getX(), area.getBottom()); break;
        case juce::TabbedButtonBar::TabsAtRight:  t = t.rotated (juce::MathConstants<float>::pi *  0.5f).translated (area.getRight(), area.getY()); break;
        case juce::TabbedButtonBar::TabsAtTop:
        case juce::TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY()); break;
        default:                                  jassertfalse; break;
    }
    
    g.addTransform (t);
    textLayout.draw (g, juce::Rectangle<float> (length, depth));
}

