/*
  ==============================================================================

    SequenceStrategy.h
    Created: 15 Sep 2020 2:20:49pm
    Author:  jacop

  ==============================================================================

  Provides an interface allowing the Scheduler to determine when the next grain should occur andwhat its duration should be

*/

#pragma once

class SequenceStrategy {


private:



public:

    int nextDuration(); //TODO: check if int or float (depending if samples or time)
    int nextInterOnset();

};