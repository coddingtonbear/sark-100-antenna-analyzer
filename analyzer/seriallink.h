#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QString>

//#define DUMMY_DEV

#define FMIN 1000000
#define FMAX 60000000

class SerialLink
{
public:
    SerialLink(const char *dev,int speed);
    ~SerialLink();

    void TxCmd(char *cmd);
    int RxLine();
    int RxFlush();

    bool IsUp();
    void Cmd_Scan(long fstart, long fend, long fstep, bool useraw, EventReceiver *erx);
    void Cmd_Off(EventReceiver *erx);

    QString devname;

protected:
    int devfd;

    char rxbuff[128];
    int rxbufflen;

private:
#ifdef DUMMY_DEV
    const char *dummy_data;
    int dummy_pos;
#endif
};

#endif // SERIALLINK_H
