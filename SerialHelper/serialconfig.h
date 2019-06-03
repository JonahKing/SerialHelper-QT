#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H


class SerialConfig
{
public:
    bool serial_open_status;
    bool stop_display;

    bool send_data_timming1;
    bool send_data_timming2;
    bool send_data_timming3;
    bool allow_joint_net;
    int current_timming_index;

    int receive_frame_duration;
    SerialConfig();

};

#endif // SERIALCONFIG_H
