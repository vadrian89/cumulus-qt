# News
Hello everybody,  
I have managed to work a bit on Cumulus, have been working on refactoring the code a bit because it was turning
into one big pile of \"mom's spaghetti\".    
Added:  
- Positioning support for location search, it automatically searches for your location when you want to add a new location.  
- Positioning support for automatic location, opt-in if you want the application to detect your location and update the weather based of your actual position.  
*These 2 are tied to services based on platform, it uses GeoClue on Linux, if it isn't installed it won't find your location.  
- Multiple locations can be saved from inside the application.   
- AppImage package "Start on login" support, enabling the option from an AppImage should correctly detect the package's location and add it to ~/.config/autostart  


Changed:  
- Layout of forecast from Left -> Right, to Top -> Bottom.  
- Whole lot of refactoring which can be seen in the commit logs.      


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

# Major Thanks
- [Daryl Bennett](https://github.com/kd8bny)
- [Archisman Panigrahi](https://github.com/apandada1)
- [Erik Flowers](https://github.com/erikflowers) for his [weather icons](https://github.com/erikflowers/weather-icons),
which sadly has discontinued support for them  
- People working on [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) and [AppImage](https://github.com/AppImage)    

# Known Issues

- [Yahoo search might not work](https://github.com/vadrian89/cumulus-qt/issues/3) on non-Ubuntu distros, depends on ssl library used in the system  
- AppImage seems highly unstable on Deepin Linux, might also be the case for other distros.  

# TODO
- Code refactoring  
- Add possibility for translations  
- Improve UI  
- Rename application   
- Support for custom API keys, for OpenWeatherMap and Weather Underground     
- Snap package  
- Ubuntu Touch support   
