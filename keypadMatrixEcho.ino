// Purpose: To mirror keypad presses on a portion of a matrix.
// Reference: https://bit.ly/2Wf0Fkz
// Author: A. Goldman
// Date: Dec. 17, 2020
// Status: Working

uint8_t columnData; // Variable to store column data
uint8_t rowData;    // Variable to store row data
uint8_t valueOut;   // Variable to store output data

void setup()
{
    DDRA |= (0 << PA7);   // Sets all bits of Port A as inputs
    PORTA |= ~(0 << PA7); // Activates internal pull ups for Port A
    DDRB |= ~(0 << PB2);  // Sets three bits of Port B as outputs
}

// Creates low level shift out function for Port B...
void outShiftPB(uint8_t data, uint8_t clk, uint8_t latch, uint8_t value)
{
    uint8_t mask = (1 << 7); // Creates mask of 128
    PORTB &= ~(1 << latch);  // Pulls latch low
    while (mask)
    {                         // While mask > 0...
        PORTB &= ~(1 << clk); // Pull clock low
        if (value & mask)
        {                         // If bit to send is high...
            PORTB |= (1 << data); // Pull data line high
        }
        else
        {                          // If bit to send is low...
            PORTB &= ~(1 << data); // Pull data line low
        }
        PORTB |= (1 << clk); // Pull clock high
        mask >>= 1;          // Shift mask right by 1
    }
    PORTB |= (1 << latch); // Pulls latch high
}

void loop()
{
    if (~PINA)
    { // If any key pressed...
        // seperate column data and row data from keypad...
        // and convert to values for matrix
        columnData = (PINA & B00000111);
        rowData = (~(PINA & B01111000)) & B01111000;
        valueOut = rowData | columnData;     // combine data
        outShiftPB(PB0, PB1, PB2, valueOut); // Shift it out
        delay(300);                          // Delay for 300 ms
    }
}
