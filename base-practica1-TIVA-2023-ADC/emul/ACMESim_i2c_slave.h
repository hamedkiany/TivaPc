/*
 * i2c_slave.h
 *
 *  Created on: Jun 7, 2019
 *      Author: cano
 */

#ifndef ACMESIM_I2C_SLAVE_H_
#define ACMESIM_I2C_SLAVE_H_

void ACMEsim_I2C1SlaveIntHandler(void);
void ACMEsim_InitSlave (void);
void ACMEsim_SlaveEnable (void);
void ACMEsim_SlaveDisable (void);

#endif /* ACMESIM_I2C_SLAVE_H_ */
