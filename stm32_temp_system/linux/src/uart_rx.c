#include "uart_rx.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <pthread.h>

// Configure UART
int uart_setting(int fd)
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

    // Enable receiver
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
int fifo_setting(void)
{
    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo failed");
            return 0;
        }
    }

    return 1;
}

// Write all data to FIFO
static int fifo_write_all(int fd, const char *data, size_t len)
{
    size_t written = 0;

    while (written < len)
    {
        ssize_t n = write(fd, data + written, len - written);

        if (n > 0)
        {
            written += (size_t)n;
        }
        else if (n < 0 && errno == EINTR)
        {
            continue;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

// UART receive thread
void *uart_rx_thread(void *arg)
{
    int uart_fd;
    int fifo_fd;

    char rx_buffer[RX_BUFFER_SIZE];
    int rx_index = 0;
    char ch;

    fd_set readfds;

    (void)arg;

    // Open UART
    uart_fd = open("/dev/serial0", O_RDONLY | O_NOCTTY);

    if (uart_fd < 0)
    {
        perror("UART open failed");
        return NULL;
    }

    // Configure UART
    if (!uart_setting(uart_fd))
    {
        close(uart_fd);
        return NULL;
    }

    // Create FIFO
    if (!fifo_setting())
    {
        close(uart_fd);
        return NULL;
    }

    // Open FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);

    if (fifo_fd < 0)
    {
        perror("FIFO open failed");
        close(uart_fd);
        return NULL;
    }

    while (1)
    {
        // Wait for UART data
        FD_ZERO(&readfds);
        FD_SET(uart_fd, &readfds);

        int ret = select(uart_fd + 1, &readfds, NULL, NULL, NULL);

        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("select failed");
            break;
        }

        // Read only when data is available
        if (FD_ISSET(uart_fd, &readfds))
        {
            ssize_t n = read(uart_fd, &ch, 1);

            if (n == 1)
            {
                // Send a line on newline
                if (ch == '\n')
                {
                    if (!fifo_write_all(fifo_fd, rx_buffer, rx_index) ||
                        !fifo_write_all(fifo_fd, "\n", 1))
                    {
                        perror("FIFO write failed");
                        break;
                    }

                    rx_index = 0;
                }
                // Ignore carriage return
                else if (ch == '\r')
                {
                    continue;
                }
                // Store received character
                else if (rx_index < RX_BUFFER_SIZE - 1)
                {
                    rx_buffer[rx_index++] = ch;
                }
                else
                {
                    // Reset on overflow
                    rx_index = 0;
                }
            }
            else if (n < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }

                perror("UART read failed");
                break;
            }
        }
    }

    // Close file descriptors
    close(fifo_fd);
    close(uart_fd);

    return NULL;
}
