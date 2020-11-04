# GranularSynth
## Sound Analysis Synthesis and Processing Project

This software is a digital granular synthesizer implemented by our group to achieve independent time-stretching 
and frequency shifting using the [JUCE 
framework](https://juce.com/) and an Hilbert transform implementation took from 
[this software developed by the Harvard-Smithsonian center for astrophysics](https://www.cfa.harvard.edu/~spaine/am/). 



















**"/Smithsonians_Discrete_Fourier_Hartley_transform/am_sysdep.h"** from the authors: 

* For optimum performance, the default processor data cache size
 settings here can be overridden by target-specific definitions
 supplied at compile time.
 
 * Line-by-line and CIA computations are blocked to fit in L1
  cache.  If the cache size is set to 0, cache blocking is
  turned off.
 
  * For FFTs and FHTs, the L1 cache size setting controls the
  point at which the computation switches over from recursive
  to iterative.
