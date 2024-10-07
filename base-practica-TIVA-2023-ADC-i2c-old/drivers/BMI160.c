/* SISTEMAS EMPOTRADOS UMA
    Este código está basado parcialmente en una biblioteca implementada para otros dispositivos cuyo copyright se copia más abajo.
    Los desarrolladores de dicha biblioteca NO SON responsables de los problemas que esta modificación pueda ocasionar
    */
/*
===============================================
BMI160 accelerometer/gyroscope library for Intel(R) Curie(TM) devices.
Copyright (c) 2015 Intel Corporation.  All rights reserved.

Based on MPU6050 Arduino library provided by Jeff Rowberg as part of his
excellent I2Cdev device library: https://github.com/jrowberg/i2cdevlib

===============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>

#include "BMI160.h"

#include "FreeRTOS.h"
#include "task.h"

#include "drivers/i2cm_drv.h"

#include "utils/uartstdio.h"

#define BMI160_CHIP_ID 0xD1

#define BMI160_ACCEL_POWERUP_DELAY_MS 10
#define BMI160_GYRO_POWERUP_DELAY_MS 100

/* Test the sign bit and set remaining MSBs if sign bit is set */
#define BMI160_SIGN_EXTEND(val, from) \
    (((val) & (1 << ((from) - 1))) ? (val | (((1 << (1 + (sizeof(val) << 3) - (from))) - 1) << (from))) : val)


typedef struct
{
    //
    // The pointer to the I2C master interface instance used to communicate
    // with the SHT21.
    //
    tI2CMInstance *psI2CInst;

    //
    // the I2C address of the SHT21.
    //
    uint8_t ui8Addr;
} BMI160_drv;

static BMI160_drv BMI160_data;

static volatile int semaforo_malo=0;
void BMI160AppCallback(void *pvCallbackData, uint_fast8_t ui8Status)
{
    if(ui8Status != I2CM_STATUS_SUCCESS)
    {

    }

    semaforo_malo=1;
}

int32_t BMI160_i2c_transfer(uint8_t *tx_buf, unsigned tx_cnt, uint8_t *rx_buf, unsigned rx_cnt)
{
    semaforo_malo=0;

    I2CMCommand(BMI160_data.psI2CInst,BMI160_data.ui8Addr, tx_buf , tx_cnt, tx_cnt, rx_buf,rx_cnt ,
                rx_cnt, BMI160AppCallback, NULL);

    while(!semaforo_malo) {vTaskDelay(10);} //XXX CAMBIAR EP33!!!!

    return 0; //XXX CAMBIAR EP33!!!!
}






/******************************************************************************/

uint8_t BMI160_reg_read (uint8_t reg)
{
    uint8_t buffer[1];
    buffer[0] = reg;
    if (BMI160_i2c_transfer(buffer, 1, buffer, 1)<0) UARTprintf("Read failed\n\r");
    return buffer[0];
}

void BMI160_reg_write(uint8_t reg, uint8_t data)
{
    uint8_t buffer[2];
    buffer[0] = reg;
    buffer[1] = data;
    if (BMI160_i2c_transfer(buffer, 2, NULL, 0)<0) UARTprintf("Write failed\n\r");
}

void BMI160_reg_write_bits(uint8_t reg, uint8_t data, unsigned pos, unsigned len)
{
    uint8_t b = BMI160_reg_read(reg);
    uint8_t mask = ((1 << len) - 1) << pos;
    data <<= pos; // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte
    BMI160_reg_write(reg, b);
}

uint8_t BMI160_reg_read_bits(uint8_t reg, unsigned pos, unsigned len)
{
    uint8_t b = BMI160_reg_read(reg);
    uint8_t mask = (1 << len) - 1;
    b >>= pos;
    b &= mask;
    return b;
}

/******************************************************************************/

/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to default range settings, namely +/- 2g and +/- 250 degrees/sec.
 */

