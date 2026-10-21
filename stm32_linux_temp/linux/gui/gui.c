#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/select.h>

#define RX_FIFO_PATH "/tmp/stm32_rx_fifo"
#define TX_FIFO_PATH "/tmp/stm32_tx_fifo"
#define RX_BUFFER_SIZE 128

GtkWidget *power_label;
GtkWidget *state_label;
GtkWidget *temperature_label;
GtkWidget *entry;

typedef struct
{
    GtkWidget *label;
    char *value;
    char *name;
} GUI_UPDATE;

// Update a label
gboolean update_label(gpointer data)
{
    GUI_UPDATE *update = (GUI_UPDATE *)data;
    char display[300];

    snprintf(display, sizeof(display), "%s : %s",
             update->name, update->value);

    gtk_label_set_text(GTK_LABEL(update->label), display);

    free(update->value);
    free(update);

    return G_SOURCE_REMOVE;
}

// Schedule a label update
static void schedule_label_update(
    GtkWidget *label,
    const char *name,
    const char *value)
{
    GUI_UPDATE *update = malloc(sizeof(GUI_UPDATE));

    if (update == NULL)
        return;

    update->value = strdup(value);

    if (update->value == NULL)
    {
        free(update);
        return;
    }

    update->label = label;
    update->name = (char *)name;

    g_idle_add(update_label, update);
}

// Process a received line
static void process_rx_line(char *line)
{
    char command[32];
    char value[RX_BUFFER_SIZE];

    if (sscanf(line, "%31s %127[^\n]", command, value) != 2)
        return;

    if (strcmp(command, "power") == 0 ||
        strcmp(command, "POWER") == 0)
    {
        schedule_label_update(power_label, "POWER", value);
    }
    else if (strcmp(command, "state") == 0 ||
             strcmp(command, "STATE") == 0)
    {
        schedule_label_update(state_label, "STATE", value);
    }
    else if (strcmp(command, "temperature") == 0 ||
             strcmp(command, "TEMPERATURE") == 0)
    {
        schedule_label_update(
            temperature_label, "TEMPERATURE", value);
    }
}

// Receive data from FIFO
void *fifo_rx_thread(void *arg)
{
    int fd;
    fd_set readfds;
    char buffer[RX_BUFFER_SIZE];
    int index = 0;
    char ch;

    (void)arg;

    // Open RX FIFO
    fd = open(RX_FIFO_PATH, O_RDONLY);

    if (fd < 0)
    {
        perror("RX FIFO open failed");
        return NULL;
    }

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        // Wait for FIFO data
        int ret = select(fd + 1, &readfds, NULL, NULL, NULL);

        if (ret < 0)
        {
            if (errno == EINTR)
                continue;

            perror("RX FIFO select failed");
            break;
        }

        if (!FD_ISSET(fd, &readfds))
            continue;

        ssize_t n = read(fd, &ch, 1);

        if (n == 1)
        {
            if (ch == '\n')
            {
                buffer[index] = '\0';

                if (index > 0)
                    process_rx_line(buffer);

                index = 0;
            }
            else if (ch != '\r')
            {
                if (index < RX_BUFFER_SIZE - 1)
                {
                    buffer[index++] = ch;
                }
                else
                {
                    index = 0;
                }
            }
        }
        else if (n == 0)
        {
            // Wait for the next FIFO writer
            close(fd);

            fd = open(RX_FIFO_PATH, O_RDONLY);

            if (fd < 0)
            {
                perror("RX FIFO reopen failed");
                return NULL;
            }

            index = 0;
        }
        else if (errno != EINTR)
        {
            perror("RX FIFO read failed");
            break;
        }
    }

    close(fd);
    return NULL;
}

// Send a command to FIFO
static int fifo_tx_send(const char *message)
{
    int fd;
    size_t length = strlen(message);
    size_t sent = 0;

    // Open TX FIFO
    fd = open(TX_FIFO_PATH, O_WRONLY | O_NONBLOCK);

    if (fd < 0)
    {
        perror("TX FIFO open failed");
        return 0;
    }

    // Send the message
    while (sent < length)
    {
        ssize_t n = write(fd, message + sent, length - sent);

        if (n > 0)
        {
            sent += (size_t)n;
        }
        else if (n < 0 && errno == EINTR)
        {
            continue;
        }
        else
        {
            perror("TX FIFO write failed");
            close(fd);
            return 0;
        }
    }

    // Send newline
    if (write(fd, "\n", 1) != 1)
    {
        perror("TX FIFO newline failed");
        close(fd);
        return 0;
    }

    close(fd);
    return 1;
}

