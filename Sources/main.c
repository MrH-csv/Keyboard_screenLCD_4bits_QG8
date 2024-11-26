// ANGEL HABID NAVARRO MENDEZ 20110320 IDESI 7ºK

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

unsigned char cadena[16]={"El caracter es:"};//mensaje(&cadena[0]);
unsigned char initLCD[6]={0x33,0x32,0x28,0x0F,0x06,0x01};
unsigned char filas[4] = {0xEF, 0xDF, 0xBF, 0x7F}; // Estado teclado
unsigned char columnas[4] = {0x7E, 0xBE, 0xDE, 0xEE}; // Valor columna
unsigned char numeros [4][4] = {{0x30/*0*/, 0x31/*1*/, 0x32/*2*/, 0x33/*3*/},{0x34/*4*/, 0x35/*5*/, 0x36/*6*/, 0x37/*7*/},
								{0x38/*8*/, 0x39/*9*/, 0x3D/*=*/, 0x2B/*+*/},{0x2A/***/, 0x2D/*-*/, 0x2F/*/*/, 0x25/*%*/}};

/************************ Basic Functions***************************/
void MCU_ini(void);
void swept(void);
/******************************************************************/

/**********LCD Functions*****************************************/

void caracterCGRAM(unsigned char *m);
void mensaje (unsigned char *p);
void inicializa (void);
void datos (unsigned char x);
void enable(void);
void delay(void);
void instrucciones(unsigned char x);
/******************************************************************/

void main(void) {
	
	SOPT1=0x12;
	MCU_ini();
	inicializa();
	mensaje(&cadena[0]);
	instrucciones(0xc0);
    while(1) {
        swept();
    }
	for(;;) {
	__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
	}

void MCU_ini(void){
	/*init LCD*/
	PTBDD=0xFF;		//Para el Bus de datos.
	PTADD_PTADD0=1; //E
	PTADD_PTADD1=1; //RS
	PTAD_PTAD0=0; 	//Poner E=0.
	PTAD_PTAD1=0;	//RS=0.
	//Teclado:
    SOPT1 = 0x12;
    PTBSE = 0xF0;
    PTBDS = 0xF0;
    PTBDD = 0xF0;
    PTBD = 0xF0;	//Para debug del puerto B se inicializan en 1 las filas. y cero las columnas.
    				// Filas = Salidas ; Columnas = entradas
}

void delay (void)
{
	 SRTISC=0x02;
	while(SRTISC_RTIF==0);
	   SRTISC_RTIACK=1;
	SRTISC=0X00;
}

void enable (void)
{
	PTAD_PTAD0=1;
	delay();
	PTAD_PTAD0=0;
	delay();
}

void datos (unsigned char x)
{
	unsigned char y=0x00;
	PTAD_PTAD1=1;	//RS=1;
	y=x&0xF0;
	PTBD=y;
	enable();
	y=x&0x0F;
	y=y<<4;//y<<=4 "contraccion"
	PTBD=y;
	enable();
}

void instrucciones (unsigned char x)
{
	unsigned char y=0x00;
	PTAD_PTAD1=0; //RS=0
	y=x&0xF0;
	PTBD=y;
	enable();
	y=x&0x0F;
	y=y<<4;//y<<=4 "contraccion"
	PTBD=y;
	enable();
}

void mensaje (unsigned char *p)
{
	while (*p!=0)
	{
		datos(*p);
		p=p+1;	
	}
}

void inicializa (void)
{
	unsigned char t=0x00;
	while(t!=6)
	{
		instrucciones(initLCD[t]);
		t=t+1;
	}
}

void swept(void) {
	unsigned char row , col , temp = 0;
    //PTBD = 0; //apaga puerto B
    for (row = 0; row < 4; row++) {//barre filas
        PTBD = filas[row];
        delay();
    for (col = 0; col < 4; col++) {
    	if (!(PTBD & (1 << col))) { //barre columnas y valida si la tecla esta presionada.
    		temp = PTBD;
    		datos(numeros[row][col]);
            	while(!(PTBD & (1 << col))){}// Valida si ya solto el botton.
            	
    			}
            }
        }
    }
