/* ###################################################################
**     Filename    : main.c
**     Project     : Zumo
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-11-04, 14:37, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Led_Azul.h"
#include "Led_Verde.h"
#include "Led_Rojo.h"
#include "WAIT1.h"
#include "CI2C1.h"
#include "UTIL1.h"
#include "Term1.h"
#include "Inhr1.h"
#include "ASerialLdd1.h"
#include "MMA1.h"
#include "GI2C1.h"
#include "IR_Frente.h"
#include "BitIoLdd1.h"
#include "TU2.h"
#include "IR_Atras.h"
#include "BitIoLdd2.h"
#include "Contador.h"
#include "IR_Izq.h"
#include "BitIoLdd3.h"
#include "IR_Der.h"
#include "BitIoLdd4.h"
#include "Echo.h"
#include "BitIoLdd7.h"
#include "Trigger.h"
#include "SW1.h"
#include "BitIoLdd5.h"
#include "PTE.h"
#include "Trigger.h"
#include "BitIoLdd6.h"
#include "MIzq_Adela.h"
#include "TU1.h"
#include "MIzq_Atras.h"
#include "MDer_Adela.h"
#include "MDer_Atras.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#define BLANCO 1
#define NEGRO 0
#define FONDO BLANCO
#define RAYA NEGRO

#define IZQUIERDA 1
#define DERECHA 0
#define ADELANTE 1
#define ATRAS 0

#define CUENTA_UMBRAL 12000

#define OPONENTE 0
#define NO_OPONENTE 1

uint8_t flag_over;
Contador_TValueType tiempo[2];
uint16_t CUENTA = 20000;
uint8_t IR_FRENTE,IR_ATRAS;
uint8_t IR_IZQUIERDA,IR_DERECHA;
uint32_t Ultrasonico;
int16_t x,y,z;

/* User includes (#include below this line is not maintained by Processor Expert) */
void Read_IR(void){
	
	IR_Frente_SetVal();
	WAIT1_Waitus(10);
	Contador_ResetCounter(Contador_DeviceData);
	IR_Frente_SetDir(FALSE);
	flag_over = 0;
	do{
		if(IR_Frente_GetVal() == 1){
			tiempo[0] = Contador_GetCounterValue(Contador_DeviceData);
			if(tiempo[0] > CUENTA)
				flag_over = 1;
		}
		
		else
			flag_over = 1;
	}while(flag_over == 0);
	if(tiempo[0] < CUENTA_UMBRAL)
		IR_FRENTE = BLANCO;
	else
		IR_FRENTE = NEGRO;
	IR_Frente_SetDir(TRUE);
	IR_Frente_ClrVal();

	IR_Atras_SetVal();
	WAIT1_Waitus(10);
	Contador_ResetCounter(Contador_DeviceData);
	IR_Atras_SetDir(FALSE);
	flag_over = 0;
	do{
		if(IR_Atras_GetVal() == 1){
			tiempo[1] = Contador_GetCounterValue(Contador_DeviceData);
			if(tiempo[1] > CUENTA)
				flag_over = 1;
		}
		
		else
			flag_over = 1;
	}while(flag_over == 0);
	if(tiempo[1] < CUENTA_UMBRAL)
		IR_ATRAS = BLANCO;
	else
		IR_ATRAS = NEGRO;
	IR_Atras_SetDir(TRUE);
	IR_Atras_ClrVal();
	return;
}

void Read_Ultrasonico (void){
	Trigger_SetVal();
	WAIT1_Waitus(10);
	Trigger_ClrVal();
	while(Echo_GetVal() == 0){}
	Contador_ResetCounter(Contador_DeviceData);
	flag_over = 1;
	do{
		if(Echo_GetVal() == 1){
			Ultrasonico = Contador_GetCounterValue(Contador_DeviceData);
			if(Ultrasonico >= 52000){
				Ultrasonico = 52000;
				flag_over = 0;
			}
		}
		
		else{
			flag_over = 0;
		}
	}
	while(flag_over);
	Ultrasonico = Ultrasonico/1737;
}

