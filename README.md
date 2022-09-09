# GSoC 2022 Work Product Submission
## Improving TIFF image support in GNU Octave
In this project I worked on Improving support for TIFF images in Octave by implementing the classdef `Tiff` that exposes all the functionality of LibTIFF to the Octave code. I also modified the image functions of Octave (`imread`, `imwrite` and `imfinfo`) to use the new interface when handling TIFF images.

All the code written throughout the coding period is on this forked mercurial repository [octave-libtiff](https://hg.octave.org/octave-libtiff/). The commits relevant to the project start from [changeset 31092:a736190ce738](https://hg.octave.org/octave-libtiff/rev/a736190ce738) and end at [changeset 31217:c09f6a3597e6](https://hg.octave.org/octave-libtiff/rev/c09f6a3597e6) with all the changesets in between those included.

The fork is in the process of getting merged into the main Octave repository through the patch request [here](https://savannah.gnu.org/patch/?10276).

Weekly updates on the progress throughout the coding period are available through a series of posts on my website [here](https://magedrifaat.github.io/).

### Usage
The deliverables of this project will be usable directly through future Octave releases once the fork is merged.
In the mean-time, in order to use the new features implemented in this project, you can build Octave using my forked repository.

To do that, start by cloning my repository:
```shell
hg clone https://hg.octave.org/octave-libtiff
```
Then follow the steps for building Octave through their [tutorial](https://wiki.octave.org/Building).

To use the Tiff class, check out the weekly update posts mentioned earlier or you can use matlab's documentation for the class [here](https://www.mathworks.com/help/matlab/ref/tiff.html).