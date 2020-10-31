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
    g.fillAll(Colours::darkred);
    Path path;

    if (drawnPoints->size() != 0) {
        path.startNewSubPath(drawnPoints->getFirst().getX() * (float)getWidth(),
                             drawnPoints->getFirst().getY() * (float)getHeight());

        for (auto p : *drawnPoints)
        {
            path.lineTo(p.getX() * (float)getWidth(), p.getY() * (float)getHeight());
        }
        g.strokePath(path, PathStrokeType(3.0f));
    }

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
    g.setColour(Colours::aliceblue);
    g.drawEllipse(point.getX() * getWidth(), point.getY() * getHeight(), 3, 3, 1);
}
