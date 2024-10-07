/*
 * ACME_3416.c
 *
 *  Created on: 12 jun. 2019
 *      Author: Embedded Systems UMA
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "drivers/i2cm_drv.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define ACME_ID_REGISTER 0
#define ACME_DIR_REGISTER 1
#define ACME_OUTPUT_REGISTER 2
#define ACME_INT_TYPE_REGISTER_A 3
#define ACME_INT_TYPE_REGISTER_B 4
#define ACME_INT_CLEAR_REGISTER 5
#define ACME_INPUT_REGISTER 6
#define ACME_INT_STATUS 7

#define ACME_ID_REGISTER_VALUE 0xB6




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
} ACME_drv;

static ACME_drv ACME_data;

//static volatile int semaforo_malo=0;
//TaskHandle_t taskHandleACME;


void ACMEAppCallback(void *pvCallbackData, uint_fast8_t ui8Status)
{
//    if(ui8Status != I2CM_STATUS_SUCCESS)
//    {
//
//    }
//
//    semaforo_malo=1;
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       //vTaskNotifyGiveFromISR(taskHandleACME, &xHigherPriorityTaskWoken);

       if (ui8Status != I2CM_STATUS_SUCCESS)
       {
           xTaskNotifyFromISR(pvCallbackData, ui8Status, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
       }
       else
       {
           xTaskNotifyFromISR(pvCallbackData, I2CM_STATUS_SUCCESS, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
       }

       portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int32_t ACME_WriteAndRead(uint8_t *ptrtotxdata,uint8_t txcount, uint8_t *ptrtorxdata,uint8_t rxcount)
{
//    semaforo_malo=0;
/*    if(taskHandleACME == NULL)
        taskHandleACME = xTaskGetCurrentTaskHandle();*/
    I2CMCommand(ACME_data.psI2CInst,ACME_data.ui8Addr, ptrtotxdata , txcount, txcount, ptrtorxdata,rxcount ,
                rxcount, ACMEAppCallback, xTaskGetCurrentTaskHandle());
    int32_t notificationValue;
        if (xTaskNotifyWait(0x00, 10, &notificationValue, portMAX_DELAY) == pdTRUE)
        {
            if (notificationValue != I2CM_STATUS_SUCCESS)
            {
                return -1; // Error in I2C transaction
            }
        }
        else
        {
            return -1; // Notification wait failed
        }

//    while(!semaforo_malo) {vTaskDelay(10);} //XXX CAMBIAR EP33!!!!

    return notificationValue; //XXX CAMBIAR EP33!!!!
}

int32_t ACME_Write(uint8_t *ptrtotxdata,uint8_t txcount)
{
//    semaforo_malo=0;
//    if(taskHandleACME == NULL)
//        taskHandleACME = xTaskGetCurrentTaskHandle();
     I2CMCommand(ACME_data.psI2CInst,ACME_data.ui8Addr, ptrtotxdata , txcount, txcount, NULL,0 ,
                    0, ACMEAppCallback, xTaskGetCurrentTaskHandle());
     int32_t notificationValue;
         if (xTaskNotifyWait(0x00, 10, &notificationValue, portMAX_DELAY) == pdTRUE)
         {
             if (notificationValue != I2CM_STATUS_SUCCESS)
             {
                 return -1; // Error in I2C transaction
             }
         }
         else
         {
             return -1; // Notification wait failed
         }

 //    while(!semaforo_malo) {vTaskDelay(10);} //XXX CAMBIAR EP33!!!!

     return notificationValue; //XXX CAMBIAR EP33!!!!
}

int32_t ACME_Read(uint8_t *ptrtorxdata,uint8_t rxcount)
{
//    semaforo_malo=0;
//    if(taskHandleACME == NULL)
//        taskHandleACME = xTaskGetCurrentTaskHandle();

    I2CMCommand(ACME_data.psI2CInst,ACME_data.ui8Addr, NULL , 0, 0, ptrtorxdata,rxcount ,
                    rxcount, ACMEAppCallback, xTaskGetCurrentTaskHandle());
    int32_t notificationValue;
        if (xTaskNotifyWait(0x00, 10, &notificationValue, portMAX_DELAY) == pdTRUE)
        {
            if (notificationValue != I2CM_STATUS_SUCCESS)
            {
                return -1; // Error in I2C transaction
            }
        }
        else
        {
            return -1; // Notification wait failed
        }

//    while(!semaforo_malo) {vTaskDelay(10);} //XXX CAMBIAR EP33!!!!

    return notificationValue; //XXX CAMBIAR EP33!!!!
}