void Calibracion(void){
	uint8_t flag_calibracion = 1;
	Led_Azul_ClrVal(Led_Azul_DeviceData);
	Led_Verde_ClrVal(Led_Verde_DeviceData);
	char simbolo;
	while(flag_calibracion){
		Term1_CRLF();
		Term1_SendStr("***** CALIBRACION *****");
		Term1_CRLF();
		Read_IR();
		IR_IZQUIERDA = IR_Izq_GetVal();
		IR_DERECHA = IR_Der_GetVal();
		Read_Ultrasonico();
		x = MMA1_GetX();
		WAIT1_Waitus(500);
		y = MMA1_GetY();
		Term1_SendStr("tiempo[0]: ");
		Term1_SendNum(tiempo[0]);
		Term1_CRLF();
		Term1_SendStr("tiempo[1]: ");
		Term1_SendNum(tiempo[1]);
		Term1_CRLF();
		Term1_SendStr("IR FRENTE: ");
		Term1_SendNum(IR_FRENTE);
		Term1_CRLF();
		Term1_SendStr("IR  ATRAS: ");
		Term1_SendNum(IR_ATRAS);
		Term1_CRLF();
		Term1_SendStr("IR  Eje X: ");
		Term1_SendNum(x);
		Term1_CRLF();
		Term1_SendStr("IR  Eje Y: ");
		Term1_SendNum(y);
		Term1_CRLF();
		Term1_SendStr("Sensor Izquierdo: ");
		Term1_SendNum(IR_IZQUIERDA);
		Term1_CRLF();
		Term1_SendStr("Sensor Derecho: ");
		Term1_SendNum(IR_DERECHA);
		Term1_CRLF();
		Term1_SendStr("Ultrasonico (cm): ");
		Term1_SendNum(Ultrasonico);
		Term1_CRLF();
		Term1_ReadChar(&simbolo);
		WAIT1_Waitms(20);
		
		if(simbolo == 'O'){
			flag_calibracion = 0;
		}
	}
	
	Led_Azul_SetVal(Led_Azul_DeviceData);
	Led_Verde_SetVal(Led_Verde_DeviceData);
}

void Motor(uint8_t motor, uint8_t direccion, uint8_t velocidad){
	if(motor){ // Izquierda
		if(direccion){ // Adelante
			MIzq_Atras_SetDutyUS(MIzq_Atras_DeviceData, 100);
			MIzq_Adela_SetDutyUS(MIzq_Adela_DeviceData, 100 - velocidad);
		}
		
		else{ // Atras
			MIzq_Adela_SetDutyUS(MIzq_Adela_DeviceData, 100);
			MIzq_Atras_SetDutyUS(MIzq_Atras_DeviceData, 100 - velocidad);
		}
	}
	
	else{ // Derecha
		if(direccion){ // Adelante
			MDer_Atras_SetDutyUS(MDer_Atras_DeviceData, 100);
			MDer_Adela_SetDutyUS(MDer_Adela_DeviceData, 100 - velocidad);
		}
		
		else{ // Atras
			MDer_Adela_SetDutyUS(MDer_Adela_DeviceData, 100);
			MDer_Atras_SetDutyUS(MDer_Atras_DeviceData, 100 - velocidad);
		}
		
	}
}

void Giro90(void){
	  Motor(IZQUIERDA,ADELANTE, 100);
	  Motor(DERECHA,ATRAS, 100);
	  WAIT1_Waitms(200);
}

void Recta (void){
	  Motor(IZQUIERDA,ADELANTE, 50);
	  Motor(DERECHA,ADELANTE, 50);
}
enum STM{
	IDLE,
	BUSQUEDA,
	ATAQUE,
	DEFENSA,
	STOP
};

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
	enum STM estado;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/


