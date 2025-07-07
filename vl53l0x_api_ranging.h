#ifndef VL53L0X_API_RANGING_H
#define VL53L0X_API_RANGING_H

#include "vl53l0x_types.h"
#include "vl53l0x_def.h"

#ifdef __cplusplus
extern "C" {
#endif

// Ranging measurement data type
typedef struct {
    uint16_t RangeMilliMeter;
    uint8_t Status;
} VL53L0X_RangingMeasurementData_t;

// Perform single shot ranging
VL53L0X_Error VL53L0X_PerformSingleRanging(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *data);

#ifdef __cplusplus
}
#endif

#endif // VL53L0X_API_RANGING_H