//INICIALIZA EL DISPOSITIVO
//Esta funcion debe comprobar si el sensor esta o no disponible y habilitar las salidas que indiquemos
int32_t ACME_initDevice(tI2CMInstance *instance,uint8_t address)
{

    ACME_data.psI2CInst=instance;
    ACME_data.ui8Addr=address;

    uint8_t array_envio[2];
    uint8_t rxvalue;
    int32_t status;

    array_envio[0]=ACME_ID_REGISTER;
    status=ACME_WriteAndRead(array_envio,1,&rxvalue,1);
    if ((status!=0)||(rxvalue!=ACME_ID_REGISTER_VALUE))
            return -1;  // XXX Esta operación va a fallar hasta que se arregle la biblioteca i2c_driver.c

    array_envio[0]=ACME_DIR_REGISTER;
    array_envio[1]=0;
    status=ACME_Write(array_envio,sizeof(array_envio));
    if (status!=0)
       return -1;

    array_envio[0]=ACME_INT_TYPE_REGISTER_A;
    array_envio[1]=0;
    status=ACME_Write(array_envio,sizeof(array_envio));
    if (status!=0)
            return -1;

    array_envio[0]=ACME_INT_TYPE_REGISTER_B;
    array_envio[1]=0;
    status=ACME_Write(array_envio,sizeof(array_envio));
    if (status!=0)
            return -1;

    array_envio[0]=ACME_INT_CLEAR_REGISTER;
    array_envio[1]=0x3F;
    status=ACME_Write(array_envio,sizeof(array_envio));
    if (status!=0)
            return -1;
    return status;
}

//Establece la direccion de los pines
int32_t ACME_setPinDir (uint8_t dir)
{
    uint8_t array_envio[2];
    int32_t status;

    array_envio[0]=ACME_DIR_REGISTER;
    array_envio[1]=dir;
    status=ACME_Write(array_envio,sizeof(array_envio));
    return status;
}

//Escribe en los pines de salida
int32_t ACME_writePin (uint8_t pin)
{
    uint8_t array_envio[2];
    int32_t status;

    array_envio[0]=ACME_OUTPUT_REGISTER;
    array_envio[1]=pin;
    status=ACME_Write(array_envio,sizeof(array_envio));
    return status;
}

//Lee los pines de entrada
int32_t ACME_readPin (uint8_t *pin)
{
    uint8_t dir_register=ACME_INPUT_REGISTER;
    int32_t status;

    status=ACME_WriteAndRead(&dir_register,sizeof(dir_register),pin,sizeof(uint8_t));
    return status;
}

//Borra las interrupciones
int32_t ACME_clearInt (uint8_t pin)
{
    uint8_t array_envio[2];
    int32_t status;

    array_envio[0]=ACME_INT_CLEAR_REGISTER;
    array_envio[1]=pin;
    status=ACME_Write(array_envio,sizeof(array_envio));
    return status;
}

//habilita/deshabilita las intertupciones y el tipo
int32_t ACME_setIntType (uint8_t regA,uint8_t regB)
{
    //XXX PENDIENTE DE IMPLEMENTAR
    uint8_t array_envio[3]; // Array para enviar datos por I2C
    int32_t status;

    array_envio[0] = ACME_INT_TYPE_REGISTER_A; // Dirección del registro INT_TYPE_A
    array_envio[1] = regA; // Valor para INT_TYPE_A
//    status = ACME_Write(array_envio, sizeof(array_envio)); // Escribir los valores en el registro
//    if (status!=0)
//            return -1;
//    array_envio[0] = ACME_INT_TYPE_REGISTER_B; // Dirección del registro INT_TYPE_B
    array_envio[2] = regB; // Valor para INT_TYPE_B
    status = ACME_Write(array_envio, sizeof(array_envio)); // Escribir los valores en el registro
    if (status!=0)
            return -1;

    return status;
}

//Lee el estado de interrupción
int32_t ACME_readInt (uint8_t *pin)
{
    uint8_t dir_register=ACME_INT_STATUS;
    int32_t status;


    // Leer el valor del registro de interrupción
    status = ACME_WriteAndRead(&dir_register, sizeof(dir_register), pin, sizeof(uint8_t));
    if (status!=0)
            return -1;

    return status;
}

//Funcion para probar la lectura...
int32_t ACME_read (uint8_t *bytes,uint8_t size)
{
    int32_t status;
    status=ACME_Read(bytes,size);
    return status;
}
