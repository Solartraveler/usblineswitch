CC=g++
BINARY=usblineonoff
PACKAGEDIR=../packages
CONTROLFILE=$(PACKAGEDIR)/DEBIAN/control
VERSION=1.0

all:
	$(CC) -O2 -Wall -Wextra -D_VERSION_=\"$(VERSION)\" lineOnOff.cpp -lusb-1.0 -o $(BINARY)

deb: $(BINARY)
	mkdir -p $(PACKAGEDIR)/usr/bin
	cp $(BINARY) $(PACKAGEDIR)/usr/bin/
	mkdir -p $(PACKAGEDIR)/etc/udev/rules.d
	cp 83-usblineswitch.rules $(PACKAGEDIR)/etc/udev/rules.d/
	mkdir -p $(PACKAGEDIR)/usr/share/doc/$(BINARY)
	cp ../LICENSE $(PACKAGEDIR)/usr/share/doc/$(BINARY)/copyright

	rm -f $(CONTROLFILE)

	$(eval BINARYBYTES=$(shell du -s -k $(PACKAGEDIR) | cut -f1 ))
	#Note: This might fail for some type of crossbuilds!
	$(eval ARCHITECTURE=$(shell dpkg --print-architecture))

	echo "Package: usblineonoff" > $(CONTROLFILE)
	echo "Version: $(VERSION)" >> $(CONTROLFILE)
	echo "Priority: optional" >> $(CONTROLFILE)
	echo "Architecture: $(ARCHITECTURE)" >> $(CONTROLFILE)
	echo "Essential: no" >> $(CONTROLFILE)
	echo "Installed-size: $(BINARYBYTES)" >> $(CONTROLFILE)
	echo "Maintainer: Malte Marwedel" >> $(CONTROLFILE)
	echo "Homepage: https://github.com/Solartraveler/usblineswitch" >> $(CONTROLFILE)
	echo "Description: Program for controling one USB line switch device" >> $(CONTROLFILE)
	echo "Depends: libusb-1.0-0" >> $(CONTROLFILE)
	echo "Build-Depends: libusb-1.0-0, libusb-1.0-0-dev, g++ make" >> $(CONTROLFILE)
	dpkg-deb --build $(PACKAGEDIR) ../$(BINARY)_$(VERSION)_$(ARCHITECTURE).deb

clean:
	rm -f $(BINARY)
	rm -f $(CONTROLFILE)

cleanbackup:
	rm -f *~