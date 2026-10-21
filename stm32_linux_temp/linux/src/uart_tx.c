#include "uart_tx.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <pthread.h>

// Configure UART
int uart_tx_setting(int fd)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0)
    {
        perror("tcgetattr failed");
        return 0;
    }

    // Set baud rate
    if (cfsetispeed(&tty, B9600) != 0 ||
        cfsetospeed(&tty, B9600) != 0)
    {
        perror("Set baud rate failed");
        return 0;
    }

    // Set 8-N-1 mode
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    // Disable hardware flow control
    tty.c_cflag &= ~CRTSCTS;

    // Enable transmitter
    tty.c_cflag |= (CLOCAL | CREAD);

    // Disable input processing
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICRNL | INLCR | IGNCR);

    // Disable echo and canonical mode
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Disable output processing
    tty.c_oflag &= ~OPOST;

    // Set read behavior
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 0;

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr failed");
        return 0;
    }

    return 1;
}

// Create FIFO
int tx_fifo_setting(void)
{
    if (mkfifo(TX_FIFO_PATH, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo failed");
            return 0;
        }
    }

    return 1;
}

// UART transmit thread
void *uart_tx_thread(void *arg)
{
    int uart_fd;
    int fifo_fd;
    fd_set readfds;
    char buffer[128];

    (void)arg;

    // Open UART
    uart_fd = open("/dev/serial0", O_WRONLY | O_NOCTTY);

    if (uart_fd < 0)
    {
        perror("UART open failed");
        return NULL;
    }

    // Configure UART
    if (!uart_tx_setting(uart_fd))
    {
        close(uart_fd);
        return NULL;
    }

    // Create FIFO
    if (!tx_fifo_setting())
    {
        close(uart_fd);
        return NULL;
    }

    // Open FIFO for reading
    fifo_fd = open(TX_FIFO_PATH, O_RDONLY);

    if (fifo_fd < 0)
    {
        perror("FIFO open failed");
        close(uart_fd);
        return NULL;
    }

    printf("UART TX thread started...\n");

    while (1)
    {
        // Wait for FIFO data
        FD_ZERO(&readfds);
        FD_SET(fifo_fd, &readfds);

        int ret = select(fifo_fd + 1, &readfds, NULL, NULL, NULL);

        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("select failed");
            break;
        }

        // Read only when FIFO is ready
        if (FD_ISSET(fifo_fd, &readfds))
        {
            ssize_t n = read(fifo_fd, buffer, sizeof(buffer));

            if (n > 0)
            {
                // Forward FIFO data to UART
                ssize_t sent = 0;

                while (sent < n)
                {
                    ssize_t written = write(
                        uart_fd,
                        buffer + sent,
                        (size_t)(n - sent)
                    );

                    if (written > 0)
                    {
                        sent += written;
                    }
                    else if (written < 0 && errno == EINTR)
                    {
                        continue;
                    }
                    else
                    {
                        perror("UART write failed");
                        goto cleanup;
                    }
                }
            }
            else if (n < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }

                perror("FIFO read failed");
                break;
            }
            else
            {
                // Wait for a new FIFO writer
                close(fifo_fd);

                fifo_fd = open(TX_FIFO_PATH, O_RDONLY);

                if (fifo_fd < 0)
                {
                    perror("FIFO reopen failed");
                    break;
                }
            }
        }
    }

cleanup:
    // Close file descriptors
    close(fifo_fd);
    close(uart_fd);

    return NULL;
}
