<h1 align="center">GRANULATOR</h1>

<h2 align="center"> A Granular Synthesis playground </h2>

<p>img del plugin</p>

## What is Granular Synthesis

### Brief history
The most common definition of sound is related to the concept of wave propagating in space and time. With the emerging developments in the electro-acoustic technologies raised in the XX century, the British physicist Dennis Gabor published in the late 40s three important papers, where he combined theory from quantum mechanics with practical experiment. 
His idea is that any continuous tone can be decomposed into a family of functions obtained by shifting in time and frequency a single acoustic grain. This theory was strongly affected by psychoacoustics since human hearing is not continuous and infinite in resolution.
This technique was born with the publishment by Dr. Ing. D. Gabor of the article "*Theory of communication*" [[1]] in  which, by the use of the [Hilbert Transform](https://en.wikipedia.org/wiki/Hilbert_transform) and its meaning in the Quantum Mechanichal Formalism,  he derived a new two-dimensional representation of signals with time and frequency as coordinates which he called "diagram of information": from a representation of a signal in time domain, s(t), and from one in frequency domain S(f) derived from Fourier analysis, he created a mathematical abstraction able to map a signal and its energy onto an “effective time and effective spectral width”. Gabor’s analysis can be also thought as a series of localized Fourier transforms.[4]
Gabor noticed that any analysis requiring signal windowing (such as STFT), entails a strong relationship between time and frequency. High resolution in frequency requires long time windows, at cost of losing track of when exactly some event occurred. Vice versa, having high temporal resolution (short windows), implies losing frequency precision. This strong connection establish a sort of "uncertainty principle", setting the limits of our comprehension of acoustic quantas.

### Granulation
The main technique on which granular synthesis is based on is granulation, a process in which an audio file is broken down into tiny segments of audio, usually lasting from 5 to 100 ms [[4]]. The sequence of grains in chronological order is called the “graintable”; if grains are played in this order at the speed of the original sample, the output will play back that original file, if one makes sure that amplitude envelopes of overlapping grains sum up to one. If grains are instead played back in a different way (for example by skipping or repeating some of them, by changing their order or applying a frequency shift) different and particular timbres start to emerge.

Because of its eclectic nature, granular synthesis is best used as a textural technique, often for drones, pads, natural sounds and interesting noise layer.

## The application
The goal of our application is to provide a useful and relatively simple tool to play with granular synthesis and understand it better, along with providing a software able to 
make the user easily experience independent time-stretching and frequency shifting. 
In this section all the controls are explained one by one.

### File loading
-------------img con numerini-------------------------
1. The audio file can be loaded by pressing the "LOAD" button or by directly dropping it. Once the file is loaded the waveform is displayed on the right screen.
2. Play/Stop the reproduction
3. Controls the master volume
4. Change the starting position of the granulated section
5. Change the duration of the granulated section. Once the end of the section is reached the reproduction will loop back to the starting position

### Envelope
-------------img con numerini-------------------------

As envelope is intended the window applied to each grain in the playback phase.
1. Display the envelope
2. Select the type of envelope between Gaussian, Trapezoidal and Raised Cosine Bell
3. Change the envelope lobe width

### Parameters
-------------img con numerini-------------------------
1. Controls how many grains are generated each second. The range goes from 2 to 200 grains per second
2. Controls the duration of the grains. The range goes from 5 ms up to 100 ms
3. Represents the probability of playing back a grain from a random position in a range controlled by the "Random Spread" parameter
3. Represents the range in which the randomic grain can be selected

### Time-Frequency Pad
-------------img-------------------------

This control is a 2-D canvas in which the user can draw a shape that will control the time (in the original audio file) at which the grains are selected to be played back and their frequency shift. 
In particular the horizontal axis represents the position of the grain to be selected in the active section, while the vertical axis represents the amount of frequency shift (in Hz). The control is enabled by starting to draw and can be disabled by simply clicking on it.

### Spectrum analyzer
-------------img-------------------------

Display the spectrum of the output signal. The FFT size is of 2048 samples, then it is processed into 256 bins for representation purposes.

## Frequency shifting
( COSE PAZZE JACO)

## Software implementation
The application is built in C++ using the [JUCE framework](https://juce.com/) and an Hilbert transform implementation taken from  [this software](https://www.cfa.harvard.edu/~spaine/am/)  developed by the Harvard-Smithsonian center for astrophysics. 

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


[4] Microsound, Curtis Roads, MIT Press, 2002

[[5]] The Basics of Granular Synthesis, Griffin Brown

[1]: http://www.granularsynthesis.com/pdf/gabor.pdf 
[2]: https://www.jstor.org/stable/3679939?seq=1
[5]: https://www.izotope.com/en/learn/the-basics-of-granular-synthesis.html

## Notes
This project was developed for the course of Sound Analysis Synthesis and Processing (SASP) held by Professor Augusto Sarti at the Politecnico of Milan.
