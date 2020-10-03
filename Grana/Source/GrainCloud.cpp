/*
  ==============================================================================

    GrainCloud.cpp
    Created: 22 Sep 2020 5:20:42pm
    Author:  jacop

  ==============================================================================
*/

#include "GrainCloud.h"

GrainCloud::GrainCloud()
{
}

GrainCloud::~GrainCloud()
{
    for (auto g : grains) {
        delete g;
    }
}

void GrainCloud::addGrain(Grain* g)
{
    grains.add(g);
}

void GrainCloud::deleteGrain(Grain* g)
{
    grains.remove(&g);
}
