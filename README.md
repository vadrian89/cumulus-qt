# Important announcement
Weather underground have [terminated](https://apicommunity.wunderground.com/weatherapi/topics/end-of-service-for-the-weather-underground-api) it's API so I will need to replace it.
The application's ability to search for locations and get GPS feature might also break because they are dependent of Weather underground and Yahoo! Weather.
Will do my best to fix this issue in a fast time frame.

# News
Hello everybody,  
I have managed to work a bit on Cumulus and here are some small updates:
- Added support for translations, it is only for UI language and current only translated to romanian language; Will implement a way to translate weather conditions soon;
- Added a button to toggle between locations list and weather view;
- Made the toggle for window controls positioning available for Android users as well;
- Started reimplementing the option to remove window decorations from window manager; In the future you will be able to select if you want to use window manager decorations or application's;
- Removed Yahoo! Weather provider from UI, because they changed how their provide their API(yet again);
- Fixed an issue where the high and low temperatures for the current weather were taken from tomorrow's forecast;
- Fixed an issue where the application won't keep the windows controls locations;

If you want to add translations of your language to your UI, do the following:
- clone the repository;
- copy tempestas_ro.po to tempestas_\<country initials\>.po;
- modify your copy with the text from romanian to your language;
- make pull request with your change;

# Info  
[![Build Status](https://travis-ci.org/vadrian89/cumulus-qt.svg?branch=master)](https://travis-ci.org/vadrian89/cumulus-qt)   
Both offline and online installers for 64 bit Linux distributions can be found at the [releases page](https://github.com/vadrian89/cumulus-qt/releases).
For any feedback you can mail me at verbanady@gmail.com, please add the following subject so I can save them into a special category: Cumulus - Feedback.
Thanks!

# Cumulus
Cross platform and open source weather application for GNU/Linux and Android built with Qt.

Forked from [typhoon](https://github.com/apandada1/typhoon) which was
Based on [Stormcloud](https://github.com/consindo/stormcloud/) by [Jono Cooper](https://twitter.com/consindo).

Features:
- colors selectors for background and text with alpha channel, knock yourselves up ^^;
- tray icon, with aditional data to be added on menu :);
- qt installer, without root installation to allow future updates fast without breaking on upgrade;
- AppImage for those who want the application in a portable fashion;
- possbility for multiple instances with different settings and locations;
- multiple weather providers: [Open Weather Map](http://openweathermap.org/) & [Weather Underground](https://www.wunderground.com/?apiref=697599e3b1e7adec);
- locations list;

# For multiple instances:

1. make a new shell script with the following content updated for Cumulus installation path  
#!/bin/bash  
\<path-to-cumulus-executable> -i \<whatever-you-want>
Example: /home/user/Cumulus/Cumulus -i NewInstance

2. make the shell script executable

# In case you want to build it yourself:

- recommended Qt version: 5.9  

# Major Thanks:
- [Daryl Bennett](https://github.com/kd8bny)
- [Archisman Panigrahi](https://github.com/apandada1)
- [Erik Flowers](https://github.com/erikflowers) for his [weather icons](https://github.com/erikflowers/weather-icons),
which sadly has discontinued support for them  
- People working on [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) and [AppImage](https://github.com/AppImage)    

# Known Issues
- [Yahoo is currently not working](https://github.com/vadrian89/cumulus-qt/issues/40);
- AppImage seems highly unstable on Deepin Linux, might also be the case for other distros;
- Geo location is not working correctly;

# TODO
- Fix connection to [Yahoo! Weather](http://weather.yahoo.com);
- Code refactoring;
- Translations for weather conditions;
- Improve UI;
- Rename application;
- Support for custom API keys, for OpenWeatherMap and Weather Underground;
- Snap package;
- Ubuntu Touch support;
