# usblineswitch
Minimalistic linux control program for usb line switch, available from

http://www.reusch-elektronik.de/index_de_uls.htm

For using as non-root user, copy the 83-usblineswitch.rules to /etc/udev/rules.d/
then run udevadm control --reload-rules and then remove and replug the USB device.

Install dependencies by sudo apt install libusb-1.0-0-dev g++ make

Then run make and run ./lineonoff --help to see how to use the progam.

