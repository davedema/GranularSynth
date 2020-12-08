/*
  ==============================================================================

    FreqTimeDrawable.cpp
    Created: 30 Oct 2020 6:21:22pm
    Author:  amere

  ==============================================================================
*/

#include "FreqTimeDrawable.h"

FreqTimeDrawable::FreqTimeDrawable()
{
    setSize(200, 150);
    setFramesPerSecond(60);
    this->drawnPoints = nullptr;
    this->model = nullptr;

}

FreqTimeDrawable::~FreqTimeDrawable()
{
}

void FreqTimeDrawable::paint(Graphics&g)
{
    g.fillAll(Colour(ColourPalette::drawbox));
    Path path;
    this->drawAxis(g);



    if (drawnPoints->size() != 0) {
        path.startNewSubPath(drawnPoints->getFirst().getX() * (float)getWidth(),
                             drawnPoints->getFirst().getY() * (float)getHeight());

        for (auto p : *drawnPoints)
        {
            path.lineTo(p.getX() * (float)getWidth(), p.getY() * (float)getHeight());
        }
        g.setColour(Colour(ColourPalette::numbers));
        g.strokePath(path, PathStrokeType(3.5f));
    }
    g.setFont(10);

    g.setColour(Colour(ColourPalette::bright_component));
    g.drawText("Freq. shift [Hz]", getWidth() / 2 - 3, 5, 100, 6, Justification::centred, false);
    g.drawText("Time [ms]", getWidth() - 85, getHeight() / 2 + 8, 100, 6, juce::Justification::centred, false);

    g.drawLine(getWidth() / 2 - 3, 5, getWidth() / 2 + 3, 5); //2k tick
    g.drawText("+2k", getWidth() / 2 - 25, 5, 18, 5, juce::Justification::centred, false);
    
    g.drawLine(getWidth() / 2 - 3, getHeight() - 5, getWidth() / 2 + 3, getHeight() - 5); // -2k tick
    g.drawText("-2k", getWidth() / 2 - 25, getHeight() - 10, 18, 5, juce::Justification::centred, false);


    g.drawHorizontalLine(5, 3, 3);

    enlightPoint(model->getCurrentxyPosition(), g);

}

void FreqTimeDrawable::resized()
{
}

void FreqTimeDrawable::update()
{
    repaint();
}

void FreqTimeDrawable::mouseDown(const MouseEvent& event)
{
    drawnPoints->clear();
}

void FreqTimeDrawable::mouseDrag(const MouseEvent& event)
{
    drawnPoints->add(Point<float>(event.x / (float)getWidth(), event.y / (float)getHeight()));
}

void FreqTimeDrawable::setModel(Model* model)
{
    this -> model = model;
    this->drawnPoints = model->getxyPlane();
}

void FreqTimeDrawable::enlightPoint(Point<float> point, Graphics& g)
{
    g.setColour(Colours::white);
    g.drawEllipse(point.getX() * getWidth(), point.getY() * getHeight(), 4, 4, 2);
}

void FreqTimeDrawable::drawAxis(Graphics&g)
{
    auto margin = 5.0f;
    Line<float> freq_ax(Point<float>(getWidth() / 2, (float)getHeight() - margin),
        Point<float>((float)getWidth() / 2, margin));

    Line<float> time_ax(Point<float>(margin, (float)getHeight() / 2 ),
        Point<float>((float)getWidth() - margin, (float)getHeight()/2));
    g.setColour(Colour(ColourPalette::bright_component));

    g.drawLine(freq_ax);
    g.drawLine(time_ax);

}

