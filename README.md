actuator-belt
=============

The actuactor-belt project! Trying to experience the [haptic compass](http://feelspace.cogsci.uni-osnabrueck.de/).
This is the result of a day hacking with friends.

![Prototype](https://raw.github.com/pimnijdam/actuator-belt/master/doc/prototype.jpg)


A small belt with vibrating motors is worn around the ankle and fastened using your sock. The arduino can be worn a little heigher on the leg, being quite comfortably strapped to the leg. Alternatively the Arduino can also be placed inside your sock.

The Phone senses north using the compass, it sends over bluetooth the command to turn on a specific motor. The Arduino Nano receives the command and periodicly (each second) turns the specified motor on for 100 ms.

## Hardware

### Components:

- Arduino Nano
- 8 vibrating motors
- Ribbon cable
- 9V battery + clip
- Bluetooth serial module

Motors are directly connected to the ribbon cable instead of applying a connector.

### Schematic
![Schematic](https://raw.github.com/pimnijdam/actuator-belt/master/schematics/sheet1.jpg)


## Power consumption ##
Here are some power consumption measurements.

| Mode      | Current (mA)|
|-----------|-------------|
|Standby    | 24          |
|Scanning   | 35-65       |
|Connected (idle) | 31    |
|BT Receiving[1] | 47        |
|1 Motor vibrating | 70   |
|BT Receiving + 1 Motor vibrating | 90 |
|8 motors vibrating | ~400|

It appears standby power consumption is alot. This is probably the main area that can be improved.
The motors also draws quite some power, but this is already addressed with the 10% duty cycle.
Also bluetooth receiving is addressed somewhat by only sending changes.

[1]: These measurements were taken a while back, I don't remember the tail time for BT receiving.

## Credits ##
Credits to the researchers of the [feelspace group](http://feelspace.cogsci.uni-osnabrueck.de/) who inspired us to create our version of the haptic compass. But most of all to my friends which whom I developed this fully functional prototype.

- [Maarten](https://github.com/MaartenFaddegon), persevering to get things done the Scala way.
- [Martin](https://github.com/mnitram), wielding the soldering iron with blazing speed.
- [Pepijn](https://github.com/Gneisbaard), producing ideas and subsequent arduino code effortlessly.
