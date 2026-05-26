#include "MKL25Z4.h"
/* Define o endereço do registrador SIM_SCGC5 (Porta B: 10)*/
#define SIM_SCGC5 (*((volatile unsigned int*)0x40048038))

/* Define o endereço do registrador GPIOB_PDDR */
#define GPIOB_PDDR (*((volatile unsigned int*)0x400FF054))

/* Define o endereço do registrador GPIOB_PDOR */
#define GPIOB_PDOR (*((volatile unsigned int*)0x400FF040))

/* Define o endereço do registrador PORTB_PCR19 */
#define PORTB_PCR19 (*((volatile unsigned int*)0x4004A04C))

void delayMs (int n) {
    /* Função: Espera n milisegundos */
    /* esta função depende do clock default do microcontrolador. Para o KL25Z a frequência é 21 MHz aproximadamente. 
    O valor do contador deverá ser ajustado para se conseguir o tempo de espera desejado. */
	volatile int i;
	volatile int j;
	for (i = 0; i < n; i++)
		for (j = 0; j < 7000; j++) {}
}

int main(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    // ANTES: SIM_SCGC5 |= (1<<10);
    
    GPIOB->PDDR |= (1<<19);
    // ANTES: GPIOB_PDDR |= (1<<19);

    PORTB->PCR[19] = PORT_PCR_MUX(1);
    // ANTES:
    // PORTB_PCR19 |= (1<<8);
    // PORTB_PCR19 &= ~(1<<9);
    // PORTB_PCR19 &= ~(1<<10);

    for(;;) {
        GPIOB->PTOR |= (1<<19);
        //GPIOB_PDOR |= (1<<19);
        delayMs(1500);
        // ANTES: GPIOB_PDOR &= ~(1<<19);
        GPIOB->PTOR &= ~(1<<19);
        // delayMs(3000);
    }

    return 0;
}