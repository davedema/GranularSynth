# GranularSynth
Sound Analysis Synthesis and Processing Project

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
