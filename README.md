# Update

[Online installer for x64 bit Linux distributions](https://github.com/vadrian89/cumulus-qt/releases/tag/2.0.1a) is live.  
Online installer for x32 bit Linux distributions TBA. 

# Cumulus

Ported to Qt/Qml!   
This is the first step, the app will make a full transition to Qt/Qml because in it's current state
the installation size is to big.

Simple weather application, powered by [Yahoo! Weather](http://weather.yahoo.com) & [Open Weather Map](http://openweathermap.org/)

Forked from [typhoon](https://github.com/apandada1/typhoon) which was
Based on [Stormcloud](https://github.com/consindo/stormcloud/) by [Jono Cooper](https://twitter.com/consindo).

# Major Thanks
- [Daryl Bennett](https://github.com/kd8bny)
- [Archisman Panigrahi](https://github.com/apandada1)
- [Adam Whitcroft](https://twitter.com/AdamWhitcroft) for [Climacons](http://adamwhitcroft.com/climacons/).

# Known Issues
- Yahoo weather is a mess at the moment and works less then it should, that's why I am working on adding another API
- Counter is working only for Unity(naturally since the original application was intended for Ubuntu with Unity)
- No hide from launcher option yet  
- Due to application being an webapp, the webengine dependencies are quite a few and some libraries have biger size than desired and installed the application currently uses almost 200mb
- Gradient selections for background is a bit of a mess

# TODO
- Complete port to Qt
- Custom Color
- Add another API
