brainbundler
============

The brainbundler repository contains the sources for two programs, both to be compiled using Qt (see http://qt.nokia.com/products/developer-tools/ for QT creator, or do the usual qmake / make thing...).

bundler
-------

...contains the mean-shift-bundling command-line tool. It takes a binary graph as input, and outputs a .fib (binary vtk-file used in fiber tracking software). The input can either be a pair of ascii files with coordinates / connections; or a .fib-file, which only contains straight edges with no intermediate points. Usage as follows:

"bundler (-nodes <nodes> -cons <connections> / -fib <.fib-file>) [-c_thr <compatibility threshold>] [-start_i <iterations in 1st cycle>] [-numcycles <number of cycles>]"

Ascii File-Formats
..................

The file formats for the ascii files are as follows:

The nodes file contains three coordinates (x,y,z) for each node on each line:

x0 y0 z0
x1 y1 z1
x2 y2 z2
x3 y3 z3
etc...

The list of connections contains pairs of indizes in the coordinate file. Note that the first line in that file is referred to as node 0:

0 3
2 3
4 1
etc...

Parameters
..........

c_thr: The compatibility threshold (default 0.8). This value determines how compatible two edges have to be in order to move towards a common center of gravity. c_thr is responsible for how many distinct bundles emerge from the bundling: Too low values make everything bundle together, while too high values leave too many edges unbundled.

start_i: Number of iterations in the first cycle; every following cycle has one iteration less. This parameter, like numcycles, influences the shape of the bundles.

numcycles: Number of cycles. This parameter determines the shape of the bundles, as well as the number of subdivision points.

Also: To convert a pair of ascii-files to a .fib-file, use the following:

bundler -nodes <nodefile> -cons <connections> -numcycles 0

fibviewer
---------

...is a simple viewer for .fib-files. Basic usage is as follows:

fibviewer <.fib-file>

This would also be the place to start if you want to bundle your own data in a different format: Subclassing "Connections" should make it easy to look at data, and then export it to .fib.

For an example (or if you simply want some data to play around with) have a look at "artificialconnections". That data gets loaded if the first argument is:

fibviewer artificial

fibviewer artificial -writefib

then writes a .fib-File, which can be bundled with the bundler...
