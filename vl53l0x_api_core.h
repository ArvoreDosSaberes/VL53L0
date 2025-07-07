#ifndef VL53L0X_API_CORE_H
#define VL53L0X_API_CORE_H

#include <stdint.h>
#include "vl53l0x_types.h"
#include "vl53l0x_def.h"

#ifdef __cplusplus
extern "C" {
#endif

// Single register write
VL53L0X_Error VL53L0X_WriteReg(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t value);
// Single register read
VL53L0X_Error VL53L0X_ReadReg(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t *value);
// Multiple bytes write
VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t *pdata, uint32_t count);
// Multiple bytes read
VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t *pdata, uint32_t count);

#ifdef __cplusplus
}
#endif

#endif // VL53L0X_API_CORE_H
