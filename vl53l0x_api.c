#include "vl53l0x_api.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_platform_log.h"
#include "vl53l0x_registers.h"

VL53L0X_Error VL53L0X_SetDeviceAddress(VL53L0X_Dev_t *pdev, uint8_t new_address) {
    // The address given is the 7-bit I2C address
    return VL53L0X_WriteReg(pdev, I2C_SLAVE_DEVICE_ADDRESS, new_address);
}

VL53L0X_Error VL53L0X_InitDevice(VL53L0X_Dev_t *pdev) {
    VL53L0X_Error err;

    // Check model ID
    uint8_t model_id = 0;
    err = VL53L0X_ReadReg(pdev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &model_id);
    if (err != VL53L0X_ERROR_NONE) return err;
    if (model_id != VL53L0X_MODEL_ID) return (VL53L0X_Error)(-1);

    // Minimal initialization sequence
    VL53L0X_WriteReg(pdev, 0x88, 0x00);
    VL53L0X_WriteReg(pdev, 0x80, 0x01);
    VL53L0X_WriteReg(pdev, 0xFF, 0x01);
    VL53L0X_WriteReg(pdev, 0x00, 0x00);
    uint8_t stop_variable = VL53L0X_ReadReg(pdev, 0x91, &stop_variable);
    VL53L0X_WriteReg(pdev, 0x00, 0x01);
    VL53L0X_WriteReg(pdev, 0xFF, 0x00);
    VL53L0X_WriteReg(pdev, 0x80, 0x00);

    // Set high speed mode
    VL53L0X_WriteReg(pdev, 0x80, 0x01);
    VL53L0X_WriteReg(pdev, 0xFF, 0x01);
    VL53L0X_WriteReg(pdev, 0x00, 0x00);
    VL53L0X_WriteReg(pdev, 0x91, stop_variable);
    VL53L0X_WriteReg(pdev, 0x00, 0x01);
    VL53L0X_WriteReg(pdev, 0xFF, 0x00);
    VL53L0X_WriteReg(pdev, 0x80, 0x00);

    // Set default ranging mode
    VL53L0X_WriteReg(pdev, VL53L0X_REG_SYSRANGE_START, 0x01);

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
