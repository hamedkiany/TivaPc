//*****************************************************************************
//
// protocol.h - USB/serial based protocol (headers and definitions) --> QT side
//
// Copyright (c) 2012-2013 JM Cano - Dpt Tecnologia Electronica (DTE).  All rights reserved.
// Software License Agreement
//
// DTE is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// DTE and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.0.1.11577-43721 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifndef __cplusplus
typedef unsigned char bool;
static const bool False = 0;
static const bool True = 1;
#endif

/****   Definiciones relacionadas con el protocolo de comunicacion por el interfaz USB/serie  ****/

//Caracteres especiales
#define START_FRAME_CHAR 0xCF
#define STOP_FRAME_CHAR 0xDF
#define ESCAPE_CHAR 0xEF
#define STUFFING_MASK 0x02

//Definiciones generales
#define INT32U unsigned int
#define INT16U unsigned short
#define INT8U unsigned char

#define CHEKSUM_TYPE INT16U
#define COMMAND_TYPE INT8U

#define CHECKSUM_SIZE (sizeof(CHEKSUM_TYPE))
#define COMMAND_SIZE (sizeof(COMMAND_TYPE))
#define START_SIZE (1)
#define END_SIZE (1)

#define MINIMUN_FRAME_SIZE (START_SIZE+COMMAND_SIZE+CHECKSUM_SIZE+END_SIZE)

#define MAX_DATA_SIZE (32)
#define MAX_FRAME_SIZE (2*(MAX_DATA_SIZE))

//Codigos de Error
#define PROT_ERROR_BAD_CHECKSUM (-1)
#define PROT_ERROR_RX_FRAME_TOO_LONG (-2)
#define PROT_ERROR_NOMEM (-3)
#define PROT_ERROR_STUFFED_FRAME_TOO_LONG (-4)
#define PROT_ERROR_COMMAND_TOO_LONG (-5)
#define PROT_ERROR_INCONSISTENT_FRAME_FORMAT !(check_command_param_size(frame_size,param_size))

//Macros que obtienen campos del paquete

#define NetToU32(array,posicion) ((((INT32U)array[posicion+3])<<24)|(((INT32U)array[posicion+2])<<16)|(((INT32U)array[posicion+1])<<8)|((INT32U)array[posicion]))
#define NetToU16(array,posicion) ((((INT16U)array[posicion+1])<<8)|(((INT16U)array[posicion])))
#define NetToU8(array,posicion) ((INT32U)array[posicion])
#define SwapBytes16(datos) ((0xFF&(((unsigned short)datos)>>8))|((((unsigned short)datos)&0xFF)<<8))                  
#define decode_command_type(paquete,offset) (NetToU8(paquete,(offset)))
#define extract_packet_command_param(paquete,payload,campo) memcpy(campo,paquete+COMMAND_SIZE,payload);

//Otra macro que puede ser de utilidad
#define check_command_param_size(frame_size,param_size) (!(frame_size-param_size-COMMAND_SIZE-CHECKSUM_SIZE))
#define get_param_size(frame_size) (frame_size-COMMAND_SIZE-CHECKSUM_SIZE)

//Funciones de la libreria
int create_frame(unsigned char *frame, unsigned char command_type, void * param, int param_size, int max_size);
// La funcionalidad de estas dos en la parte de PC (QTs) las hace el componente QtSerialPort, con 'write' y 'readAll'
//int send_frame(int COM, unsigned char *frame, int FrameSize);--> QSerialPort::write
//int receive_frame(int COM, unsigned char *frame, int maxFrameSize); --> QSerialPort::readAll
int destuff_and_check_checksum (unsigned char *frame, int max_size);

/****           Definiciones relacionadas con la implementacion de comandos de la aplicacion       ****/

// El estudiante debe añadir aqui cada nuevo comando que implemente.
// IMPORTANTE el orden de los comandos debe SER EL MISMO aqui, y en el codigo equivalente en la parte del
// microcontrolador (TIVA/Code Composer)

typedef enum {
    COMANDO_NO_IMPLEMENTADO,
    COMANDO_PING,  // Comando PING
    COMANDO_START, // Comando aviso de inicio de aplicacion
    COMANDO_STOP,  // Comando de aviso de fin de aplicación
    COMANDO_ADC,   // Comando ADC
    COMANDO_SPEED, // Comando actualizacion de velocidad
    COMANDO_FUEL,   // Comando actualizacion de fuel
    COMANDO_HEIGHT, // Comando actualizacion de la altura
    COMANDO_CRASH,   // Comando que indica que nos hemos estrellado
    COMANDO_AUTOPILOT,  // Comando de entrada y salida del modo autopiloto
    COMANDO_MSG         // Comando de envío de mensajes por terminal serie
} commandTypes;

//Estructuras relacionadas con los parametros de los comandos
// El estudiante debera crear y añadir las estructuras adecuadas segun los
// parametros que quiera asociar a los comandos. Las estructuras deben ser iguales en los
// ficheros correspondientes de la parte del micro (Code Composer)

#pragma pack(1)	//Con esto consigo compatibilizar el alineamiento de las estructuras en memoria del PC (32 bits)
// y del ARM (aunque, en este caso particular no haria falta ya que ambos son 32bit-Little Endian)

#define PACKED 

// Estructura de parametros para "COMANDO_NO_IMPLEMENTADO"
typedef struct {
    unsigned char command;
} PACKED PARAM_COMANDO_NO_IMPLEMENTADO;

// Estructura de parametros para "COMANDO_ADC"
typedef struct{
    unsigned short yaw;
    unsigned short roll;
    unsigned short pitch;
    bool isPitch;
}PACKED PARAM_COMANDO_ADC;

// Estructura de parametros para "COMANDO_SPEED"
typedef struct {
     unsigned short speed_pctg;
} PACKED PARAM_COMANDO_SPEED;

// Estructura de parametros para "COMANDO_FUEL"
typedef struct {
     float fuel;
} PACKED PARAM_COMANDO_FUEL;

// Estructura de parametros para "COMANDO_HEIGHT"
typedef struct {
     float height;
} PACKED PARAM_COMANDO_HEIGHT;

// Estructura de parametros para "COMANDO_AUTOPILOT"
typedef struct {
     unsigned char on:1;
} PACKED PARAM_COMANDO_AUTOPILOT;

// Estructura de parametros para "COMANDO_MSG"
typedef struct {
     char msg[40];
} PACKED PARAM_COMANDO_MSG;

#pragma pack()	//...Pero solo para los comandos que voy a intercambiar, no para el resto.

#endif
