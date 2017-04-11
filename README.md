# Update

[Online installer for x64 bit Linux distributions](https://github.com/vadrian89/cumulus-qt/releases/tag/2.0.1a) is live.  
Online installer for x32 bit Linux distributions To Be Decided.

To update properly please reinstall, otherwise update application and reinstall libraries only.

# Cumulus

Full port Qt/Qml is here!

Simple weather application, powered by [Yahoo! Weather](http://weather.yahoo.com) & [Open Weather Map](http://openweathermap.org/)

Forked from [typhoon](https://github.com/apandada1/typhoon) which was
Based on [Stormcloud](https://github.com/consindo/stormcloud/) by [Jono Cooper](https://twitter.com/consindo).

Features:
- resizable window with weather data adjusting accordingly
- units recalculations on-the-fly without data needed to be re-downloaded
- colors selectors for background and text with alpha channel, knock yourselves up ^^
- tray icon, with aditional data to be added on menu :)
- qt installer, without root installation to allow future updates fast without breaking on upgrade
- possbility for multiple instances with different settings and locations

# For multiple instances:

1. make a new shell script with the following content updated for Cumulus installation path  
#!/bin/bash  
\<path-to-cumulus-executable> \<whatever-you-want>  

Example: /home/user/Cumulus/Cumulus NewInstance

2. make the shell script executable  
Is as easy right-click script -> Properties -> Permisions -> check as executable  

\<Optional>  
3. if you want it to start on login  

Add the following content to your \<home-directory>/.profile file:  
if [ -f \<path-to-shell-script> ]; then  
  \<path-to-shell-script>  
fi  
Example:  
if [ -f /home/script.sh ]; then  
  /home/script.sh  
fi  
This will check if the script exists, so in case you accidentally delete you will not be getting errors on login.

# Major Thanks
- [Daryl Bennett](https://github.com/kd8bny)
- [Archisman Panigrahi](https://github.com/apandada1)
- [Erik Flowers](https://github.com/erikflowers) for his [weather icons](https://github.com/erikflowers/weather-icons),
which sadly has discontinued support for them

# Known Issues

Will be updated once the new version is live

# TODO
- Add another API
