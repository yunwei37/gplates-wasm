# GPlates WebAssembly

A port of GPlates 2.3.0 with WebAssembly.

## Compiling GPlates with Emscripten

Use the Qt Creator to open the `CMakelists.txt` file in the root directory of the project, and then select the `WebAssembly_Qt_6_5_0_single_threaded` build kit to compile.

The tested compiling platform is Windows 11.

### Dependencies

- Qt 6.5.0
- emscripten 3.1.25
- CMake
- GDAL library: <https://github.com/yunwei37/gdal-proj-wasm>

All other dependencies are checked in and included in the `third_party` directory.

## demo

<https://yunwei37.github.io/gplates-wasm/examples/wasm-gplates-qt-demo/>

## benchmark

<https://yunwei37.github.io/gplates-wasm/examples/benchmark/benchmark-js.html>

## GPlates README

This is the source code for the GPlates software.

GPlates is open-source software, distributed under the terms of the GNU General
Public License (version 2, dated June 1991) unless otherwise explicitly stated.
A copy of the license can be found in the file "COPYING" in this directory.

Compiling GPlates on Linux
--------------------------

To compile GPlates on Linux, you will need the programs and libraries as
documented in the "Program and library dependencies" section in the "DEPS.Linux"
file in this directory.

After installing those dependencies, to compile on Linux, execute:
    cmake .
    make
and then optionally:
    sudo make install

More information can be found in the files "DEPS.Linux" and "BUILD.Linux" in
this directory.

Compiling GPlates on macOS
--------------------------

To compile GPlates on macOS, you will need the programs and libraries as
documented in the "Program and library dependencies" section in the "DEPS.OSX"
file in this directory.

After installing those dependencies, to compile on macOS, execute:
    cmake .
    make
and then optionally:
    sudo make install

More information can be found in the files "DEPS.OSX" and "BUILD.OSX" in
this directory.

Compiling GPlates on Windows
----------------------------

To compile GPlates on Microsoft Windows, you will need the programs and libraries
as documented in the "Program and library dependencies" section in the "DEPS.Windows"
file in this directory.

To compile on Windows:

- Run the CMake GUI program.
- Set both the 'source' and 'build' directories to the root GPlates source directory
   (the one that has 'src' as a sub-directory).
- Click 'configure'.
- Specify 'Visual Studio 14 2015', or above.
  - And specify 'x64' as the optional platform (to compile GPlates as 64-bit).
- Click 'configure' once more.
- Click 'generate'.
- Open newly created 'GPlates.sln' with the Visual Studio IDE.
- Select 'Release' build from the "Build->Configuration Manager..." menu.
- Right-click the 'gplates' project and select 'build'.

More information can be found in the files "DEPS.Windows" and "BUILD.Windows" in this directory.

GPlates on the Web
------------------

For up-to-date information about GPlates, you can go to the GPlates website:
 <http://www.gplates.org/>
