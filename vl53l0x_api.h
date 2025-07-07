#ifndef VL53L0X_API_H
#define VL53L0X_API_H

#include "vl53l0x_types.h"
#include "vl53l0x_def.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the device
VL53L0X_Error VL53L0X_InitDevice(VL53L0X_Dev_t *pdev);

// Start continuous ranging
VL53L0X_Error VL53L0X_StartRanging(VL53L0X_Dev_t *pdev);

// Get a single distance measurement in millimeters
VL53L0X_Error VL53L0X_GetDistance(VL53L0X_Dev_t *pdev, uint16_t *distance);

// Stop continuous ranging
VL53L0X_Error VL53L0X_StopRanging(VL53L0X_Dev_t *pdev);

#ifdef __cplusplus
}
#endif

#endif // VL53L0X_API_H
