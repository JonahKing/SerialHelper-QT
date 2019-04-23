#include "serialconfig.h"

SerialConfig::SerialConfig()
{
    serial_open_status  = false;
    stop_display        = false;
    receive_frame_duration = 100;
}
