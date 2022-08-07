/*
 * baudrate: is an input argument that describes baudrate that the UART needs to make the communications.
 */
 #include <avr/io.h>
 #include <avr/interrupt.h>

#define NUMBER_OF_OVERFLOWS_PER_SECOND 123

volatile unsigned short timeOut = 0;

 void SWUART_init(uint32_t baudrate)
 {
    unsigned short PreScale;

    UCSRA |= (1 << U2X);

    UCSRB = (UCSRB & 0x67) | 0x98;

    SET_BIT(UCSRC,URSEL);
	UCSRC = (UCSRC & 0xCF) | 2;
	SET_BIT(UCSRC,USBS);
	UCSRC = (UCSRC & 0xF9) | (3 << 1);

    PreScale = (F_CPU /( 8 * (baudrate))) - 1;

    UBRRH = (UART_preScale >> 8);
	UBRRL = UART_preScale;
 }

/*
 * data: is an input argument that describes a byte of data to be send over the SW UART.
 */
 void SWUART_send(uint8_t data)
 {
    while(BIT_IS_CLEAR(UCSRA,UDRE));   /* Wait until UDR became empty */

	/* Put the required data in UDR and this also clear the UDRE flag because UDR isn't empty now */
	UDR = data;
 }

 /*
 * data: is an output argument that describes a byte of data to be recieved by the SW UART.
 */
  void SWUART_recieve(uint8_t *data)
  {
    while(timeOut == 0);   /* Wait until there's new unread data in UDR */
	*data = UDR;                       /* return this data */
    timeOut = 0;
  }

  void Timer0_NormalMode_Init()
  {
    TCNT = 0;
    TIMSK |= (1 << TOIE0);
    TCCR0 = (1 << FOC) | (1 << CS02);

  }

  ISR(TIMER0_VECT)
  {
    g_tick++;

    if(g_tick == NUMBER_OF_OVERFLOWS_PER_SECOND)
    {
        timOut = 1
    }
  }

void main() 
{
    unsigned char rec = 0;

    while(1)
    {


        SWUART_recieve(&rec);

        SWUART_send();
    }   

}









