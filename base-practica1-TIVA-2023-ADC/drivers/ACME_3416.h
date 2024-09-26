/*
 * ACME_3416.h
 *
 *  Created on: 12 jun. 2019
 *      Author: Embedded Systems UMA
 */

#ifndef ACME_3416_H_
#define ACME_3416_H_

#include"drivers/i2cm_drv.h"

extern int32_t ACME_initDevice(tI2CMInstance *instance,uint8_t address);
extern int32_t ACME_setPinDir (uint8_t dir);
extern int32_t ACME_writePin (uint8_t pin);
extern int32_t ACME_readPin (uint8_t *pin);
extern int32_t ACME_clearInt (uint8_t pin);
extern int32_t ACME_setIntType (uint8_t regA,uint8_t regB);
extern int32_t ACME_readInt (uint8_t *pin);
extern int32_t ACME_read (uint8_t *bytes,uint8_t size);
#endif /* ACME_3416_H_ */
