# usblineswitch
Minimalistic linux control program for usb line switch, available from

http://www.reusch-elektronik.de/re/de/produkte/uls/index.htm

Currently, only one connected device is supported.

For using as non-root user, copy the 83-usblineswitch.rules to /etc/udev/rules.d/
then run

sudo udevadm control --reload-rules

and then remove and replug the USB device or run sudo udevadm trigger --subsystem-match=usb

Install dependencies by sudo apt install libusb-1.0-0 libusb-1.0-0-dev g++ make

Then run make and run ./usblineonoff --help to see how to use the progam.

Instead of manually copying the udev rule and run udevadm, you can call

make deb

to create a debian packet and install it with

sudo apt install ./usblineonoff_1.0_amd64.deb

Then just unplug and replug the USB device or run sudo udevadm trigger --subsystem-match=usb

