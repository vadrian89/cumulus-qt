# Announcement  
Hello everybody,  
The journey to Cumulus-Qt was a blast, met people, learned stuff and enjoyed seeing Cumulus-Qt articles here and there but I don't want it to stop here.    
I know I haven't updated the application in a while now but this wasn't because I didn't want to, it's just September was a full month for me(even though I have made some updates then) because I got married then and in October til now I simply couldn't handle myself to do any programming because of work schedule and my mind simply being tired.   
Going further and December right around the corner(with it's winter holidays) Cumulus-Qt might not get updated til January, the app isn't getting abandoned and I will monitory any issues which might arise in the issue tracker, it's just I want to focus on some other endeavors, such as:   

- working on a feature I promised to do for [linuxdeployqt](https://github.com/probonopd/linuxdeployqt), because it's a great tool for deploying on Linux(which it is used for AppImages, also used for Cumulus-Qt), maybe I will go further and create a front-end for it...who knows    
- learn snap to package Cumulus and any future projects I will get myself into         
- focus on Ubuntu Touch development; I loved UT and finally managing to get my hands on a Nexus 5 I want to focus my development into making applications and scopes for the platform, I hope something good will come from this  

Besides those mentioned above, I will also try to make Cumulus-Qt better, add more features, making a better UI aswell finishing making it a true cross-platform application, because it is my first open-source project.


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
- Add another API
- Code refactoring
- Improve application resizing

