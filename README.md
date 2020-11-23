<h1 align="center">GRANULATOR</h1>

<h2 align="center"> An application to play with Granular Synthesis </h2>

<p>img del plugin</p>

## What is Granular Synthesis

### Brief history
The most common definition of sound is related to the concept of wave propagating in space and time. With the emerging developments in the electro-acoustic technologies raised in the XX century, the British physicist Dennis Gabor published in the late 40s three important papers, where he combines theories from quantum theory with practical experiment. 
His idea is that any continuous tone can be decomposed into a family of functions obtained by shifting in time and frequency a single acoustic grain. This theory was strongly affected by psychoacoustics since human hearing is not continuous and infinite in resolution.
So, from a representation of a signal in time domain, s(t), and from one in frequency domain S(f) derived from Fourier analysis, he created a mathematical abstraction able to map a signal and its energy onto an “effective time and effective spectral width”. Gabor’s analysis can be also thought as a series of localized Fourier transforms.[4]
This technique was born with the publishment by Dr. Ing. D. Gabor of the article "*Theory of communication*" [[1]] in  which, by the use of the [Hilbert Transform](https://en.wikipedia.org/wiki/Hilbert_transform) and its meaning in the Quantum Mechanichal Formalism,  he derived a new two-dimensional representation of signals with time and frequency as coordinates which he called "diagram of information".
Gabor noticed that any analysis requiring signal windowing (such as STFT), entails a strong relationship between time and frequency. High resolution in frequency requires long time windows, at cost of losing track of when exactly some event occurred. Vice versa, having high temporal resolution (short windows), implies losing frequency precision. This strong connection establish a sort of "uncertainty principle", setting the limits of our comprehension of acoustic quantas.

## The application
Our goal was to achieve sound granulation, starting from a sample, with independent time-stretching  and frequency shifting using the [JUCE framework](https://juce.com/) and an Hilbert transform implementation taken from  [this software](https://www.cfa.harvard.edu/~spaine/am/)  developed by the Harvard-Smithsonian center for astrophysics. 

### Some math
Our approach to Gabor's brilliant article and our interpretation of frequency shifting with a parallelism to the quantum mechanical formalism ...
( COSE PAZZE JACO)
## Software implementation

### Architecture
The project is built around the JUCE framework and its core library. 
We created some custom classes to work in an object-oriented paradigm. 
The most important ones are:
- **FileLoader**: file loader class manages the sample, storing it into an audio buffer. Also a thumbnail is created to plot the waveform. At load, we compute the Hilbert transform to later processing and pitch shifting.
- **Grain**: each grain instance is able to manage itself. The grain knows where it starts in the audio buffer, how much will it last, which kind of envelope will be applied to it and is able to delete itself once its life-cycle is over.
- **GrainEnvelope**: class to manage different types of envelopes. Each grain can access to its methods to get the current value of the envelope to apply at each sample.
- **SequenceStrategy**: this class manages the time scheduling of grains, based on user-driven parameters. 
- **Granulator**: this class is the core of the application. It contains a list of active grains. It cycles through all the active grains and writes directly onto the output audio buffer. When a grain is over, it is removed from the stack. The next on-set depends on the grain density defined by the user. It is decremented at each cycles and when it reaches zero, it means that the next grain should be added to the stack and played. 
- **Model** : model class created to implement a Model-View-Control pattern. The state of the plugin parameters is stored here, and all the other classes can access to it in order to retrieve information about the different controls.

### File controls
The user can upload a sample, and it will be used as a wavetable to be granulized.
Also the starting position and the size of the section can be controlled, so that the granulator will reproduce cyclically through the selected area. 
Also a master gain control is provided.
### Grain parameters 
The choice of grain parameters is fundamental to achieve different sonic behaviour of our grain cloud.

- **Grain density**: represents how many grains are generated each second. This parameter must be treated carefully since a great overlap of grains will result in some unpredictable clippings. Low values of density will create rhythmic patterns since the distance between successive grains becomes greater.
- **Grain size**: typical grain durations are between 5ms and 100ms. Just around 50ms we start hearing some stable pitch-like behaviour, while shorter segmenets will result in impulsive/noise-like sounds. 
Notice that the spectral bandwidth of the grain is inversely proportional its duration.
- **Envelope shape**: we created three different possible envelopes. The envelope width can be adjusted through the width slider.
   - The Gaussian envelope, as originally thought by Gabor to mathematically model its acoustic quantas. 
   - The raised-cosine envelope.
   - Trapezoidal envelope.

An interesting aspect related to grain enveloping is that they contribute to an AM effect [4].
### Grain playback

### Spectral Output
Extractor class takes a portion of the output buffer and everytime it has been filled an asynchronous call is made in order to plot the spectrum. 
The FFT size is of 2048 samples. Then it is processed into 256 bins for representation purposes.
## Time-Frequency plane
This section is a 2-D canvas where the user can draw. Clicking will enable the control and the user can decide when a grain will be played and how much it will be shifted in frequency. 
The horizontal axis represents the position in the active section, while the y axis will decide the amount of shift (in Hz).
## References

[[1]] Theory of Communication, D.Gabor, 

[[2]] Generation and Combination of Grains for Music Synthesis,
Douglas L. Jones and Thomas W. Parks

[[3]] Spectral Granular Synthesis, Stefano Fasciani

[4] Microsound, Curtis Roads, MIT Press, 2002

[1]: http://www.granularsynthesis.com/pdf/gabor.pdf 
[2]: https://www.jstor.org/stable/3679939?seq=1
[3]: https://www.researchgate.net/publication/326316857_Spectral_Granular_Synthesis

## Notes
This project was developed for the course of Sound Analysis Synthesis and Processing (SASP) held by Professor Augusto Sarti at the Politecnico of Milan.
