

#include <mega32.h>
#include <delay.h>
#include <stdio.h>
// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
char text[20];
// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    if (PIND.2 == 0){
     OCR0 ++;  //in dastor register end Timer ra balatar mibarad
     while (PIND.2 == 0);
    }

}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
    if (PIND.3 == 0 && OCR0 > 1){
     OCR0 --;  //in dastor register end Timer ra kamtar mikonad
     while (PIND.3 == 0);
    }

}

// Timer 0 output compare interrupt service routine
interrupt [TIM0_COMP] void timer0_comp_isr(void)
{

/* in function dar vaghe
 baad az har bar tamom shodan meghdar counter
 yek interupt faal mikonad
 ke dar inja fellan ma azash estefade nemikonim*/

}

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=0 Bit2=T Bit1=T Bit0=P 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (1<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=P Bit2=P Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: T0 pin Falling Edge
// Mode: CTC top=OCR0
// OC0 output: Toggle on compare match
TCCR0=(0<<WGM00) | (0<<COM01) | (1<<COM00) | (1<<WGM01) | (1<<CS02) | (1<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x02;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (1<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1: On
// INT1 Mode: Falling Edge
// INT2: Off
GICR|=(1<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(1<<INTF1) | (1<<INTF0) | (0<<INTF2);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTA Bit 0
// RD - PORTA Bit 1
// EN - PORTA Bit 2
// D4 - PORTA Bit 4
// D5 - PORTA Bit 5
// D6 - PORTA Bit 6
// D7 - PORTA Bit 7
// Characters/line: 16
lcd_init(16);

// Global enable interrupts
#asm("sei")

while (1)
      {
      /* shart mizarim ke meghdar register
       az 1 kamter bod biad register ro 1 konad*/
      if (OCR0 < 1 ) OCR0 = 1; 
      /*hala agar meghdar register payani timer
       az meghdar register shoroee timer yaani "TCNT0"
       kamtar bod meghdar TCNT0 ra dar OCR0 beriz  */
       if (OCR0 < TCNT0 ) TCNT0 = OCR0;
       /*dastor sprintf mikhahim dakhel reshte ke darim
       meghdar TCNT0 berizim ta chap beshe*/
       sprintf(text,"stert:%d",TCNT0);
       //dastor mokhtasat lcd
       lcd_gotoxy(0,0);
       //chap araye text
       lcd_puts(text);
       //dastor mokhtasat khad baadi
       lcd_gotoxy(1,1);
       //dastor sprintf meghdar register end
       sprintf(text,"end:%d",OCR0);
       //chap araye text
       lcd_puts(text); 
       //pak kardan lcd bad az 100 ms
       delay_ms(100);
       lcd_clear();
       

      }
}
