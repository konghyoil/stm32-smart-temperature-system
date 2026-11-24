
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include "uart_rx.h"
#include "uart_tx.h"

#define GUI_PATH "./gui/gui"

int main(void)
{
    pid_t pid;
    int status;

    pthread_t rx_thread;
    pthread_t tx_thread;

    int rx_created = 0;
    int tx_created = 0;

    /* Create the GUI process */
    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        /* Execute the GUI */
        execl(GUI_PATH, "gui", (char *)NULL);

        perror("exec failed");
        _exit(EXIT_FAILURE);
    }

    /* Print process information */
    printf("Temp System started.\n");
    printf("GUI process PID: %d\n", (int)pid);

    /* Create the UART RX thread */
    if (pthread_create(
            &rx_thread,
            NULL,
            uart_rx_thread,
            NULL) != 0)
    {
        perror("RX thread creation failed");
        goto cleanup;
    }

    rx_created = 1;

    /* Create the UART TX thread */
    if (pthread_create(
            &tx_thread,
            NULL,
            uart_tx_thread,
            NULL) != 0)
    {
        perror("TX thread creation failed");
        goto cleanup;
    }

    tx_created = 1;

    printf("UART RX/TX threads started.\n");

    /* Wait for the GUI to exit */
    while (waitpid(pid, &status, 0) < 0)
    {
        if (errno == EINTR)
        {
            continue;
        }

        perror("waitpid failed");
        goto cleanup;
    }

    printf("GUI process terminated.\n");

cleanup:

    /* Stop the UART RX thread */
    if (rx_created)
    {
        pthread_cancel(rx_thread);
        pthread_join(rx_thread, NULL);
    }

    /* Stop the UART TX thread */
    if (tx_created)
    {
        pthread_cancel(tx_thread);
        pthread_join(tx_thread, NULL);
    }

    /* Stop the GUI if thread creation failed */
    if (rx_created == 0 || tx_created == 0)
    {
        kill(pid, SIGTERM);

        while (waitpid(pid, &status, 0) < 0 &&
               errno == EINTR)
        {
        }
    }

    printf("Temp System terminated.\n");

    return EXIT_SUCCESS;
}
