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
        horizontalLines.add((float)i / (float)10);
    }
    

    
    std::vector<int> verticalLinePos;
    for (int j = 1; j < 4; ++j) {
        for (int i = 1; i < 10; ++i) {
            verticalLinePos.push_back(pow(10, j) * i);
     
        }
    }
    verticalLinePos.push_back(pow(10, 4));
    verticalLinePos.push_back(pow(10, 4) * 2);
    for (auto vLine : verticalLinePos) {
        verticalLines.add((std::log10(vLine) -1 ) * (getLocalBounds().getWidth() * 1.8 / 3.3f) + 5);

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
        auto height = getLocalBounds().getHeight() - 2*margin;
        auto width = getLocalBounds().getWidth() - 2*margin;

        Path myPath;
        myPath.startNewSubPath(margin, getHeight() - 2*margin);
        int size = 256;
        for (int i = 0; i < size - 1; ++i)
        {
            float freqToX1 = (std::log10(*(freqBins + i)) - 1) * (width / 3.3f);
            float freqToX2 = (std::log10(*(freqBins + i + 1)) - 1) * (width / 3.3f);
            myPath.quadraticTo(Point<float>(
                (float)freqToX1, 
                jmap(*(currentFrame + i), 0.0f, 1.0f, (float)height, 0.0f)),

                Point<float>(
                    (float)freqToX2,
                    jmap(*(currentFrame + i + 1), 0.0f, 1.0f, (float)height, 0.0f)));
        }
        myPath.lineTo(width, height);
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
        auto freqLine = (std::log10(this->averageFrequency) - 1) * (200 * 1.8 / 3.3f + margin );
        g.drawLine(freqLine, 0, freqLine, getHeight() - margin, 0.3);   
    }  
    

}

void SpectrumDrawable::resized()
{
}

void SpectrumDrawable::drawNextFrame(float* bins, float* freqBins, float measuredShift, float resolution, float averageFreq, float sampleRate)
{
    currentFrame = bins;
    this->freqBins = freqBins;
    this->measuredShift = measuredShift;
    this->averageFrequency = averageFreq;
    this->resolution = resolution;
    this->sampleRate = sampleRate;

    repaint();
}

 
void SpectrumDrawable::drawGrid(Graphics& g)
{
    auto margin = 5.0f;
  Line<float> freq_ax(Point<float>(margin, (float)getHeight() - 2*margin),
                        Point<float>((float)getWidth() - margin, (float)getHeight() - 2*margin));

  Line<float> mag_ax(Point<float>(margin, (float)getHeight() - 2*margin),
      Point<float>(margin, margin));
    g.setColour(Colour(ColourPalette::bright_component));
    g.drawLine(freq_ax); 
    g.drawLine(mag_ax);
    

    for (auto line : verticalLines) {

        g.drawLine(line, 0, line, getHeight()-2*margin, 0.3);
    }

    for (auto line : horizontalLines)
    {
        g.drawLine(margin, line * getHeight(), getWidth() - margin, line * getHeight(), 0.3);
    }

    g.setFont(10);
    g.drawText("100", getWidth() / 3 - 4*margin, getHeight() - 2*margin + 3, 20, 6, Justification::left, false);
    g.drawText("1k", getWidth() / 2 + 6*margin, getHeight() - 2 * margin + 3, 20, 6, Justification::left, false);
    g.drawText("10k", getWidth() - 55, getHeight() - 2 * margin + 3, 20, 6, Justification::left, false);


}