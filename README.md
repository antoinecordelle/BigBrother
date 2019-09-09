# About

BigBrother is a console program written in C++ to monitor performance and availability of websites.

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
  * [File Structure](#file-structure)
* [Possible improvements](#possible-improvements)


# Setup

## Requirements :

The program requires :

 At least **C++11**

- **CMake** to manage the build proces
- **Ncurses** to manage the user interface (Dashboard) 
- **Google Test** 
- **Doxygen** to generate the documentation

The program has been tested on both Linux and MacOS. It will not work with Windows as Ncurses is a Unix type library, that Windows does not support. If that does raise any issue, I am at your disposal to find a solution.

## Installation :

### Libs for Linux :

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

### Libs for MacOS

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

### Project building (both MacOS and Linux)
```
cd (project)
mkdir build
cd build
cmake ..
make
```

If you encounter any issue while installing, I am at your disposal to find the solution.

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

To access the documentation : Open ```(project)/build/doc/html/annotated.html```

Most of the documentation is located in the header files.


## Dependencies

This program uses :
Ncurses for the console interface : C library the one used in programs like htop or vim for instance

Google Test : C++ test library

Doxygen


# Architecture

## Overview

### Global architecture

This project is based around three parts :

The Processing Part : Back-end of the app in charge of :
* Sending the pings
* Collecting the metrics
* Processing the metrics (Website, Metrics, AlertHandler)
* Sending the metrics to the Application

The Dashboard : user console interface in charge of :
* Initializing the webistes and ping intervals
* Displaying the metrics of the different websites
* Receiving user input to chose the focused website and exit the application

The Application in charge of :
* Launching the differents websites (one thread per website) and the dashboard
* Taking care of the communication between the data processing, the alert handler and the dashboard.

This architecture separates clearly the data processing and the user interface, everything communicating through the application class.

### Lib choices

**Curses** is the console interface library used for most of the console programs used in Unix systems like htop or vim. To build a console interface, using ncurses was the logical choice and a quite interesting C library to use.

**GoogleTest** is one of the main C++ test libraries (with QTest and Boost Test)

**Doxygen** allows to generate a complete documentation from a templated header commenting. Therefore, most of the comments are located in the headers, allowing more readable source files.

## Structure

The global process is described in the [overview](#overview). In this section I will detail the most important aspects of the project :

Classes :

* **Alert :**
AlertHandler, receives data from the Application (past two minutes) and sends back an alert (availability, time and name of the website) if needed.

* **Application :**
Center of the program : it manages the websites, the alertHandler and the dashboard.
Launchs websites' threads, dashboard's thread. Every 10sec it gets Metrics for the past 2minutes and 10minutes from the different websites. Every minute, it gets the metrics from the past hour.
It then sends the 10min and a hour metrics to the dashboard, and the 2min metrics to the alertHandler.
When the dashboard sends the exit signal, application shuts down the threads.

* **Website :**
Represents each website to monitor, possessed by the application. Contains a list of the pings from the past hour, and a set of metrics corresponding to the different time windows. From a separated thread it requests a ping, which is added to the list and updates the metrics.
When the application retrieves the metrics, the old pings are removed from the metrics statistics.

* **Metrics :**
Possessed by a website, one "metrics" per time window per website(2min, 10min, 1hour)
Updated whenever a ping arrives.
**Old pings removal algorithm :** A Metrics object possesses an iterator in the website's ping list, corresponding to the oldest ping taken into account in the metrics. Each time the metrics is called, old pings need to be removed. The iterator allows us to have to iterate only from this oldestPing (now too old) to the new oldestPing that should be taken. Therefore, instead of iterating each time through the whole ping list ( O(n) at least), we only have to iterate through the last getMetricsInterval, which is either 10sec or 1hour. This allows a really efficient ping removal, in constant time complexity against the ping count.

* Dashboard
* Error
* Pinger
* Utility


## File structure
```
. 
├── /src                    
│   ├─── /Alert       
│   ├─── /Application
│   ├─── /Dashboard
│   ├─── /Error
│   ├─── /Metrics
│   ├─── /Pinger
│   ├─── /Utility
│   ├─── /Website
│   ├─ CMakeLists.txt           
│   └─ main.cpp   
├── /doc  
│   ├─ CMakeLists.txt      
│   └─ Doxyfile.in
├── /tests  
│   ├─ CMakeLists.txt           
│   └─ test.cpp   
└─ CMakeLists           
```

# Possible Improvements

### Processing improvements :

**Other metrics to add :** Some interesting metrics can be added, such as a more detailed error tracking, or the 99th percentile which could be a really interesting metrics (allowing to know if a really high maxTime ping is an exception or not for example). 

**Program Configuration :** Right now, the timeWindow and refreshing times are coded in the application. Ideally, a configuration file should be added to the program to allow the user to change more easily these times.

**Resiliency to network interruptions :** A network interruption results in unexpected ping errors, which may shutdown the dashboard and the program. 

**Unit testing :** Currently, only the AlertLogic and a function from website are tested by GoogleTest. On the long run, a more systematic unit testing would be a good investment, allowing the project to be more maintainable.

Allow the addition of new websites while monitoring the actual websites.

### Dashboard specific improvements :

**Better responsivity :** The dashboard's responsivity is not ideal, especially when setting the console size to a way smaller window.

**Show the evolution of the metrics :** Being able to monitor the evolution of the metrics, with graphs showing the avgTime evolution for exemple, would be a great assset, allowing to visualize the performance of the website over time.



















