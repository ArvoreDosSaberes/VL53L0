#include "vl53l0x_api_ranging.h"
#include "vl53l0x_api.h"
#include "vl53l0x_api_core.h"

VL53L0X_Error VL53L0X_PerformSingleRanging(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *data) {
    uint16_t distance = 0;
    VL53L0X_Error err = VL53L0X_GetDistance(pdev, &distance);
    data->RangeMilliMeter = distance;
    data->Status = (uint8_t)(err != VL53L0X_ERROR_NONE);
    return err;
}
