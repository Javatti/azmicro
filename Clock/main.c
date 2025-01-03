#include <mega32.h>
#include <stdio.h>
#include <delay.h>
// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
int counter;
// taarif moteghayer haye sanie daghighe va saat
int s=58,m=59,h=12;
//taarif moteghayer haye marboot be alarm va meghdar dehi anha
int Alarm_S=15,Alarm_M=2,Alarm_H=0;
// taarif reshte baraye neshan dadan maghadir saat
char text[20];
// yek flag baraye taaeen vazeeiat am va pm 0=am 1=pm
int status_hour = 1;
//flag baraye alarm
int alarm = 0;

// Timer 0 output compare interrupt service routine
interrupt [TIM0_COMP] void timer0_comp_isr(void)
{
/*
ba tavajoh be eenke ma interupt timer ra bar roy
2 ms tanzim kardim pas har 500ms*2ms = 1 sec
*/
counter ++;
if ( counter == 500 ){
    //shomarande ra 0 mikonim ta dobare beshmorad
    counter = 0;                                 
    // sanie ziad mishavad
    s ++;
    //shart 60 sanie va daghighe ezafe mishavad
    if ( s == 60 ){
        //sanie reset mishavad 0
        s = 0;
        //be daghighe ezafe mishavad
        m ++;                       
        //shart saat va residan daghighe be 60 min
        if ( m == 60 ){
            //daghighe reset mishavad
            m = 0;
            // be saat ezafe mishavad
            h ++;
            //shart saat 13 ke nadarim beshe 0 bejay 24 halat am/pm gharar dadim
            //code hay paeen baray 24 saat mibashad az comment mitavan kharejesh kard
            /*
            if ( h == 24 ){
                h = 0;
                }
            */
            if ( h == 13){
                h = 0;
                //dar inja miam meghdar moteghayer am/pm ra ba khodash NOT mishavad
                // yaani agar 0 bashad ba khodash NOT shavad mishavad 1
                // va agar 1 bashad ba khodash NOT shavad mishavad 0
                status_hour = ! status_hour ;

                }
            
            }
        }
        //pak kardan lcd
        lcd_clear();
        //gharar dadan meghdar reshte saat
        sprintf(text,"time:%02d:%02d:%02d",h,m,s);
        //dastor taan moghiat makani 
        lcd_gotoxy(0,0);
        //neshan dadan meghdar reshteh
        lcd_puts(text);
        lcd_gotoxy(14,0);
        //shart taan halat am/pm
        if ( status_hour == 0)lcd_puts("AM");
        else lcd_puts("PM");
        if (alarm == 1){
            lcd_gotoxy(4,1);
            lcd_puts("!!!ALARM!!!");
            }
    }
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
// Function: Bit7=Out Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(1<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=0 Bit6=T Bit5=T Bit4=T Bit3=P Bit2=P Bit1=P Bit0=P 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);


// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 125/000 kHz
// Mode: CTC top=OCR0
// OC0 output: Disconnected
// Timer Period: 2 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0x00;
OCR0=0xF9;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (1<<OCIE0) | (0<<TOIE0);

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
      
      //shart check kardan meghdar tanzim shode alarm ba meghdar saat va hamchenin am bashad ya pm
      //ke dar inja AM mibashad
      if (( Alarm_H == h ) && ( Alarm_M == m ) && ( Alarm_S == s ) && ( status_hour == 0 )){
        //led roshan mishavad
        PORTB.7 = 1;
        //dastorat chap vazhe alarm
        alarm = 1;
        }           
      //shart check kardan mizan 5 sanie va khamooosh shodan led 
      if ( ( Alarm_H == h ) && ( Alarm_M == m ) && (  s == Alarm_S + 5 ) && ( status_hour == 0 ) ){
        PORTB.7 = 0;
        alarm = 0;
        }
      /*dastorat marboot be kelid hay mizan hour va minutes*/
      //kelid bala bordan mizan saat
      if ( PINB.0 == 0 && h < 13){
            //jahat bala naraftan alaki meghdar
            while ( PINB.0 == 0 );
            h ++;
            //in dastor va shart an dar line 43-53 tozzih dadeh shode ast
            if ( h == 13){
                h = 0;
                status_hour = ! status_hour;
                }
            } 
        //shart vase kam kardan saat
      if ( PINB.1 == 0 && h > 0){
            while ( PINB.1 == 0 );
            h --;
            }
         //shart vase bala bordan daghighe
      if ( PINB.2 == 0 && m < 60 ){
            while ( PINB.2 == 0 );
            m ++;
            }
      //shart kam kardan daghighe
      if ( PINB.3 == 0 && m > 0 ){
            while ( PINB.3 == 0 );
            m --;
            }
      }
}

