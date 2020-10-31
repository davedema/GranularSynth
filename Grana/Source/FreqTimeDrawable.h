/*
  ==============================================================================

    FreqTimeDrawable.h
    Created: 30 Oct 2020 6:21:22pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "./Model.h"

class FreqTimeDrawable : public AnimatedAppComponent, public MouseListener {
public:

    FreqTimeDrawable();
    ~FreqTimeDrawable();

    void paint(Graphics&) override;
    void resized() override;
    void update() override;

    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;

    void setModel(Model* model);


private:
    Model* model;
    void addPoint(float xpos, float ypos);
    Array<Point<float>>* drawnPoints;
    Point<float> currentxyPlanePosition;
    void enlightPoint(Point<float> point, Graphics& g);
};