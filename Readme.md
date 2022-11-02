## ESP-IDF component for HM3301

![HM3301](https://user-images.githubusercontent.com/2692928/199354622-72c09356-7dbc-43ad-882c-4306de36ace9.jpg)

Datasheet of the HM3301 you can find in [HM3301 Wiki](https://github.com/martinberlin/HM3301-idf/wiki).
Our own PCB controller based on ESP32S3 with 2MB PSRAM is available in our [Tindie store](https://www.tindie.com/stores/fasani)

Based on [Seeed Arduino library for HM330x](https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301)

Heavily inspired in Tomoto version for Arduino

Licensed with MIT, please copy it, and make it better!

### Requirements

ESP-IDF >= v.4.4
An additional component that is a wrapper of i2c, called i2cdev, that is just 2 files and you can grab from the wiki:
https://github.com/martinberlin/HM3301-idf/wiki

This come as an idea with our project [epaper-weather-station](https://github.com/martinberlin/epaper-weather-station) because if you use RTC and other sensors, then it's silly to add to each of them their own I2C library!
Just share one common wrapper and use it for all of them.


PRs are welcome if you find an error and want to correct something.

Martin Fasani, Barcelona

[Fasani Corporation](https://fasani.de)

November, 2022