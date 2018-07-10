#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "ocnet_thread.h"

#include "onevent.h"

#include "onlfds.h"

static int g_running = 0;

static void *__event_handler(void *arg)
{
    void *event = arg;
    int rc = 0;
    void *lfds = ocnet_lfds_new();

    do {
        rc = ocnet_event_wait(event, lfds, 100000);
        printf("Event is wakeuped rc=%d\n", rc);
    } while (1 == g_running);

    ocnet_lfds_del(lfds);

    return NULL;
}

static void __sigint_handler(int sig)
{
    g_running = 0;
}

int main(int argc, char *argv[])
{
    void *thread = NULL;
    void *event = ocnet_event_create(1,
            ocnet_EVENT_READ | ocnet_EVENT_ERROR,
            1, 0, 0);

    if (NULL == event) {
        return -1;
    }

    g_running = 1;

    thread = ocnet_thread_create(__event_handler, event);
    if (NULL == thread) {
        ocnet_event_destroy(event);
        return -1;
    }

    signal(SIGINT, __sigint_handler);

    do {
        ocnet_event_wakeup(event);
        sleep(1);
    } while (1 == g_running);

    ocnet_event_wakeup(event);
    ocnet_thread_join(thread);
    ocnet_event_destroy(event);

    return 0;
}
