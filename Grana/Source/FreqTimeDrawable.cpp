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
}

void FreqTimeDrawable::resized()
{
}

void FreqTimeDrawable::mouseDown(const MouseEvent& event)
{
    DBG("down" << std::to_string(event.x) << "  " << std::to_string(event.y));
    drawnPoints->clear();
    repaint();

    drawnPoints->add(Point<float>(event.x / (float)getWidth(), event.y / (float)getHeight()));
}

void FreqTimeDrawable::mouseDrag(const MouseEvent& event)
{
    drawnPoints->add(Point<float>(event.x / (float)getWidth(), event.y / (float)getHeight()));
    repaint();

}

void FreqTimeDrawable::setModel(Model* model)
{
    this -> model = model;
    this->drawnPoints = model->getxyPlane();
}
