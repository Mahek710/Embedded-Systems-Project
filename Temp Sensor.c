#include <LPC17xx.h>
#include <stdio.h>
 
#define rsctrl 0x00000100       // p0.8
#define enctrl 0x00000200       // p0.9 the lcd enable
#define dtctrl 0x000000f0       // p0.4-7 4 bit mode
 
#define ADC_CHANNEL_SEL (1<<0)  // The channel selecting on ADC
#define ADC_ENABLE (1<<21)      // The ADC component Enabling  
#define ADC_START (1<<24)       // The ADC is loading into software mode
#define ADC_DONE (1U<<31)       // defining it as compiler will throw #61-D warning or an unsigned value 
 
void lcd_init(); 
void wr_cn(); 
void clr_disp(); 
void delay(); 
void lcd_com(); 
void wr_dn(); 
void lcd_data(); 
void clear_ports(); 
void lcd_puts(unsigned char *); 
 
unsigned int i, j, temp, temp1=0, temp2=0; 
float final;
char temper[20];
unsigned long LED = 0x00000010;
 
int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
 
    LPC_PINCON->PINSEL1 |= (1 << 14);
 
    while (1) {
        LPC_ADC->ADCR = ADC_CHANNEL_SEL | ADC_ENABLE | ADC_START;
        
        while (((temp = LPC_ADC->ADDR0) & ADC_DONE) == 0);
        temp = LPC_ADC->ADDR0;
        temp &= 0xFFF0;
        temp >>= 4;
        final = (float)(temp / 12.41);
        sprintf(temper, "%3.2fC", final);
 
        lcd_init();			//lcd_init function called
        temp1 = 0x80;
 
        lcd_com();           //lcd_com function called
        delay(800);          //delay function called
 
        lcd_puts( & temper[0]);
    }
 
}
 
void lcd_init() {
    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= dtctrl;
    LPC_GPIO0->FIODIR |= rsctrl;
    LPC_GPIO0->FIODIR |= enctrl;
    clear_ports();			//clear_ports function called
    delay(3200);			//delay function called
    for (i = 0; i < 3; i++) {
        temp2 = (0x30);
        wr_cn();			//wr_cn function called
        delay(30000);		//delay function called
    }
 
    temp2 = (0x20);
    wr_cn();				//wr_cn function called
    delay(30000);			//delay function called
 
    temp1 = 0x28;
    lcd_com();				//lcd_com function called
    delay(30000);			//delay function called
 
    temp1 = 0x0c;
    lcd_com();				//lcd_com function called
    delay(800);			//delay function called
 
    temp1 = 0x06;
    lcd_com();				//lcd_com function called
    delay(800);			//delay function called
 
    temp1 = 0x01;
    lcd_com();				//lcd_com function called
    delay(10000);			//delay function called
 
    temp1 = 0x80;
    lcd_com();				//lcd_com function called
    delay(800);			//delay function called
}
 
void lcd_com() {
    temp2 = temp1 & 0xf0;
    temp2 = temp2;
    wr_cn();				//wr_cn function called
 
    temp2 = temp1 & 0x0f;
    temp2 = temp2 << 4;
    wr_cn();				//wr_cn function called
 
    delay(1000);			//delay function called
}
 
void wr_cn() {
    clear_ports();			//clear_ports function called
    LPC_GPIO0->FIOPIN = temp2;
    LPC_GPIO0->FIOCLR = rsctrl;
    LPC_GPIO0->FIOSET = enctrl;
    delay(25);				//delay function called
    LPC_GPIO0->FIOCLR = enctrl;
}
 
void lcd_data() {
    temp2 = temp1 & 0xf0;
    temp2 = temp2;
    wr_dn();				//wr_dn function called
    temp2 = temp1 & 0x0f;
    temp2 = temp2 << 4;
    wr_dn();				//wr_dn function called
    delay(1000);			//delay function called
}
 
void wr_dn() {
    clear_ports();			//clear_ports function called
    LPC_GPIO0->FIOPIN = temp2;
    LPC_GPIO0->FIOSET = rsctrl;
    LPC_GPIO0->FIOSET = enctrl;
    delay(25);				//delay function called
    LPC_GPIO0->FIOCLR = enctrl;
}
 
void delay(unsigned int r1) {
    unsigned int r;
    for (r = 0; r < r1; r++);
}
 
void clr_disp() {
    temp1 = 0x01;
    lcd_com();				//lcd_com function called
    delay(10000); 			//delay function called
} 
 
void clear_ports() {
    LPC_GPIO0->FIOCLR |= rsctrl;
    LPC_GPIO0->FIOCLR |= enctrl;
    LPC_GPIO0->FIOCLR |= dtctrl;
}
 
void lcd_puts(unsigned char * buff) {
    unsigned int i = 0;
    while (buff[i] != '\0') {
        temp1 = buff[i];
        i++;
        lcd_data();			//lcd_data function called
    }
}
