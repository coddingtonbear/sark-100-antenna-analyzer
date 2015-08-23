#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <poll.h>

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "scandata.h"
#include "seriallink.h"

extern ScanData scandata;

SerialLink::SerialLink(const char *dev,int speed)
{
    rxbufflen=0;

printf("SerialLink::Init: dev %s\n",dev);
    devname = dev;
    devfd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (devfd != -1)
    {
        struct termios termattr;
        tcgetattr(devfd, &termattr);
        cfmakeraw(&termattr);
printf("\tc_cflag=%x\n",termattr.c_cflag);
        termattr.c_oflag &= ~(ONLCR);
        termattr.c_cflag &= ~(CRTSCTS|PARENB|CSTOPB|CBAUD);
        termattr.c_cflag |= CS8 | CLOCAL;
printf("\tc_cflag=%x\n",termattr.c_cflag);
        switch (speed)
        {
          case 300: termattr.c_cflag |= B300; break;
          case 600: termattr.c_cflag |= B600; break;
          case 1200: termattr.c_cflag |= B1200; break;
          case 2400: termattr.c_cflag |= B2400; break;
          case 4800: termattr.c_cflag |= B4800; break;
          case 9600: termattr.c_cflag |= B9600; break;
          case 19200: termattr.c_cflag |= B19200; break;
          case 38400: termattr.c_cflag |= B38400; break;
          case 57600: termattr.c_cflag |= B57600; break;
          case 115200: termattr.c_cflag |= B115200; break;
        }
printf("\tc_cflag=%x\n",termattr.c_cflag);
        tcsetattr(devfd, TCSANOW, &termattr);
    }
printf("\tdevfd %d\n",devfd);

#ifdef DUMMY_DEV
    dummy_data = "";
    dummy_pos = 0;
#endif
}

SerialLink::~SerialLink()
{
  if (devfd != -1)
    close(devfd);
}

#ifdef DUMMY_DEV

void SerialLink::TxCmd(char *cmd)
{
printf("TxCmd: %s\n",cmd);
    if (strncmp(cmd,"scan ",5)==0)
    {
        dummy_data = "Start\n\
5.1,105,0,105\n\
6.336,102.55,0,102.55\n\
7.451,95.45,0,95.45\n\
8.336,84.39,0,84.39\n\
8.904,70.45,0,70.45\n\
9.1,55,0,55\n\
8.904,39.55,0,39.55\n\
8.336,25.61,0,25.61\n\
7.451,14.55,0,14.55\n\
6.336,7.45,0,7.45\n\
5.1,5,0,5\n\
3.864,7.45,0,7.45\n\
2.749,14.55,0,14.55\n\
1.864,25.61,0,25.61\n\
1.296,39.55,0,39.55\n\
1.1,55,0,55\n\
1.296,70.45,0,70.45\n\
1.864,84.39,0,84.39\n\
2.749,95.45,0,95.45\n\
3.864,102.55,0,102.55\n\
5.1,105,0,105\n\
End\n";
    }
    else
      dummy_data = "OK\n";
    dummy_pos = 0;
}

int SerialLink::RxLine()
{
    for (rxbufflen=0; dummy_data[dummy_pos]; dummy_pos++)
    {
        switch (dummy_data[dummy_pos])
        {
        case '\n':
            rxbuff[rxbufflen]='\0';
            dummy_pos++;
            return 0;
        case '\0':
            rxbuff[rxbufflen]='\0';
            return 3;
        default:
            rxbuff[rxbufflen++]=dummy_data[dummy_pos];
        }
    }
    return 3;
}

#else

int SerialLink::RxFlush()
{

    pollfd fds[] = {{devfd, POLLIN, 0 }};
    int timeout = 500;    //500msec, move somewhere
    int pstatus,rstatus;

    for (rxbufflen=0;;)
    {
        pstatus = poll(fds, 1, timeout);
        if (pstatus > 0)                                        //Success
        {
            if (fds[0].revents & (POLLERR|POLLHUP|POLLNVAL))
                return 3;   //Error, eof, hup etc quit
            else
                if (fds[0].revents & POLLIN)					// There's some data to read
                {
                    rstatus = read(devfd, rxbuff+rxbufflen, 1);
                    if (rstatus == -1)
                    {
                        if (errno == EAGAIN)					// Treat would block error as a timeout
                            return 1;   //Timeout
                        else
                            return 4;   //Error reading
                    }
                    else
                    {
printf("RxFlush: %c\n",rxbuff[rxbufflen]);
                    }
                }
        }
        else if (pstatus == 0)                                  //Timeout
            return 1;
        else if (pstatus == -1)                                 //Error polling
            return 2;
    }
}

