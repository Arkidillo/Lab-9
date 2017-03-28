Skip to content
This repository
Search
Pull requests
Issues
Gist
 @Arkidillo
 Sign out
 Unwatch 3
  Star 0
 Fork 0 gw-cs3410-s17/final-project-devin-sarah Private
 Code  Issues 0  Pull requests 0  Projects 0  Wiki  Pulse  Graphs
Branch: master Find file Copy pathfinal-project-devin-sarah/ArduinoCommunicator.c
11af8fa  a day ago
@Arkidillo Arkidillo Show X can now send up to 2^16-1 heartrate rather than just up to 128…
1 contributor
RawBlameHistory     
176 lines (148 sloc)  4.55 KB
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>
#include <math.h>

int init_tty(int fd);

/**
 * Run this program and pass the location of the serial file descriptor as an argument
 * On linux, this is likely /dev/ttyACM0
 * On OSX, this may be similar to /dev/cu.usbmodem1D1131
 */

int
main(int argc, char **argv) {
    int fd;
    int count;
    char *device;
    char *destination;
    char byte;

    /*
     * Read the device path from input,
     * or default to /dev/ttyACM0
     */
    if (argc == 2) {
        device = argv[1];
    } else {
        device = "/dev/ttyACM0";
    }
    printf("Connecting to %s\n", device);

    /* Get file name */
    if (argc == 3){
	destination = argv[2];	
    }
    printf("Revieved file %s\n", device);

    /*
     * Need the following flags to open:
     * O_RDWR: to read from/write to the devices
     * O_NOCTTY: Do not become the process's controlling terminal
     * O_NDELAY: Open the resource in nonblocking mode
     */
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Error opening serial");
        return -1;
    }

    /* Configure settings on the serial port */
    if (init_tty(fd) == -1) {
        perror("init");
        close(fd);
        return -1;
    }

    /* Wait for Arduino to reboot */
    sleep(1);

    /* Flush whatever is remaining in the buffer */
    tcflush(fd, TCIFLUSH);

    char in[255];
    char *delim = malloc(sizeof(char));
    *delim = ' ';
    ssize_t readsize;
    while(1){
        readsize = read(fd, in, sizeof(in));
    }

    close(fd);
    return 0;
}

int
init_tty(int fd) {
    struct termios tty;
    /*
     * Configure the serial port.
     * First, get a reference to options for the tty
     * Then, set the baud rate to 9600 (same as on Arduino)
     */
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) == -1) {
        perror("tcgetattr");
        return -1;
    }

    if (cfsetospeed(&tty, (speed_t)B9600) == -1) {
        perror("ctsetospeed");
        return -1;
    }
    if (cfsetispeed(&tty, (speed_t)B9600) == -1) {
        perror("ctsetispeed");
        return -1;
    }

    // 8 bits, no parity, no stop bits
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    // No flow control
    tty.c_cflag &= ~CRTSCTS;

    // Set local mode and enable the receiver
    tty.c_cflag |= (CLOCAL | CREAD);

    // Disable software flow control
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Make raw
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;

    // Infinite timeout and return from read() with >1 byte available
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 0;

    // Update options for the port
    if (tcsetattr(fd, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}
Contact GitHub API Training Shop Blog About
© 2017 GitHub, Inc. Terms Privacy Security Status Help