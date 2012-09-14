brainbundler
============

The brainbundler repository contains the sources for two programs, which are both compiled using Qt (see http://qt.nokia.com/products/developer-tools/ for QT creator, or compile using qmake / make ...).

bundler
-------

Bundler is a mean-shift-bundling command-line tool. It takes a binary 3D graph as the input, and outputs a .fib (binary vtk-file used in fiber tracking software). The input can either be two ascii files (one with coordinates, the other describing connections); or a .fib-file, which only contains straight edges without intermediate points. 

### Usage:

<pre><code>
bundler (-nodes "nodesfile" -cons "connectionsfile" / -fib ".fib-file") [-c_thr "compatibility threshold"] [-start_i "iterations in 1st cycle"] [-numcycles "number of cycles"]
</code></pre>

### Input files

Both ascii files and .fib files can be used for loading data into Bundler.  

#### Ascii File-Formats:

The file formats for the ascii files are as follows:

The nodes file contains three coordinates (x,y,z) for each node on each line:

<pre>
x0 y0 z0
x1 y1 z1
x2 y2 z2
x3 y3 z3
etc...
</pre>

The list of connections contains pairs of indices in the coordinate file. Note that the first line in that file is referred to as node 0:

<pre>
0 3
2 3
4 1
etc...
</pre>

#### Converting ascii to .fib

To convert a pair of ascii-files to a single .fib-file, use the following command:

    bundler -nodes "nodefile" -cons "connectionsfile" -numcycles 0


### Parameters:

-c_thr: The compatibility threshold (default 0.8). This value determines how compatible two edges should be in order to move towards a common center of gravity. c_thr is responsible for how many distinct bundles emerge from the bundling: Too low values make everything bundle together, while too high values leave too many edges unbundled.

-start_i: Number of iterations in the first cycle; every following cycle has one iteration less. This parameter, like numcycles, influences the shape of the bundles.

-numcycles: Number of cycles. This parameter determines the shape of the bundles, as well as the number of subdivision points.


fibviewer
---------

...is a simple viewer for .fib-files. Basic usage is as follows:

fibviewer ".fib-file"

This would also be the place to start if you want to bundle your own data in a different format: Subclassing "Connections" should make it easy to look at data, and then export it to a .fib format file.

For an example, see "artificialconnections". It can be loaded with the following command:

    fibviewer artificial

In order to write a .fib-File, which can then be bundled with Bundler:

    fibviewer artificial -writefib