void SerialLink::TxCmd(char *cmd)
{
    if (devfd != -1)
    {
printf("TxCmd: %s\n",cmd);
        write(devfd, cmd, strlen(cmd));
        write(devfd, "\n", 1);
    }
}

int SerialLink::RxLine()
{

    pollfd fds[] = {{devfd, POLLIN, 0 }};
    int timeout = 10000;    //10sec, move somewhere
    int pstatus,rstatus;

    for (rxbufflen=0;;)
    {
        pstatus = poll(fds, 1, timeout);
        if (pstatus > 0)                                        //Success
        {
            if (fds[0].revents & (POLLERR|POLLHUP|POLLNVAL))
                return 3;   //Error, eof, hup etc quit
            else
                if (fds[0].revents & POLLIN)					// There's some data to read
                {
                    rstatus = read(devfd, rxbuff+rxbufflen, 1);
                    if (rstatus == -1)
                    {
                        if (errno == EAGAIN)					// Treat would block error as a timeout
                            return 1;   //Timeout
                        else
                            return 4;   //Error reading
                    }
                    else
                    {
//printf("RxLine: c=%c (0x%02x)\n",rxbuff[rxbufflen],rxbuff[rxbufflen]);
                        switch (rxbuff[rxbufflen])
                        {
                        case '\r':  break;
                        case '\n':
                            rxbuff[rxbufflen] = '\0';
                            return 0;
                        default:
                            rxbufflen += rstatus;
                            break;
                        }
                    }
                }
        }
        else if (pstatus == 0)                                  //Timeout
            return 1;
        else if (pstatus == -1)                                 //Error polling
            return 2;
    }
}

#endif

bool SerialLink::IsUp()
{
   return devfd != -1;
}

void SerialLink::Cmd_Scan(long fstart, long fend, long fstep, bool useraw, EventReceiver *erx)
{
    char cmd[64];
    int state = 0,r;
    Sample sample;
    long frequency = fstart;

    //RxFlush();

    sprintf(cmd,"%s %ld %ld %ld\r", useraw?"scanr":"scan", fstart, fend>=FMAX ? FMAX : (fend+1), fstep);     //Add 1Hz to end freq to make it inclusive
    TxCmd(cmd);

    scandata.points.resize(0);

    for (;state<10;)
    {
        r = RxLine();
printf("RxLine=%d\n",r);
        if (r==0)
        {
printf("RxLine: %s (%d)\n",rxbuff,rxbufflen);
printf("state: %d\n",state);
            switch (state)
            {
            case 0:
                if (strcmp(rxbuff,"Start")==0)
                {
                    erx->RaiseEvent(EventReceiver::progress_event, 0);
                    state=1;    //Results
                }
                else if (strncmp(rxbuff,"Error:",5)==0)
                {
                    state=10;   //Error
                }
                else
                    state=0;   //Garbage ? Ignore
                break;

            case 1:
                if (strcmp(rxbuff,"End")==0)
                {
                    erx->RaiseEvent(EventReceiver::progress_event, 100);
                    state=10;   //Done
                }
                else
                {
printf("f=%ld\n",frequency);
                    sample.freq = (double)(frequency);
                    if (useraw)
                    {
                        double vf,vr,vz,va;
                        sscanf(rxbuff,"%lf,%lf,%lf,%lf",&vf,&vr,&vz,&va);
                        sample.fromRaw(vf,vr,vz,va);
                    }
                    else
                    {
                        sscanf(rxbuff,"%lf,%lf,%lf,%lf",&sample.swr,&sample.R,&sample.X,&sample.Z);
                    }

                    scandata.points.push_back(sample);

                    frequency += fstep;
                    erx->RaiseEvent(EventReceiver::progress_event, (int)((frequency-fstart)*100/(fend-fstart)));
                }
                break;
            }
        }
        else
            state=10;   //Timeout
    }

    scandata.UpdateStats();
}

void SerialLink::Cmd_Off(EventReceiver *erx)
{
    int state = 0,r;

    //RxFlush();

    TxCmd("off\r");

    for (;state<10;)
    {
        r = RxLine();
printf("RxLine=%d\n",r);
        if (r==0)
        {
printf("RxLine: %s (%d)\n",rxbuff,rxbufflen);
printf("state: %d\n",state);
            switch (state)
            {
            case 0:
                if (strcmp(rxbuff,"OK")==0)
                {
                    erx->RaiseEvent(EventReceiver::progress_event, 100);
                    state=10;    //Results
                }
                else if (strncmp(rxbuff,"Error:",5)==0)
                {
                    state=10;   //Error
                }
                else
                    state=0;   //Garbage ? Ignore
                break;
            }
        }
        else
            state=10;   //Timeout
    }
}
