# News
Hello everybody,  
I have managed to work a bit on Cumulus, have been working on refactoring the code a bit because it was turning
into one big pile of \"mom's spaghetti\".
I haven't managed to make a snap out of Cumulus because it is a bit hard, because the
Qt version used in snap packaging is lower than what I am using so I am going to take a step back and focus on
application development.
I mentioned I wanted to focus on Ubuntu Touch development, but after taking a look at the bigger picture I saw that
I cannot focus to much on it because of all the other things I am working on.
That's about for this commit update.


# Info  
[![Build Status](https://travis-ci.org/vadrian89/cumulus-qt.svg?branch=master)](https://travis-ci.org/vadrian89/cumulus-qt)   
Both offline and online installers for 64 bit Linux distributions can be found at the [releases page](https://github.com/vadrian89/cumulus-qt/releases).

For any feedback you can mail me at verbanady@gmail.com, please add the following subject so I can save them into a special category: Cumulus - Feedback.
Thanks!

# Cumulus

Full port Qt/Qml is here!

Simple weather application, powered by:
- [Yahoo! Weather](http://weather.yahoo.com)
- [Open Weather Map](http://openweathermap.org/)
- [Weather Underground](https://www.wunderground.com/?apiref=697599e3b1e7adec)

Forked from [typhoon](https://github.com/apandada1/typhoon) which was
Based on [Stormcloud](https://github.com/consindo/stormcloud/) by [Jono Cooper](https://twitter.com/consindo).

Features:
- colors selectors for background and text with alpha channel, knock yourselves up ^^
- tray icon, with aditional data to be added on menu :)
- qt installer, without root installation to allow future updates fast without breaking on upgrade
- AppImage for those who want the application in a portable fashion
- possbility for multiple instances with different settings and locations

# For multiple instances:

1. make a new shell script with the following content updated for Cumulus installation path  
#!/bin/bash  
\<path-to-cumulus-executable> -i \<whatever-you-want>
Example: /home/user/Cumulus/Cumulus -i NewInstance

2. make the shell script executable

# In case you want to build it yourself:

- recommended Qt version: 5.9
- remove the following line from Cumulus.pro file: QMAKE_LFLAGS += "-Wl,-rpath,'$$ORIGIN/lib'"
- remove the following entry from qml.qrc file: "qt/etc/qt.conf"

# Major Thanks
- [Daryl Bennett](https://github.com/kd8bny)
- [Archisman Panigrahi](https://github.com/apandada1)
- [Erik Flowers](https://github.com/erikflowers) for his [weather icons](https://github.com/erikflowers/weather-icons),
which sadly has discontinued support for them

# Known Issues
- The resizing is implemented, but it's a bit haotic on Linux-based OSes, looking into it and hopefully I will find a solution soon.

# TODO
- Code refactoring
- Multiple locations from same instance
- Add possibility for translations
- Automatic location search
- Improve UI

