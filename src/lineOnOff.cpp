/*
(c) 2020 by Malte Marwedel

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

--------------------------------------------------------------------------------


Some ideas are from:
https://www.dreamincode.net/forums/topic/148707-introduction-to-using-libusb-10/

Controls the USB Line Switch (tested wit version 2.3), which is available for
buying under:
http://www.reusch-elektronik.de/index_de_uls.htm

For proper compiling under Debian 10:
apt install libusb-1.0-0-dev g++ make

Restrictions:
This works only with one device connected. If multiple are present, the first
found is used.

*/

#include <stdio.h>
#include <string.h>

#include <libusb-1.0/libusb.h>

#define VID 0x16C0
#define PID 0x2823

#define CONTROLBYTES 8

//in ms
#define TIMEOUT 500

int main(int argc, char ** argv) {
	const char * argument = NULL;
	if (argc >= 2) {
		argument = argv[1];
		if ((strcmp(argument, "on")) && (strcmp(argument, "off")))
		{
			printf("LineOnOff version 1.0 (c) 2020 by Malte Marwedel\n");
			printf("Usage: \n");
			printf("No arguments: Prints the current USB line switch status\n");
			printf("         off: Powers off the switch\n");
			printf("          on: Enables the switch\n");
			printf("Everything else prints this screen\n");
			printf("This program has the following exit codes:\n");
			printf("  0 on success\n");
			printf(" -1 if an USB error occurrs\n");
			printf(" -2 if the written state does not match the readback state\n");
			printf(" 1 if this screen is printed\n");
			return 1;
		}
	}
	libusb_context * plc;
	if (libusb_init(&plc) != LIBUSB_SUCCESS) {
		printf("Error, could not initialize libusb\n");
		return -1;
	}
	libusb_device_handle * pldh = libusb_open_device_with_vid_pid(plc, VID, PID);
	int exitCode = 0;
	if (pldh) {
		unsigned char data[CONTROLBYTES] = {0};
		unsigned int requested = 0;
		if (argument) { //write case
			if (strcmp(argument, "on") == 0) {
				//printf("New state should be on\n");
				requested = 1;
			} else {
				//printf("New state should be off\n");
			}
			data[0] = requested;
			const int bRequest = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE;
			//somehow the new state is present both, in the data and as wValue
			//bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout [ms]
			int status = libusb_control_transfer(pldh, bRequest, 1, requested, 0, data, CONTROLBYTES, TIMEOUT);
			if (status >= 0) { //we dont expect any bytes to return
				//printf("New state written\n");
			} else {
				printf("Error: Write control failed with error code %i\n", status);
				exitCode = -1;
			}
		}
		//always read what the device reports

		//bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout [ms]
		memset(data, 0, CONTROLBYTES);
		const int bRequest = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE;
		int status = libusb_control_transfer(pldh, bRequest, 0, 0, 0, data, CONTROLBYTES, TIMEOUT);
		if (status > 0) { //we want at least one byt
			printf("The current state is %s\n", data[0] & 1 ? "on" : "off");
		} else {
			printf("Error: Read control failed with error code %i\n", status);
			exitCode = -1;
		}
		libusb_close(pldh);
		//verify should vs is
		if (argument) {
			if (data[0] != requested) {
				printf("Error, written and read back state do not match!\n");
				exitCode = 2;
			}
		}
	} else {
		printf("Error: No device connected, or not enough permissions! Have you added a proper udev rule or tried as root?\n");
		exitCode = -1;
	}
	libusb_exit(plc);
	return exitCode;
}
