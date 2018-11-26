# About

BigBrother is console program written in C++ to monitor performance and availability of websites proposed by Datadog.

![preview](https://github.com/antoinecordelle/BigBrother/blob/master/screen.png)

# Table of contents

* [Setup](#Setup)
  * [Requirements](#Requirements)
  * [Installation](#install)
  * [Run](#run)
  * [Documentation](#documentation)
  * [Dependencies](#dependencies)
* [Architecture](#architecture)
  * [Overview](#overview)
  * [Structure](#structure)
  * [Code Structure](#code-structure)
* [Possible improvements](#possible-improvements)


# Setup

## Requirements :

The program requires :

 **C++11**

- **CMake** to manage the build proces
- **Ncurses** to manage the user interface (Dashboard) 
- **Google Test** 
- **Doxygen** to generate the documentation

The program has been tested on both Linux and MacOS. It will not work with Windows as Ncurses is a Unix type library, that Windows does not support. If that does raise any issue, I am at your disposal to find a solution.

## Installation :

Follow the 

### Linux :

**Libs installation**
```
apt-get install cmake
apt-get install ncurses
apt-get install doxygen   # to generate doc
```
**Google Test installation**
```
cd /usr/src/gtest
cmake CMakeLists.txt
make
cp *.a /usr/lib
```
 
**Project building**
```
cd (project)
mkdir build
cd build
cmake ..
make
```

### MacOS

**Libs installation**
```
brew install cmake
brew install ncurses
brew install doxygen   # to generate doc
```

**Google Test Installation**
```
git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake ..
make
make install
```

**Project building**
```
cd (project)
mkdir build
cd build
cmake ..
make
```

## Run

To run tests for the alert logic : 
In the (project)/build folder : 
``` ./tests/runTests```

To run the program :
In the (project)/build folder : 
```./src/app```


## Documentation

To generate the documentation :
Install doxygen : ```apt-get install doxygen``` or ```brew install doxygen```

Generation : in (project)/build : 
```
cmake ..
make doc
```

To access the documentation : Open ./doc/index.html

Most of the documentation is located in the header files.


## Dependencies

This program uses :
Ncurses for the console interface : C library the one used in programs like htop or vim for instance

Google Test : C++ test library

Doxygen


# Architecture

## Overview

This project is based around three parts :

- The Processing Part : back-end of the app in charge of sending the pings, collecting and processing the metrics (Website, Metrics, AlertHandler)

- The Dashboard : user console interface, displaying the metrics of the different websites

- The Application : this class is in charge of launching the differents websites (one thread per website) and the dashboard. This class handles the communication between the data processing, the alert handler and the dashboard.

This architecture separates clearly the data processing and the user interface, everything communicating through the application class.


## Structure

## Code structure

# Possible Improvements






















