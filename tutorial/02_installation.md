\page installation Installation

Next Tutorial: \ref nodestopics
Previous Tutorial: \ref introduction

## Overview

This page contains instructions to install Aerozen on all the
support platforms including major Linux.

You can find all Aerozen versions at [Aerozen](https://github.com/quandy2020/aerozen).

# Source Install

## Install Prerequisites

### Ubuntu Linux

For compiling the latest version of Aerozen you will need an Ubuntu distribution or newer.

Make sure you have removed the Ubuntu pre-compiled binaries before
installing from source:

Install prerequisites. A clean Ubuntu system will need:
```
sudo apt-get install git cmake pkg-config python3 python3-pybind11 python3-pytest ruby-ronn libprotoc-dev libprotobuf-dev protobuf-compiler uuid-dev libzmq3-dev libzenohc-dev libzenohcpp-dev libsqlite3-dev
```

## Clone, Configure, and Build

Clone the repository
```bash
git clone https://github.com/quandy2020/aerozen.git
```
Configure and build
```
cd aerozen
mkdir build
cd build
cmake ..
make
```

Optionally, install
```
sudo make install
```

### Configuration options

Configure Aerozen (choose either method a or b below):

A.  Release mode (recommended): This will generate optimized code, but will not have
    debug symbols. Use this mode if you don't need to use [GDB](https://www.gnu.org/software/gdb/) (advanced).
```
cmake ..
```

Note: You can use a custom install path to make it easier to switch
between source and debian installs:
```
cmake -DCMAKE_INSTALL_PREFIX=/home/$USER/local ..
```

B. Debug mode: This will generate code with debug symbols.Aerozen will run slower, but you'll be able to use GDB.
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

The output from `cmake ../` may generate a number of errors and warnings
about missing packages. You must install the missing packages that have
errors and re-run `cmake ../`. Make sure all the build errors are
resolved before continuing (they should be there from the earlier step
in which you installed prerequisites).

Make note of your install path, which is output from cmake and should
look something like:
```
-- Install path: /home/$USER/local
```

Build Aerozen:
```
make -j4
```

Install Aerozen:
```
sudo make install
```

If you decide to install aerozen in a local directory you'll need to
modify your `LD_LIBRARY_PATH`:
```
echo "export LD_LIBRARY_PATH=<install_path>/local/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
```

### Build python bindings separately from main library

If you want to build Python bindings separately from the main gz-transport library
(for example if you want to build Python bindings for multiple versions of Python),
you can invoke cmake on the `python` folder instead of the root folder.
Specify the path to the python executable with which you wish to build bindings
in the `Python3_EXECUTABLE` cmake variable.
Specify the install path for the bindings in the `CMAKE_INSTALL_PREFIX`
variable, and be sure to set your `PYTHONPATH` accordingly after install.

```bash
cd sdformat
mkdir build_python3
cd build_python3
cmake ../python \
    -DPython3_EXECUTABLE=/usr/local/bin/python3.12 \
    -DCMAKE_INSTALL_PREFIX=<prefix>
```

See the homebrew [sdformat15 formula](https://github.com/osrf/homebrew-simulation/blob/027d06f5be49da1e40d01180aedae7f76dc7ff47/Formula/sdformat15.rb#L12-L56)
for an example of building bindings for multiple versions of Python.

### Uninstalling Source-based Install

If you need to uninstall  Aerozen or switch back to a
debian-based install when you currently have installed the library from
source, navigate to your source code directory's build folders and run
`make uninstall`:

```
cd /tmp/aerozen/build
sudo make uninstall
```

## Build documentation
```bash
cd build
make doc
```

View the documentation by running the following command from the `build` directory.
```
firefox doxygen/html/index.html
```

# Testing

Tests can be run by building the `test` target. From your build directory you
can run:
```bash
make test
```

*Note:* Python tests require running `sudo make install` before running `make test`.
