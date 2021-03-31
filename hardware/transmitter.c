#include "transmitter.h"

void send_string(char* msg);

void USART_Init()
{
    // Set Baud Rate
    UBRR0H = BAUD_PRESCALER >> 8;
    UBRR0L = BAUD_PRESCALER;

    // Set Frame Format
    UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;

    // Enable Receiver and Transmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void USART_TransmitPolling(uint8_t DataByte)
{
    while (( UCSR0A & (1<<UDRE0)) == 0) {}; // Do nothing until UDR is ready
    UDR0 = DataByte;
}

uint8_t USART_ReceivePolling() // Write 1 if received 0 else
{
    uint8_t DataByte;
    while (( UCSR0A & (1<<RXC0)) == 0) {}; // Do nothing until data have been received
    DataByte = UDR0 ;
    if (DataByte == 10)
        return USART_ReceivePolling();
    send_string("1\n"); // IMPORTANT !! Don't remove it !!
    return DataByte;
}

void send_string(char* msg)
{
    for(; *msg != '\0'; msg++) USART_TransmitPolling(*msg);
}

void read_string(char* buffer)
{
    char *start = buffer;
    char letter = USART_ReceivePolling();
    while(letter != '+')
    {
        *start = letter;
        start++;
        letter = USART_ReceivePolling();
        USART_TransmitPolling(letter);
    }
    *start = '\0'; // Stop the array

    send_string("DONE ATMEGA328p \n");
    //send_string(buffer);
    send_string("\n");

    return buffer;
}

