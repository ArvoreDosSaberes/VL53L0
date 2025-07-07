#include "vl53l0x_api_core.h"
#include "vl53l0x_platform.h"

VL53L0X_Error VL53L0X_WriteReg(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t value) {
    return VL53L0X_WriteMultiPlatform(pdev, reg, &value, 1);
}

VL53L0X_Error VL53L0X_ReadReg(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t *value) {
    return VL53L0X_ReadMultiPlatform(pdev, reg, value, 1);
}

VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t *pdata, uint32_t count) {
    return VL53L0X_WriteMultiPlatform(pdev, reg, pdata, count);
}

VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_Dev_t *pdev, uint16_t reg, uint8_t *pdata, uint32_t count) {
    return VL53L0X_ReadMultiPlatform(pdev, reg, pdata, count);
}
