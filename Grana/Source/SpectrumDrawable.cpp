/*
  ==============================================================================

    SpectrumDrawable.cpp
    Created: 26 Oct 2020 11:33:12pm
    Author:  amere

  ==============================================================================
*/

#include "SpectrumDrawable.h"

SpectrumDrawable::SpectrumDrawable()
{
    setSize(200, 100);
    currentFrame = nullptr;
    this->measuredShift = 0;
    this->resolution = 0;
    this->averageFrequency = 0;
    float skewx = 0.0f;
    for (int i = 0; i < 10; i++) {
       
        auto skewx =  -std::log10( (float)(i+1) * (float)10 / (float)100); // skew x-axis onto log scale
        horizontalLines.add((float)i / (float)10);
        verticalLines.add(skewx/3);
    }
}

SpectrumDrawable::~SpectrumDrawable()
{
}

void SpectrumDrawable::paint(Graphics& g)
{
    g.fillAll(Colour(ColourPalette::drawbox));
    this->drawGrid(g);

    g.setColour(Colour(ColourPalette::numbers));

    if (this->currentFrame != nullptr && averageFrequency != 0)
    {
        auto margin = 5;
        auto height = getLocalBounds().getHeight() - margin;
        auto width = getLocalBounds().getWidth() - margin;

        Path myPath;
        myPath.startNewSubPath(margin, getHeight() - margin);
        int size = 256;
        for (int i = 0; i < size / 2; ++i)
        {
            myPath.quadraticTo(Point<float>((float)jmap(2 * i, 0, size, margin, width - margin),
                jmap(*(currentFrame + 2 * i), 0.0f, 1.0f, (float)height - margin, 0.0f)),
                Point<float>((float)jmap(2 * i + 1, 0, size, margin, width - margin),
                    jmap(*(currentFrame + 2 * i + 1), 0.0f, 1.0f, (float)height - margin, 0.0f)));
        }
        myPath.lineTo(getWidth() - margin, getHeight() - margin);
        g.strokePath(myPath, PathStrokeType(3.0f));
        
        Colour base = Colour(ColourPalette::numbers);
        ColourGradient* gradient = new ColourGradient(Colour(ColourPalette::dark_component), 0, getHeight(), Colour(ColourPalette::numbers), 0, 0, false);
        gradient->clearColours();
        gradient->addColour(0, base.darker().darker());
        gradient->addColour(0.2, base.darker());
        gradient->addColour(0.4, base);
        gradient->addColour(0.7, base.brighter());

        g.setGradientFill(*gradient);
        g.setOpacity(0.7);
        g.fillPath(myPath);

        delete gradient;

        g.drawFittedText("Peak Shift: " + String(this->measuredShift) + "Hz" + //Print frequency data 
                         "\nOut Average Freq: " + String(this->averageFrequency) + "Hz" +
                         "\nResolution: " + String(this->resolution) + "Hz" ,
                         0, 10, getWidth(), 25, Justification::centred, 3); 
       
        g.setColour(Colours::yellow);
        auto prova = -std::log10((float)(1.0f - 2 * this->averageFrequency / (float)this->sampleRate)) * 10  ;
        g.drawVerticalLine(prova *getWidth(), margin, getHeight()-margin );
        
    }  
    

}

void SpectrumDrawable::resized()
{
}

void SpectrumDrawable::drawNextFrame(float* bins, float measuredShift, float resolution, float averageFreq, float sampleRate)
{
    currentFrame = bins;
    this->measuredShift = measuredShift;
    this->averageFrequency = averageFreq;
    this->resolution = resolution;
    this->sampleRate = sampleRate;

    repaint();
}

 
void SpectrumDrawable::drawGrid(Graphics& g)
{
    auto margin = 5.0f;
  Line<float> freq_ax(Point<float>(margin, (float)getHeight() - margin),
                        Point<float>((float)getWidth() - margin, (float)getHeight() - margin));

  Line<float> mag_ax(Point<float>(margin, (float)getHeight() - margin),
      Point<float>(margin, margin));
    g.setColour(Colour(ColourPalette::bright_component));
    g.drawLine(freq_ax); 
    g.drawLine(mag_ax);
    
  
    for (auto line : verticalLines) {
        g.drawLine((1.0f -line) * getWidth() - margin, margin, (1.0f - line) * getWidth() - margin, this->getHeight() - margin, 0.3);
        g.drawLine((1.0f - line - 0.33f) * getWidth() - margin, margin, (1.0f - line  - 0.33f) * getWidth() - margin, this->getHeight() - margin, 0.3);
        g.drawLine((1.0f - line - 0.66f) * getWidth() - margin, margin, (1.0f - line - 0.66f) * getWidth() - margin, this->getHeight() - margin, 0.3);

    }

    for (auto line : horizontalLines)
    {
        g.drawLine(margin, line * getHeight(), getWidth() - margin, line * getHeight(), 0.3);
    }

}