/* Iniciamos el Acelerometro --------------------------------------------------------------------------------------------------*/
  MMA1_Init();
  MMA1_SetFastMode(0);
  WAIT1_Waitms(1);
  MMA1_CalibrateX1g();
  WAIT1_Waitms(1);
  MMA1_CalibrateY1g();
  WAIT1_Waitms(1);
  MMA1_CalibrateZ1g();
  estado = IDLE;
  Calibracion();
  for(;;) {
	  
	  switch(estado){
	  case IDLE:
		  Motor(IZQUIERDA, ADELANTE, 0);
		  Motor(DERECHA, ADELANTE, 0);
		  Led_Azul_SetVal(Led_Azul_DeviceData);
		  Led_Verde_SetVal(Led_Verde_DeviceData);
		  Led_Rojo_SetVal(Led_Rojo_DeviceData);
		  while(SW1_GetVal() == 1){
			  Led_Azul_ClrVal(Led_Azul_DeviceData);
			  Led_Rojo_ClrVal(Led_Rojo_DeviceData);
		  }
		  Led_Azul_SetVal(Led_Azul_DeviceData);
		  Led_Rojo_ClrVal(Led_Rojo_DeviceData);
		  WAIT1_Waitms(5000);
		  Led_Rojo_SetVal(Led_Rojo_DeviceData);
		  estado = BUSQUEDA;
		  Giro90();
		  Giro90();
		  Motor(IZQUIERDA,ADELANTE, 0);
		  Motor(DERECHA,ADELANTE, 0);
		  break;
		  
	  case BUSQUEDA:
		  if(IR_FRENTE == FONDO  && IR_ATRAS == FONDO ){
			  Recta();
		  }
		  break;
		  
	  case ATAQUE:		  
		  
		  if(Ultrasonico > 12 && Ultrasonico <= 20){
			  Motor(IZQUIERDA,ADELANTE, 100);
			  Motor(DERECHA,ADELANTE, 100);
			  WAIT1_Waitms(100);
		  }
		  
		  if(Ultrasonico <= 12){
			  if(IR_IZQUIERDA == OPONENTE && IR_DERECHA == OPONENTE){   	// Ataque fuerte 
				  Motor(IZQUIERDA,ADELANTE,100);
				  Motor(DERECHA,ADELANTE,100);
			  }
			  
			  else{
				  if(IR_IZQUIERDA == OPONENTE && IR_DERECHA == NO_OPONENTE){   	// Ataque Izquierda 
					  Motor(IZQUIERDA,ADELANTE, 30);
					  Motor(DERECHA,ADELANTE, 80);
				  }
				  
				  if(IR_IZQUIERDA == NO_OPONENTE && IR_DERECHA == OPONENTE){		// Ataque Derecha 
					  Motor(IZQUIERDA, ADELANTE, 80);
					  Motor(DERECHA, ADELANTE, 30);
				  }
			  }			  
		  }

		  
		  break;
		  
	  case DEFENSA:
		  if(IR_FRENTE == RAYA){
			  Motor(IZQUIERDA,ATRAS,100);
			  Motor(DERECHA,ATRAS,100);
			  WAIT1_Waitms(500);
			  Giro90();

		  }
		  
		  if(IR_ATRAS == RAYA){
			  Motor(IZQUIERDA,ADELANTE,100);
			  Motor(DERECHA,ADELANTE,100);
		  }
		  break;
		  
	  case STOP:
		  while(SW1_GetVal()){}
		  while(SW1_GetVal() == 0){}
		  estado = IDLE;
		  break;
		    
	  }
	  
	  Read_IR();
	  Read_Ultrasonico();
	  IR_IZQUIERDA = IR_Izq_GetVal();
	  IR_DERECHA = IR_Der_GetVal();
	  
	  if(estado != IDLE){
		  if(SW1_GetVal()){
			  if(IR_FRENTE == RAYA || IR_ATRAS == RAYA){
				  Led_Azul_SetVal(Led_Azul_DeviceData);
				  Led_Verde_ClrVal(Led_Verde_DeviceData);
				  estado = DEFENSA;
			  }
			  
			  else{
				  Led_Verde_SetVal(Led_Verde_DeviceData);
				  if(IR_IZQUIERDA == 0 || IR_DERECHA == 0 || Ultrasonico <= 20){
					  Led_Azul_SetVal(Led_Azul_DeviceData);
					  Led_Rojo_ClrVal(Led_Rojo_DeviceData);
					  estado = ATAQUE;
				  }
				  
				  else{
					  Led_Rojo_SetVal(Led_Rojo_DeviceData);
					  Led_Azul_ClrVal(Led_Azul_DeviceData);
					  estado = BUSQUEDA;
				  }
			  }
		  }
		  
		  else{
			  while(SW1_GetVal() == 1){}
			  estado = STOP;
		  }
	  }
	  
	  WAIT1_Waitms(50);
  }
	  
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
