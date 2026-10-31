#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <stdbool.h>

typedef enum
{
    STATE_OFF,
    STATE_IDLE,
    STATE_HEATING,
    STATE_ERROR
} SystemState_t;

typedef struct
{
    SystemState_t state;
    float temp_current;
    float temp_target;
    bool sensor_error;
} SystemData_t;

#endif
