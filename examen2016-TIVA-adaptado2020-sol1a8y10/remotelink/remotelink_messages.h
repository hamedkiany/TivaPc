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
    MESSAGE_EMPEZAR_BF,
    MESSAGE_PARAR_BF,
    MESSAGE_PULSACIONES_BF,
    MESSAGE_REMOTE, /*cambiar nombre */
    MESSAGE_EMPEZAR_MICECATS,
    MESSAGE_PARAR_MICECATS,
    MESSAGE_BOTON_MICECATS,
    MESSAGE_MOUSE,
    MESSAGE_PUNTOS_MICECATS
    //etc, etc...
} messageTypes;

//Estructuras relacionadas con los parametros de los mensajes. El estuadiante debera crear las
// estructuras adecuadas a los mensajes usados, y asegurarse de su compatibilidad en ambos extremos

#pragma pack(1) //Cambia el alineamiento de datos en memoria a 1 byte.

typedef struct {
    uint8_t command;
} MESSAGE_REJECTED_PARAMETER;


typedef struct{
    uint32_t pulsaciones;
}  MESSAGE_PULSACIONES_BF_PARAMETER;

typedef struct{
    uint8_t modo;
}  MESSAGE_EMPEZAR_BF_PARAMETER;

typedef struct{
    uint8_t HuntedMouse;
}  MESSAGE_BOTON_MICECATS_PARAMETER;

typedef struct{
    uint8_t Mouse;
}  MESSAGE_MOUSE_PARAMETER;

typedef struct{
    int32_t puntuacion;
} MESSAGE_PUNTOS_MICECATS_PARAMETER;

typedef struct{
    int8_t remote;
} MESSAGE_REMOTE_PARAMETER;

#pragma pack()  //...Pero solo para los comandos que voy a intercambiar, no para el resto.

#endif // RPCCOMMANDS_H

