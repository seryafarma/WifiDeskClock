# WifiDeskClock
Desktop Clock with WiFi for accurate time display!


![clock](https://github.com/seryafarma/WifiDeskClock/blob/master/wifideskclock.jpg?raw=true)

## Introduction
I like clock. Especially digital ones. I made a digital desktop clock already using DS1307 as shown in my other repo:

However there are things that can be made much better.
- DS1307 time drift. After a few months / years, the time is not accurate anymore.
- No time synchronization. During setting time, need to really set it at 00 seconds to be a bit accurate.
- Inability to store timezone information.
- Need to adjust the clock for Summer Time / Winter Time. I ended up +- 1 hour. It is only 2x a year, but still.
- DS1307 is an extra hardware. Extra cabling. Extra ordering. Also CR1032 battery needed.

Having a WiFi clock which is synchronized with NTP server makes the time accuracy high. 
I don't have to set the +- 1 hour for daylight savings. 
Also I can select the timezones easily (thanks to the ezTime Library)
The only thing I need is a proper internet connection and WiFi signal.

## System Description
Hardware used:
* WeMos D1 R1 (Old version)
* max7219 4x8x8 led matrix

## Credits
### Time library
https://github.com/ropg/ezTime/

### Display library
https://github.com/MajicDesigns/MD_MAX72XX