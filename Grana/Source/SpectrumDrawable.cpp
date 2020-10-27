/*
  ==============================================================================

    SpectrumDrawable.cpp
    Created: 26 Oct 2020 11:33:12pm
    Author:  amere

  ==============================================================================
*/

#include "SpectrumDrawable.h"

SpectrumDrawable::SpectrumDrawable(): forwardFFT(fftOrder), window(fftSize, dsp::WindowingFunction<float>::hamming)
{
}

SpectrumDrawable::~SpectrumDrawable()
{
}

void SpectrumDrawable::paint(Graphics&)
{
}

void SpectrumDrawable::resized()
{
}
