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
        path.startNewSubPath(drawnPoints->getFirst());

        for (auto p : *drawnPoints)
        {
            path.lineTo(p);
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
    drawnPoints->add(Point<float>(event.x, event.y));
}

void FreqTimeDrawable::mouseDrag(const MouseEvent& event)
{
    drawnPoints->add(Point<float>(event.x, event.y));
    repaint();

}

void FreqTimeDrawable::setModel(Model* model)
{
    this -> model = model;
    this->drawnPoints = model->getxyPlane();
}
