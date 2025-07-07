#include "vl53l0x_api.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_platform_log.h"
#include "vl53l0x_registers.h"

VL53L0X_Error VL53L0X_InitDevice(VL53L0X_Dev_t *pdev) {
    uint8_t model_id = 0;
    VL53L0X_Error err = VL53L0X_ReadReg(pdev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &model_id);
    if (err != VL53L0X_ERROR_NONE) {
        VL53L0X_Log("Error reading model ID: %d\n", err);
        return err;
    }
    if (model_id != VL53L0X_MODEL_ID) {
        VL53L0X_Log("Unexpected model ID: 0x%02X\n", model_id);
        return (VL53L0X_Error)(-1);
    }
    // Additional default configuration can be added here
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_StartRanging(VL53L0X_Dev_t *pdev) {
    // Start continuous ranging mode
    return VL53L0X_WriteReg(pdev, VL53L0X_REG_SYSRANGE_START, 0x03);
}

VL53L0X_Error VL53L0X_GetDistance(VL53L0X_Dev_t *pdev, uint16_t *distance) {
    VL53L0X_Error err = VL53L0X_WriteReg(pdev, VL53L0X_REG_SYSRANGE_START, 0x01);
    if (err != VL53L0X_ERROR_NONE) {
        VL53L0X_Log("Failed to start range: %d\n", err);
        return err;
    }
    uint32_t start_tick = VL53L0X_GetTick();
    uint8_t status = 0;
    do {
        err = VL53L0X_ReadReg(pdev, VL53L0X_REG_RESULT_INTERRUPT_STATUS, &status);
        if (err != VL53L0X_ERROR_NONE) {
            VL53L0X_Log("Error reading interrupt status: %d\n", err);
            return err;
        }
    } while (((status & 0x07) == 0) && ((VL53L0X_GetTick() - start_tick) < 500));
    if ((status & 0x07) == 0) {
        VL53L0X_Log("Timeout waiting for range ready\n");
        return (VL53L0X_Error)(-1);
    }
    uint8_t hi = 0, lo = 0;
    err = VL53L0X_ReadReg(pdev, VL53L0X_REG_RESULT_RANGE_VALUE_HI, &hi);
    if (err != VL53L0X_ERROR_NONE) return err;
    err = VL53L0X_ReadReg(pdev, VL53L0X_REG_RESULT_RANGE_VALUE_LO, &lo);
    if (err != VL53L0X_ERROR_NONE) return err;
    *distance = ((uint16_t)hi << 8) | lo;
    err = VL53L0X_WriteReg(pdev, VL53L0X_REG_SYSTEM_INTERRUPT_CLEAR, 0x01);
    if (err != VL53L0X_ERROR_NONE) {
        VL53L0X_Log("Failed to clear interrupt: %d\n", err);
        return err;
    }
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_StopRanging(VL53L0X_Dev_t *pdev) {
    // Stop continuous ranging mode
    return VL53L0X_WriteReg(pdev, VL53L0X_REG_SYSRANGE_START, 0x00);
}
