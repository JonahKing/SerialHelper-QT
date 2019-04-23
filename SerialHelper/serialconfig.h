#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H


class SerialConfig
{
public:
    bool serial_open_status;
    bool stop_display;
    int receive_frame_duration;
    SerialConfig();

};

#endif // SERIALCONFIG_H
