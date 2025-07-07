#ifndef VL53L0X_TYPES_H
#define VL53L0X_TYPES_H

#include <stdint.h>
#include <hardware/i2c.h>

// Device descriptor type
typedef struct {
    i2c_inst_t *i2c;   // I2C interface
    uint8_t address;    // 7-bit I2C address
} VL53L0X_Dev_t;

#endif // VL53L0X_TYPES_H
