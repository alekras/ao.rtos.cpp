using namespace std;
/*
 * SerialPort.cpp
 *
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/ioctl.h>

#include "Include/SerialPort.h"


int SerialPort::connect() {
	return connect("/dev/ttyS0");
}

int SerialPort::connect(char *device) {
	struct termios terminalAttributes;

	/*
	 * http://linux.die.net/man/2/open
	 *
	 * Open the serial port
	 * read/write
	 * not become the process's controlling terminal
	 * When possible, the file is opened in nonblocking mode
	 *
	 */
	fileDescriptor = open(device, O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY | O_FSYNC );
  if (fileDescriptor == -1) {
   /** Could not open the port. */
    cout << "open_port: Unable to open /dev/ttyS0 - ";
    return -1;
  }
  else
    fcntl(fileDescriptor, F_SETFL, 0);
	// clear terminalAttributes data
	memset(&terminalAttributes, 0, sizeof(terminalAttributes));

	/*	http://linux.die.net/man/3/termios
	 *
	 *  control modes: c_cflag flag constants:
	 *
	 * 19200 bauds
	 * 8 bits per word
	 * Ignore modem control lines.
	 * Enable receiver.
	 */

	terminalAttributes.c_cflag = CS8 | CLOCAL | CREAD;

	/*
	 * input modes: c_iflag flag constants:
	 *
	 * Ignore framing errors and parity errors.
	 * (XSI) Map NL to CR-NL on output.
	 */
	terminalAttributes.c_iflag = IGNPAR |  ONLCR;

	/*
	 * output modes: flag constants defined in POSIX.1
	 *
	 * Enable implementation-defined output processing.
	 */

	terminalAttributes.c_oflag &= ~OPOST; //OPOST;

	/*
	 * Canonical and noncanonical mode
	 *
	 * min time
	 * min bytes to read
	 */

//	terminalAttributes.c_lflag = 0;//ICANON;
	terminalAttributes.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	terminalAttributes.c_cc[VTIME] = 5;
	terminalAttributes.c_cc[VMIN] = 1;

  cfsetospeed(&terminalAttributes,B19200);            // 19200 baud
  cfsetispeed(&terminalAttributes,B19200);            // 19200 baud
	/*
	 * http://linux.die.net/man/3/tcsetattr
	 * Set the port to our state
	 *
	 * the change occurs immediately
	 */

	tcsetattr(fileDescriptor, TCSANOW, &terminalAttributes);

	/*
	 * http://linux.die.net/man/3/tcflush
	 *
	 * flushes data written but not transmitted.
	 * flushes data received but not read.
	 */

//	tcflush(fileDescriptor, TCOFLUSH);
//	tcflush(fileDescriptor, TCIFLUSH);

	return fileDescriptor;
}

void SerialPort::disconnect(void)
{
    close(fileDescriptor);
    printf("\nPort 1 has been CLOSED and %d is the file description\n", fileDescriptor);
}

int SerialPort::sendArray(unsigned char *buffer, int len) {
	int n=write(fileDescriptor, buffer, len);
	return n;
}

int SerialPort::getArray (unsigned char *buffer, int len)
{
	int n1=bytesToRead();
	int n=read(fileDescriptor, buffer, n1);
	cout << "len=" << len << " n1=" << n1 << " n=" << n << endl;
	return n;
}

void SerialPort::clear()
{
	tcflush(fileDescriptor, TCIFLUSH);
	tcflush(fileDescriptor, TCOFLUSH);
}

int SerialPort::bytesToRead()
{
	int bytes=0;
	ioctl(fileDescriptor, FIONREAD, &bytes);

	return bytes;
}

int main(int argc, char * argv[])
{
  SerialPort serialPort;
  serialPort.connect();
//  serialPort.clear();
  char* out = "TEST\n";
  char in[100];
  serialPort.sendArray((unsigned char*)out, strlen(out));
  serialPort.getArray((unsigned char*)in, 7);
  for(int i = 0; i < strlen(in); i++) {
    cout << in[i] << ".";
  }
  cout << "\nThe end\n";
  serialPort.disconnect();
  return 0;
}
