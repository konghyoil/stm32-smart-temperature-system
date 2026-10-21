#ifndef UART_TX_H
#define UART_TX_H

#define TX_FIFO_PATH "/tmp/stm32_tx_fifo"

int uart_tx_setting(int fd);
int tx_fifo_setting(void);

void *uart_tx_thread(void *arg);

#endif
