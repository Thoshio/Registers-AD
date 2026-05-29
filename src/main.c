#include "MKL25Z4.h"
//#include <zephyr/kernel.h>

int main(void) {
    // 1. Ative o clock da GPIO usada pelo canal ADC
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // 2. Defina o bit MUX do PORTX_PCRn para o pino de entrada do ADC
    PORTB->PCR[0] = PORT_PCR_MUX(0);
    // por default os pinos são setados como inputs, então não precisamos da linha abaixo
    // GPIOB->PDDR &= ~(1<<0);

    // 3. Ative o clock para o modulo ADC usando o registrador SIM_SCGC6
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

    // 4. Escolha o tipo de trigger para inicar a conversao analógica digital usando o registrador ADC0_SC2
    //ADC0->SC2 &= ~(1<<6); // SOFTWARE -> default

    // 5. Escolha a fonte de clock e a resolucao usando o registrador ADC0_CFG1
    // default -> 0
    //ADC0->CFG1 = ADC_CFG1_MODE(2); // RESOLUCAO (10)
    //ADC0->CFG1 = ADC_CFG1_ADICLK(1); // CLOCK (BusClock/2)

    // 6. Selecione o canal de entrada ADC usando o registro ADC0_SC1A
    // certifique-se que a interrupcao esteja DESabilitada e usando single ended
    //ADC0->SC1[0] &= ~ADC_SC1_AIEN_MASK; // INTERRUPCAO (default -> 0)
    //ADC0->SC1[0] &= ~ADC_SC1_DIFF_MASK; // SINGLE-ENDED (default -> 0)
    //ADC0->SC1[0] = ADC_SC1_ADCH(8); // Canal de entrada: CH0

    // configurando leds:
    GPIOB->PDDR |= (1<<19);
    PORTB->PCR[19] = PORT_PCR_MUX(1);
    GPIOB->PDOR |= (1<<19); // desliga led verde

    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    GPIOD->PDDR |= (1<<1);
    PORTD->PCR[1] = PORT_PCR_MUX(1);
    GPIOD->PDOR |= (1<<1); // desliga led azul



    for(;;) {
    // 6. Selecione o canal de entrada ADC usando o registro ADC0_SC1A
    ADC0->SC1[0] = ADC_SC1_ADCH(8); // Canal de entrada: CH0 (essa linha )

    // 7. Monitore quando o flag de fim de conv. (COCO) no registrador ADC0_SC1A
    int COCO = 0;
    while (COCO == 0) {
    COCO = ADC0->SC1[0] & ADC_SC1_COCO_MASK;
    }

    // 8. Quando o flag COCO for setado, leia o resultado da conversão em ADC0_RA e salve-o
    int resultado;
    resultado = ADC0->R[0];

    // led azul liga quando for 3.3V
    // led verde liga quando for 0V
    if (resultado > 255/2) {
        GPIOB->PDOR |= (1<<19); // verde
        GPIOD->PDOR &= ~(1<<1); // liga azul (-> 0)
        
    } else {
        GPIOB->PDOR &= ~(1<<19); // verde
        GPIOD->PDOR |= (1<<1); // azul
    }

    }

    return 0;
}