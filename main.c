#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {

    /* -----------------------------------------------------------------
     * | PINB7 | PINB6 | PINB5 | PINB4 | PINB3 | PINB2 | PINB1 | PINB0 |
     * -----------------------------------------------------------------
     * |  N/A  |  N/A  |  SCK  |  MISO |  MOSI |  N/A  | USBEN |  USB4 |
     * -----------------------------------------------------------------
     * |   IN  |   IN  |   IN  |  OUT  |   IN  |   IN  |  OUT  |  OUT  |
     * -----------------------------------------------------------------
     */

    DDRB = 0b00010011 & 0x13;
    PORTB = 0b00000000 & 0x00;

    /* -----------------------------------------------------------------
     * |  N/A  | PINC6 | PINC5 | PINC4 | PINC3 | PINC2 | PINC1 | PINC0 |
     * -----------------------------------------------------------------
     * |  N/A  |  N/A  | LED5  | LED4  | LED3  | LED2  | LED1  | P_BTN |
     * -----------------------------------------------------------------
     * |  N/A  |  IN   |  OUT  |  OUT  |  OUT  |  OUT  |  OUT  |  IN   |
     * -----------------------------------------------------------------
     */

    DDRC = 0b00000001 & 0x01;
    PORTC = 0b00000000 & 0x00;

    /* -----------------------------------------------------------------
     * | PIND7 | PIND6 | PIND5 | PIND4 | PIND3 | PIND2 | PIND1 | PIND0 |
     * -----------------------------------------------------------------
     * |  USB3 |  USB2 |  USB1 |  N/A  |  N/A  |  TXD  |  RXD  | RESET |
     * -----------------------------------------------------------------
     * |  OUT  |  OUT  |  OUT  |  IN   |  IN   |  OUT  |  IN   |  N/A  |
     * -----------------------------------------------------------------
     */

    DDRD = 0b11100100 & 0xE4;
    PORTD = 0b00000000 & 0x00;

    /* PCIE0 => PCINT0..7 PORTB
     * PCIE1 => PCINT8..14 PORTC
     * PCIE2 => PCINT016..23 PORTD
     */

    PCICR = (0 << PCIE0) | (1 << PCIE1) | (0 << PCIE2);

    /*
     * ---------------------------------------------------------------------------------
     * |  PINB7  |  PINB6  |  PINB5  |  PINB4  |  PINB3  |  PINB2  |  PINB1  |  PINB0  |
     * ---------------------------------------------------------------------------------
     * | PCINT7  | PCINT6  | PCINT5  | PCINT4  | PCINT3  | PCINT2  | PCINT1  | PCINT0  |
     * ---------------------------------------------------------------------------------
     */

    PCMSK0 = 0;

    /*
     * ---------------------------------------------------------------------------------
     * |   N/A   |  PINC6  |  PINC5  |  PINC4  |  PINC3  |  PINC2  |  PINC1  |  PINC0  |
     * ---------------------------------------------------------------------------------
     * |         | PCINT14 | PCINT13 | PCINT12 | PCINT11 | PCINT10 | PCINT9  | PCINT8  |
     * ---------------------------------------------------------------------------------
     */

    PCMSK1 = 1 << PCINT8;

    /*
     * ---------------------------------------------------------------------------------
     * |  PIND7  |  PIND6  |  PIND5  |  PIND4  |  PIND3  |  PIND2  |  PIND1  |  PIND0  |
     * ---------------------------------------------------------------------------------
     * | PCINT23 | PCINT22 | PCINT21 | PCINT20 | PCINT19 | PCINT18 | PCINT17 | PCINT16 |
     * ---------------------------------------------------------------------------------
     */

    PCMSK2 = 0;
 
    sei();

    for ( ; ; ) asm("nop");

    return 0;
}

ISR(PCINT1_vect) {

    static volatile uint8_t xUSBPin = 0;

    xUSBPin++;

    xUSBPin %= 5;

    PORTC = (1 << (xUSBPin + 1));

    PORTD &= (0b00011111 & 0x1F);
    PORTB &= (0b11111110 & 0xF7);

    if ( xUSBPin == 4 ) {
        PORTB |= 0x01;
    }

    if ( xUSBPin <= 3 && xUSBPin > 0 ){
        PORTD |= (1 << (4 + xUSBPin));
    }

    _delay_ms(500);

    PCIFR |= (1 << PCIF1);

}
