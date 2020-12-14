<h1 align="center">GRANULATOR</h1>

<p align="center"> A Granular Synthesis playground <p>

<p align="center">
   <img src="https://user-images.githubusercontent.com/37587013/101606994-57aa7080-3a04-11eb-8d76-98abef3e62f6.png" width=75%">
</p>

## What is Granular Synthesis

### Brief history
The most common definition of sound is related to the concept of wave propagating in space and time. With the emerging developments in the electro-acoustic technologies raised in the XX century, the British physicist Dennis Gabor published in the late 40s three important papers, where he combined theory from quantum mechanics with practical experiment. 
His idea is that any continuous tone can be decomposed into a family of functions obtained by shifting in time and frequency a single acoustic grain. This theory, strongly affected by psychoacoustics since human hearing is not continuous and infinite in resolution,
was born with the publishment by Dr. Ing. D. Gabor of the article "*Theory of communication*" [[1]] in  which, by the use of the [Hilbert Transform](https://en.wikipedia.org/wiki/Hilbert_transform) and its meaning in the Quantum Mechanical Formalism,  he derived a new two-dimensional representation of signals with time and frequency as coordinates which he called "diagram of information": from a complex representation of a signal in time domain, the Hilbert transform of the original signal, and from one in frequency domain derived from Fourier analysis of the Hilbert transform, he created a mathematical abstraction able to map a signal and its energy onto an “effective time and effective spectral width”, calculated as the mean time and frequency of a signal as intended in quantum mechanics, i.e. the value of the operator averaged over the wave function of the system. 
This is where the [Hilbert Transform](https://en.wikipedia.org/wiki/Hilbert_transform) comes in handy, giving us the possibility to reconstruct the (collapsed) wave function from the real signal and use it to describe signals with the quantum mechanical formalism and also, as we will see, the possibility of applying quantum mechanical transformations to the signal.

Gabor’s analysis can be also thought as a series of localized Fourier transforms.[5]

Gabor also noticed that any analysis requiring signal windowing (such as STFT), entails a strong relationship between time and frequency. High resolution in frequency requires long time windows, at cost of losing track of when exactly some event occurred. Vice versa, having high temporal resolution (short windows), implies losing frequency precision. This strong connection establish a sort of "uncertainty principle", setting the limits of our comprehension of acoustic quanta, and enforcing once again the quantum mechanichal parallelism in his theory. 

### Granulation
The main technique on which granular synthesis is based on is granulation, a process in which an audio file is broken down into tiny segments of audio, usually lasting from 5 to 100 ms [[5]]. The sequence of grains in chronological order is called the “graintable”; if grains are played in this order at the speed of the original sample, the output will play back that original file, if one makes sure that amplitude envelopes of overlapping grains sum up to one. If grains are instead played back in a different way (for example by skipping or repeating some of them, by changing their order or applying a frequency shift) different and particular timbres start to emerge.

Because of its eclectic nature, granular synthesis is best used as a textural technique, often for drones, pads, natural sounds and interesting noise layer.

### The formalism

Gabor's ideas connect the quantum mechanichal formalism with the signal theory world. More specifically, all the concepts deriving from the usage of quadrature signals
find their explanation in the quantum world parallelism. 

A very well done introductory article about quadrature signals is found in [[3]]; what we will particularly see in this section is how frequency shifting done via a Hilbert Transformer [[4]] is nothing else than a representation of the quantum mechanichal frequency shift operator and consequently wonder about the families of quantum mechanichal transformations that can be achieved using Hilbert transformers.

Also, we will give a more straightforward and mathematical interpretation of the frequency shifting operation we're doing, in order to tackle this concept from more viewpoints.

To be clear about the meaning of what we're about to say, we want to stress here that with Hilbert transform we intend the complex sum of the real signal along the real axis with its quadrature one along the complex axis H(t) = s(t) + j * a(t) with a(t) being the quadrature signal of s(t). We will not care about if the quadrature signal is the +90 degrees or -90 degrees phase shift version of the original one, being the formalism about taking the real part of the complex signal, operation invariant for complex conjugation.

Starting from the last interpretation mentioned, we understand from [[3]] how the Hilbert transform represents a complex vector evolving in time in such a way that its real part at any moment represents the real signal at that moment. The following figure represents the Hilbert transform of a sinusoid:

<p align="center">
   <img src="https://www.dsprelated.com/blogimages/RickLyons/Quad_Signals_Fig6.JPG" width=75% alt="Hilbert transform of a sinusoidal signal">   
</p>

as we can see, this vector rotates in the complex plane as time passes. On to the frequency shifting operation, we understand from [[4]] that the shifted signal is obtained multiplying the Hilbert transform with exp(j * w * t) with **w** being the desired shift and **t** the time passed (mod(2 * PI) to handle phase). Mathematically we know from complex numbers theory that multiplying a complex number with another complex number of unitary module we obtain a rotation of the original number by the phase of the unitary vector or, equivalently, we could say that we are describing the complex number in a reference frame rotated by the phase of the unitary number. So we can now understand better what happens whrn we multiply the Hilbert transform with exp(j * w * t): we can think of this operation as if we are describing the Hilbert transform in a reference frame with the time axis coincident with the one of the previous figure, but with now a complex plane **rotating** along the time axis with angular velocity **w**. So, the period of the Hilbert Transform will consequently be lengthened or shortened, depending on the sign of **w**, by **w** and consequently the frequency (1 / period) will be modified, resulting in a frequency shift of the Hilbert transform that will of course translate in a frequency shift of its real part, our original signal.

Now that we understand mathematically what is happening, we can move forward and take a look at this operation from a quantum mechanichal point of view. We will assume background knowledge of quantum mechanics and of theory of groups and representations in this explanation, we otherwise suggest to skip to the **"The application"** section. Our knowledge comes from one of our members which is very happy to be able to suggest references [7] and [8] (any textbook on quantum mechanichs/group theory should anyway be fine). 

## The application
The application is built in C++ using the [JUCE framework](https://juce.com/) and an Hilbert transform implementation taken from  [this software](https://www.cfa.harvard.edu/~spaine/am/)  developed by the Harvard-Smithsonian center for astrophysics. It can be used both as a VST3 in a DAW or as a standalone program.  
The goal is to provide a useful and relatively simple tool to play with granular synthesis and understand it better, along with easily experiencing independent time-stretching and frequency shifting.

In this section all the controls are explained one by one.

### Source file
<img src="https://user-images.githubusercontent.com/37587013/101624208-e83f7b80-3a19-11eb-8a17-7e2e7757d77f.png" width=80%>

1. The audio file can be loaded by pressing the "LOAD" button or by directly dropping it into the screen. Once the file is loaded the waveform is displayed.
2. Play/Stop the reproduction.
3. Controls the master volume.
4. Change the starting position of the granulated section.
5. Change the duration of the granulated section. Once the end of the section is reached the reproduction will loop back to the starting position.

### Envelope
<img src="https://user-images.githubusercontent.com/37587013/101626476-52a5eb00-3a1d-11eb-8858-4e0f605349f0.png" width=40%>

As envelope is intended the window applied to each grain in the playback phase.
1. Display the envelope.
2. Select the type of envelope between Gaussian, Trapezoidal and Raised Cosine Bell.
3. Change the envelope lobe width.

### Parameters
<img src="https://user-images.githubusercontent.com/37587013/101626533-65202480-3a1d-11eb-86a4-af2e2b49a699.png" width=40%>

1. Controls how many grains are generated each second. The range goes from 2 to 200 grains per second.
2. Controls the duration of the grains. The range goes from 5 ms up to 100 ms.
3. Represents the probability of playing back a grain from a random position in a range controlled by the "Random Spread" parameter.
4. Represents the range in which the randomic grain can be selected.
5. Controls the playback speed. In order to obtain slower (or faster) reproduction the grains are created in order to repeat (or skip) certain sections of the audio file. 

### Time-Frequency Pad
<img src="https://user-images.githubusercontent.com/37587013/101624676-921f0800-3a1a-11eb-8d21-d0259a1d2767.png" width=40%>

This control is a 2-D canvas in which the user can draw a shape that will control the time (in the original audio file) at which the grains are selected to be played back and their frequency shift. In particular the horizontal axis represents the position of the grain to be selected in the active section, while the vertical axis represents the amount of frequency shift (in Hz).  
The control is enabled by starting to draw and can be disabled by simply clicking on it.

### Spectrum analyzer
<img src="https://user-images.githubusercontent.com/37587013/101624710-a236e780-3a1a-11eb-94d2-412d3e09943e.png" width=40%>

Display the spectrum of the output signal. The FFT size is of 2048 samples, then it is processed into 256 bins for representation purposes.

## Architecture
Block diagrams con spiegazione

### Pitch Shifting
Hilbert stuff

## References

[[1]] Theory of Communication, D.Gabor, 

[[2]] Generation and Combination of Grains for Music Synthesis,
Douglas L. Jones and Thomas W. Parks

[[3]] A quadrature signals tutorial: complex, but not complicated, Rick Lyons

[[4]] Phase or frequency shifting using a Hilbert transformer, Neil Robertson

[5] Microsound, Curtis Roads, MIT Press, 2002

[[6]] The Basics of Granular Synthesis, Griffin Brown

[7] Principles of Quantum Mechanics, Pauli A. M. Dirac

[8] Group theory and its applications to quantum mechanics of atomic spectra, Eugene P. Wigner

[1]: http://www.granularsynthesis.com/pdf/gabor.pdf 
[2]: https://www.jstor.org/stable/3679939?seq=1
[3]: https://www.dsprelated.com/showarticle/192.php
[4]: https://www.dsprelated.com/showarticle/1147.php
[6]: https://www.izotope.com/en/learn/the-basics-of-granular-synthesis.html

## Notes
This project was developed for the course of Sound Analysis Synthesis and Processing (SASP) held by Professor Augusto Sarti at the Politecnico of Milan.
