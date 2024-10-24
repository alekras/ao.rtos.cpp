#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc,char** argv){
    struct termios tio;
    struct termios stdio;
    int tty_fd;
    fd_set rdset;

    unsigned char c='D';

    printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open("/dev/ttyS0", O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio,B19200);            // 115200 baud
    cfsetispeed(&tio,B19200);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&tio);

    char str[] = {'V','E','R','\r', '\n', 0x0};
    //the above str[] doesn't work although it's exactly the same as the following
    //char str[] = {0x56, 0x45, 0x52, 0x0D};
    write(tty_fd,str,strlen(str));
    if (read(tty_fd,&c,1)>0)
        write(STDOUT_FILENO,&c,1);

    while (c!='q')
    {
            if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1); // if new data is available on the serial port, print it out
            if (read(STDIN_FILENO,&c,1)>0)
                if(c!='q')
                    write(tty_fd,&c,1);        // if new data is available on the console, send it to the serial port
    }

    close(tty_fd);
    return 0;
}

