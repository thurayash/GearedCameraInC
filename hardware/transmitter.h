#ifndef TRANS_H_
#define TRANS_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define F_CPU 16000000UL // Defining the CPU Frequency


#define USART_BAUDRATE 9600 // Desired Baud Rate
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0<<UMSEL00) // USART Mode Selection

#define DISABLED    (0<<UPM00)
#define EVEN_PARITY (2<<UPM00)
#define ODD_PARITY  (3<<UPM00)
#define PARITY_MODE  DISABLED // USART Parity Bit Selection

#define ONE_BIT (0<<USBS0)
#define TWO_BIT (1<<USBS0)
#define STOP_BIT ONE_BIT      // USART Stop Bit Selection

#define FIVE_BIT  (0<<UCSZ00)
#define SIX_BIT   (1<<UCSZ00)
#define SEVEN_BIT (2<<UCSZ00)
#define EIGHT_BIT (3<<UCSZ00)
#define DATA_BIT   EIGHT_BIT  // USART Data Bit Selection

#define BUF_SIZE 256

void USART_Init();

void USART_TransmitPolling(uint8_t DataByte);

uint8_t USART_ReceivePolling();

void send_string(char* msg);

void read_string(char*);

#endif
