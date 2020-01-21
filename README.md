# acoustic-camera-playground
Some initial beamforming experiments - I HAVE NO EDUCATION ON THIS, JUST PLAYING AROUND

## Why?
To get a feeling for how different geometries behave,

## How?
I do a simulation in the opposite direction (i.e. feed all the microphones the same sinewave with the same phase), and
calculate the sound intensity at a position in space.

When the argument "--polar" is supplied, I generate a polar plot (in dB).

Without the "--polar" argument, I generate a rectangular image of relative intensities (not dB) across a 20 x 20 meter wall at a distance of 10 meters
(so the angle from the center to the sides of that image is 45 degrees)

## Technical details
Now the simulations are done by just summing phasors from each microphone.
But the resulting plots matched the old ones where I instead simulated a lot of sound samples and added them
(that old way was super slow + added a lot of quantization noise to the graphs).

Note that a bunch of patterns are generated with large microphone arrays, and when the array are larger then wavelength/2,
you will get strange sidelobes which some clever software possibly could work around <b>when listening</b> (but this repository is not for that).

I've not taken any acoustic course, and mostly improvised, so there could be glaring obvious errors in what I do.
