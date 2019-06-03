#include "serialconfig.h"

SerialConfig::SerialConfig()
{
    serial_open_status  = false;
    stop_display        = false;
    receive_frame_duration = 100;
    send_data_timming1 = false;
    send_data_timming2 = false;
    send_data_timming3 = false;
    allow_joint_net = false;
}
