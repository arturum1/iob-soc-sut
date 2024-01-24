#include <stdint.h>
#include "iob-uart16550.h"

//TX FUNCTIONS
void uart16550_putc(char c) {
    *((volatile uint8_t *)(base)) = c;
}

//RX FUNCTIONS
char uart16550_getc() {
    uint8_t rvalue;
    rvalue = *((volatile uint8_t *)(base));
    return rvalue;
}

// Change UART base
void uart16550_base(int base_address) {
	base = base_address;
}

void uart16550_finish() {
  uart16550_putc(EOT);
}

//Print string, excluding end of string (0)
void uart16550_puts(const char *s) {
  while (*s) uart16550_putc(*s++);
}

//Sends the name of the file to use, including end of string (0)
void uart16550_sendstr (char* name) {
  int i=0;
  do
    uart16550_putc(name[i]);
  while (name[i++]);
}

//Receives file into mem
int uart16550_recvfile(char* file_name, char *mem) {

  uart16550_puts(UART_PROGNAME);
  uart16550_puts (": requesting to receive file\n");

  //send file receive request
  uart16550_putc (FRX);

  //send file name
  uart16550_sendstr(file_name);


  //receive file size
  int file_size = (unsigned int) uart16550_getc();
  file_size |= ((unsigned int) uart16550_getc()) << 8;
  file_size |= ((unsigned int) uart16550_getc()) << 16;
  file_size |= ((unsigned int) uart16550_getc()) << 24;

  //allocate space for file if file pointer not initialized
  if(mem == NULL) {
    mem = (char *) malloc(file_size);
    if (mem == NULL) {
      uart16550_puts(UART_PROGNAME);
      uart16550_puts("Error: malloc failed");
    }
  }

  //send ACK before receiving file
  uart16550_putc(ACK);

  //write file to memory
  for (int i = 0; i < file_size; i++) {
    mem[i] = uart16550_getc();
  }

  uart16550_puts(UART_PROGNAME);
  uart16550_puts(": file received\n");

  return file_size;
}

//Sends mem contents to a file
void uart16550_sendfile(char *file_name, int file_size, char *mem) {

  uart16550_puts(UART_PROGNAME);
  uart16550_puts(": requesting to send file\n");

  //send file transmit command
  uart16550_putc(FTX);

  //send file name
  uart16550_sendstr(file_name);

  // send file size
  uart16550_putc((char)(file_size & 0x0ff));
  uart16550_putc((char)((file_size & 0x0ff00) >> 8));
  uart16550_putc((char)((file_size & 0x0ff0000) >> 16));
  uart16550_putc((char)((file_size & 0x0ff000000) >> 24));

  // send file contents
  for (int i = 0; i < file_size; i++)
    uart16550_putc(mem[i]);

  uart16550_puts(UART_PROGNAME);
  uart16550_puts(": file sent\n");
}
