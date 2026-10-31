#ifndef UART_RX_H
#define UART_RX_H

#define RX_BUFFER_SIZE 128
#define FIFO_PATH "/tmp/stm32_rx_fifo"

int uart_setting(int fd);
int fifo_setting(void);

void *uart_rx_thread(void *arg);

#endif
