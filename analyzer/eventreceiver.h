#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

class EventReceiver
{
public:
    enum event_t {progress_event};
    //union eventarg_t {double d; int i;};

    //EventReceiver();
    virtual void RaiseEvent(event_t event,int arg) = 0;

private:
};

#endif // EVENTRECEIVER_H
