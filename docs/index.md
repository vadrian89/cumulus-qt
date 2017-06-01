---
layout: default
title: Cumulus
---

# Cumulus QT
## About
Cumulus is a simple weather application, with a back end powered by [Yahoo! Weather](http://weather.yahoo.com) & [Open Weather Map](http://openweathermap.org/).  
It is and always will be free, licensed under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).

This is a port to Qt/Qml of the [original cumulus](legacy_cumulus.md)

![weather](assets/images/weather.gif) ![location](assets/images/location.png)

## Features
- Colors selectors for background and text with alpha channel, knock yourselves up out
- Re-sizable window with weather data adjusting accordingly
- Units recalculations on-the-fly without data needed to be re-downloaded
- Tray icon, with additional data to be added on menu :)
- Qt installer, without root installation to allow future updates fast without breaking on upgrade
- Possibility for multiple instances with different settings and locations [Read Here](multiple_inst.md)

## Install/Update
1. Download installer for x64 bit Linux distributions
 - [Linux amd64](https://github.com/vadrian89/cumulus-qt/releases/)
2. Mark installer executable
`$sudo chmod +x Cumulus-online-installer-x64`
3. Run
`$./Cumulus-online-installer-x64`
4. Enjoy!

## Known Issues
- The resizing is a bit haotic on Linux-based OSes, looking into it.

## History
Forked from [typhoon](https://github.com/apandada1/typhoon) which was
Based on [Stormcloud](https://github.com/consindo/stormcloud/) by [Jono Cooper](https://twitter.com/consindo)

## Major Thanks
- [Adrian Verban](https://github.com/vadrian89)
- [Daryl Bennett](https://github.com/kd8bny)
- [Archisman Panigrahi](https://github.com/apandada1)
- [Adam Whitcroft](https://twitter.com/AdamWhitcroft), for his fonts used in original Typhoon/Cumulus
- [Erik Flowers](https://github.com/erikflowers) for his [weather icons](https://github.com/erikflowers/weather-icons),
which sadly has discontinued support for them

## TODO
- Add another API
