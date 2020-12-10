<h1 align="center">GRANULATOR</h1>

<p align="center"> A Granular Synthesis playground <p>

<p align="center">
   <img src="https://user-images.githubusercontent.com/37587013/101606994-57aa7080-3a04-11eb-8d76-98abef3e62f6.png" width=75%">
</p>

## What is Granular Synthesis

### Brief history
The most common definition of sound is related to the concept of wave propagating in space and time. With the emerging developments in the electro-acoustic technologies raised in the XX century, the British physicist Dennis Gabor published in the late 40s three important papers, where he combined theory from quantum mechanics with practical experiment. 
His idea is that any continuous tone can be decomposed into a family of functions obtained by shifting in time and frequency a single acoustic grain. This theory was strongly affected by psychoacoustics since human hearing is not continuous and infinite in resolution.
This technique was born with the publishment by Dr. Ing. D. Gabor of the article "*Theory of communication*" [[1]] in  which, by the use of the [Hilbert Transform](https://en.wikipedia.org/wiki/Hilbert_transform) and its meaning in the Quantum Mechanical Formalism,  he derived a new two-dimensional representation of signals with time and frequency as coordinates which he called "diagram of information": from a representation of a signal in time domain, s(t), and from one in frequency domain S(f) derived from Fourier analysis, he created a mathematical abstraction able to map a signal and its energy onto an “effective time and effective spectral width”. Gabor’s analysis can be also thought as a series of localized Fourier transforms.[4]
Gabor noticed that any analysis requiring signal windowing (such as STFT), entails a strong relationship between time and frequency. High resolution in frequency requires long time windows, at cost of losing track of when exactly some event occurred. Vice versa, having high temporal resolution (short windows), implies losing frequency precision. This strong connection establish a sort of "uncertainty principle", setting the limits of our comprehension of acoustic quantas.

### Granulation
The main technique on which granular synthesis is based on is granulation, a process in which an audio file is broken down into tiny segments of audio, usually lasting from 5 to 100 ms [[4]]. The sequence of grains in chronological order is called the “graintable”; if grains are played in this order at the speed of the original sample, the output will play back that original file, if one makes sure that amplitude envelopes of overlapping grains sum up to one. If grains are instead played back in a different way (for example by skipping or repeating some of them, by changing their order or applying a frequency shift) different and particular timbres start to emerge.

Because of its eclectic nature, granular synthesis is best used as a textural technique, often for drones, pads, natural sounds and interesting noise layer.

## The application
The application is built in C++ using the [JUCE framework](https://juce.com/) and can be used both as a VST3 in a DAW or as a standalone program.  
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


[4] Microsound, Curtis Roads, MIT Press, 2002

[[5]] The Basics of Granular Synthesis, Griffin Brown

[1]: http://www.granularsynthesis.com/pdf/gabor.pdf 
[2]: https://www.jstor.org/stable/3679939?seq=1
[5]: https://www.izotope.com/en/learn/the-basics-of-granular-synthesis.html

## Notes
This project was developed for the course of Sound Analysis Synthesis and Processing (SASP) held by Professor Augusto Sarti at the Politecnico of Milan.