void BMI160_initialize(tI2CMInstance *instance,uint8_t address)
{

    BMI160_data.psI2CInst=instance;
    BMI160_data.ui8Addr=address;

    /* Issue a soft-reset to bring the device into a clean state */
    BMI160_reg_write(BMI160_RA_CMD, BMI160_CMD_SOFT_RESET);
    vTaskDelay(1);

//    /* Issue a dummy-read to force the device into I2C comms mode */
    BMI160_reg_read(0x7F);
    vTaskDelay(100);

    /* Power up the accelerometer */
    BMI160_reg_write(BMI160_RA_CMD, BMI160_CMD_ACC_MODE_NORMAL);
    vTaskDelay(1000);
    /* Wait for power-up to complete */
    while (0x1 != BMI160_reg_read_bits(BMI160_RA_PMU_STATUS,
                                BMI160_ACC_PMU_STATUS_BIT,
                                BMI160_ACC_PMU_STATUS_LEN))
    {
        vTaskDelay(1);
    }

    /* Power up the gyroscope */
    BMI160_reg_write(BMI160_RA_CMD, BMI160_CMD_GYR_MODE_NORMAL);
    //vTaskDelay(1);
    /* Wait for power-up to complete */
    while (0x1 != BMI160_reg_read_bits(BMI160_RA_PMU_STATUS,
                                BMI160_GYR_PMU_STATUS_BIT,
                                BMI160_GYR_PMU_STATUS_LEN))
        //vTaskDelay(1);

    BMI160_setFullScaleGyroRange(BMI160_GYRO_RANGE_250);
    BMI160_setFullScaleAccelRange(BMI160_ACCEL_RANGE_2G);

    /* Only PIN1 interrupts currently supported - map all interrupts to PIN1 */
    BMI160_reg_write(BMI160_RA_INT_MAP_0, 0xFF);
    BMI160_reg_write(BMI160_RA_INT_MAP_1, 0xF0);
    BMI160_reg_write(BMI160_RA_INT_MAP_2, 0x00);
}

/** Get Device ID.
 * This register is used to verify the identity of the device (0b11010001, 0xD1).
 * @return Device ID (should be 0xD1)
 * @see BMI160_RA_CHIP_ID
 */
uint8_t BMI160_getDeviceID() {
    return BMI160_reg_read(BMI160_RA_CHIP_ID);
}

/** Verify the SPI connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool BMI160_testConnection()
{
    return (BMI160_CHIP_ID == BMI160_getDeviceID());
}







/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleGyroRange()
 */
void BMI160_setFullScaleGyroRange(uint8_t range) {
    // Para implementar
}

/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 * @see BMI160AccelRange
 */
void BMI160_setFullScaleAccelRange(uint8_t range) {
    //Para implementar
}






/** Get 3-axis accelerometer readings.
 * These registers store the most recent accelerometer measurements.
 */
void BMI160_getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t buffer[6];
    buffer[0] = BMI160_RA_ACCEL_X_L;
    BMI160_i2c_transfer(buffer, 1, buffer, 6);
    *x = (((int16_t)buffer[1]) << 8) | buffer[0];
    *y = (((int16_t)buffer[3]) << 8) | buffer[2];
    *z = (((int16_t)buffer[5]) << 8) | buffer[4];
}




/** Get 3-axis gyroscope readings.
 */
void BMI160_getRotation(int16_t* x, int16_t* y, int16_t* z) {
    //POR COMPLETAR....
}



/** Read a BMI160 register directly.
 * @param reg register address
 * @return 8-bit register value
 */
uint8_t BMI160_getRegister(uint8_t reg) {
    return BMI160_reg_read(reg);
}

/** Write a BMI160 register directly.
 * @param reg register address
 * @param data 8-bit register value
 */
void BMI160_setRegister(uint8_t reg, uint8_t data) {
    BMI160_reg_write(reg, data);
}
