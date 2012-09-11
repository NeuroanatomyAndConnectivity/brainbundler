brainbundler
============

The brainbundler repository contains the sources for two programs, both to be compiled using Qt (see http://qt.nokia.com/products/developer-tools/ for QT creator, or do the usual qmake / make thing...).

bundler
-------

...contains the mean-shift-bundling command-line tool. It takes a binary graph as input, and outputs a .fib (binary vtk-file used in fiber tracking software). The input can either be a pair of files with coordinates / connections; or a .fib-file, which only contains straight edges with no intermediate points. Usage as follows:

"bundler (-nodes <nodes> -cons <connections> / -fib <.fib-file>) [-c_thr <compatibility threshold>] [-start_i <iterations in 1st cycle>] [-numcycles <number of cycles>]"

fibviewer
---------

...is a simple viewer for .fib-files. Basic usage is as follows:

fibviewer <.fib-file>

This would also be the place to start if you want to bundle your own data in a different format: Subclassing "Connections" should make it easy to look at data, and then export it to .fib.

For an example (or if you simply want some data to play around with) have a look at "artificialconnections". That data gets loaded if the first argument is:

fibviewer artificial

fibviewer artificial -writefib

then writes a .fib-File, which can be bundled with the bundler...