// Handle the set button
void on_button_clicked(GtkWidget *button, gpointer data)
{
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    char message[RX_BUFFER_SIZE];
    char *input;
    char *endptr;
    double temperature;

    (void)button;
    (void)data;

    // Copy and trim input
    input = g_strdup(text);

    if (input == NULL)
        return;

    g_strstrip(input);

    if (input[0] == '\0')
    {
        g_free(input);
        return;
    }

    // Check power commands
    if (g_ascii_strcasecmp(input, "ON") == 0)
    {
        snprintf(message, sizeof(message), "POWER ON");
    }
    else if (g_ascii_strcasecmp(input, "OFF") == 0)
    {
        snprintf(message, sizeof(message), "POWER OFF");
    }
    // Check temperature input
    else
    {
        errno = 0;
        temperature = g_ascii_strtod(input, &endptr);

        if (endptr == input ||
            *endptr != '\0' ||
            errno == ERANGE)
        {
            printf("Invalid input: %s\n", input);
            fflush(stdout);
            g_free(input);
            return;
        }

        snprintf(message, sizeof(message), "TEMP %s", input);
    }

    // Send command to FIFO
    if (fifo_tx_send(message))
    {
        printf("TX: %s\n", message);
        fflush(stdout);
    }

    g_free(input);
}

// Add a CSS class
void add_css_class(GtkWidget *widget, const char *class_name)
{
    gtk_style_context_add_class(
        gtk_widget_get_style_context(widget),
        class_name);
}

int main(int argc, char *argv[])
{
    pthread_t rx_thread;

    gtk_init(&argc, &argv);

    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;

    if (!gtk_css_provider_load_from_path(
            provider, "/home/konghyoil/study/linux/temp_system/gui/style.css", &error))
    {
        g_printerr("CSS error: %s\n", error->message);
        g_clear_error(&error);
    }

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);

    // Create window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Temperature System");
    gtk_window_set_default_size(GTK_WINDOW(window), 520, 520);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create title
    GtkWidget *title = gtk_label_new("STM32 CONTROL PANEL");
    add_css_class(title, "dashboard-title");

    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 4);

    // Create power card
    GtkWidget *power_card =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

    add_css_class(power_card, "frame-card");

    GtkWidget *power_title = gtk_label_new("POWER");
    add_css_class(power_title, "section-title");

    power_label = gtk_label_new("OFF");
    add_css_class(power_label, "status-value");

    gtk_box_pack_start(GTK_BOX(power_card),
                       power_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(power_card),
                       power_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box),
                       power_card, TRUE, TRUE, 0);

    // Create state card
    GtkWidget *state_card =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

    add_css_class(state_card, "frame-card");

    GtkWidget *state_title = gtk_label_new("SYSTEM STATE");
    add_css_class(state_title, "section-title");

    state_label = gtk_label_new("IDLE");
    add_css_class(state_label, "status-value");

    gtk_box_pack_start(GTK_BOX(state_card),
                       state_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(state_card),
                       state_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box),
                       state_card, TRUE, TRUE, 0);

    // Create temperature card
    GtkWidget *temperature_card =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

    add_css_class(temperature_card, "frame-card");

    GtkWidget *temperature_title =
        gtk_label_new("TEMPERATURE");

    add_css_class(temperature_title, "section-title");

    temperature_label = gtk_label_new("--.- C");
    add_css_class(temperature_label, "status-value");

    gtk_box_pack_start(GTK_BOX(temperature_card),
                       temperature_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(temperature_card),
                       temperature_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box),
                       temperature_card, TRUE, TRUE, 0);

    // Create input controls
    GtkWidget *input_box =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

    entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(entry), "input temperature");

    GtkWidget *button = gtk_button_new_with_label("set");

    gtk_box_pack_start(GTK_BOX(input_box),
                       entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(input_box),
                       button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box),
                       input_box, FALSE, FALSE, 0);

    // Connect signals
    g_signal_connect(button, "clicked",
                     G_CALLBACK(on_button_clicked), NULL);

    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    // Start RX thread
    if (pthread_create(&rx_thread, NULL,
                       fifo_rx_thread, NULL) != 0)
    {
        fprintf(stderr, "RX thread creation failed\n");
        return 1;
    }

    pthread_detach(rx_thread);

    // Run GUI
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
