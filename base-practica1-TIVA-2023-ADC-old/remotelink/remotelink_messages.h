/*
 * remotelink_messages.h
 *
 *  Created on: March. 2019
 *
 */

#ifndef RL_MESSAGES_H
#define RL_MESSAGES_H
//Codigos de los mensajes y definicion de parametros para el protocolo RPC

// El estudiante debe agregar aqui cada nuevo mensaje que implemente. IMPORTANTE el orden de los comandos
// debe SER EL MISMO aqui, y en el codigo equivalente en la parte del microcontrolador (Code Composer)

typedef enum {
    MESSAGE_REJECTED,
    MESSAGE_PING,
    MESSAGE_SW,
    MESSAGE_LED_GPIO,
    MESSAGE_ACM_GPIO,
    MESSAGE_ACM_INT,
    MESSAGE_Mode_Cambio,
    MESSAGE_LED_Color,
    MESSAGE_LED_PWM_BRIGHTNESS,
    MESSAGE_ADC_SAMPLE,
    MESSAGE_ADC_LIVE,
    MESSAGE_B160_VELO,
    MESSAGE_B160_ACC,
    MESSAGE_B160_FRQ,
    //etc, etc...
} messageTypes;

//Estructuras relacionadas con los parametros de los mensajes. El estuadiante debera crear las
// estructuras adecuadas a los mensajes usados, y asegurarse de su compatibilidad en ambos extremos

#pragma pack(1) //Cambia el alineamiento de datos en memoria a 1 byte.

typedef struct {
    uint8_t command;
} MESSAGE_REJECTED_PARAMETER;

typedef union{
    struct {
        uint8_t number;
        uint8_t state;
        uint8_t temp;
    } sw;
} MESSAGE_SW_PARAMETER;

typedef union{
    struct {
         uint8_t padding:1;
         uint8_t red:1;
         uint8_t blue:1;
         uint8_t green:1;
    } leds;
    uint8_t value;
} MESSAGE_LED_GPIO_PARAMETER;


typedef union{
    struct {
        uint8_t GP4:1;
        uint8_t GP5:1;
        uint8_t GP6:1;
        uint8_t GP7:1;
    } GPacm;
    uint8_t value;
} MESSAGE_ACM_GPIO_PARAMETER;

typedef union{
    struct {
        int32_t GP0;
        int32_t GP1;
        int32_t GP2;
        int32_t GP3;
    } GPacm;
} MESSAGE_ACM_INT_PARAMETER;

typedef union{
    struct {
        uint8_t red;
        uint8_t blue;
        uint8_t green;
    } leds;
} MESSAGE_LED_Color_PARAMETER;


typedef struct {
    float Mode;
} MESSAGE_Mode_PARAMETER;

typedef struct {
    float rIntensity;
} MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER;

typedef struct
{
    uint16_t chan1;
    uint16_t chan2;
    uint16_t chan3;
    uint16_t chan4;
    uint16_t chan5;
    uint16_t chan6;
    uint16_t chan7;
    uint16_t chan8;

} MESSAGE_ADC_SAMPLE_PARAMETER;

typedef struct
{
    uint16_t chan1;
    uint16_t chan2;
    uint16_t chan3;
    uint16_t chan4;
    uint16_t chan5;
    uint16_t chan6;
    uint16_t chan7;
    uint16_t chan8;

} MESSAGE_ADC_LIVE_PARAMETER;


typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z;

} MESSAGE_B160_ACC_PARAMETER;

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z;

} MESSAGE_B160_VELO_PARAMETER;


typedef struct {
    float frq;
} MESSAGE_B160_FRQ_PARAMETER;
#pragma pack()  //...Pero solo para los comandos que voy a intercambiar, no para el resto.

#endif // RPCCOMMANDS_H

