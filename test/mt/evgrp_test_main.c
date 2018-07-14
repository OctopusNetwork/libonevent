#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

#include "libonplatform/ocnet_thread.h"

#include "libonevent/onevent.h"
#include "libonevent/onevgrp.h"
#include "libonevent/onlfds.h"

static int g_running = 0;

static void *__evgrp_handler(void *arg)
{
    ocnet_evgrp_t *evgrp = arg;
    ocnet_lfds_t *lfds = ocnet_lfds_new();

    do {
        ocnet_evgrp_wait(evgrp, 100000000, lfds);
        printf("Event group is wakeuped\n");
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
    ocnet_thread_t *thread = NULL;
    ocnet_event_t *event1 = NULL;
    ocnet_event_t *event2 = NULL;
    ocnet_evgrp_t *evgrp = NULL;
    int rc = 0;

    event1 = ocnet_event_create(1,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR,
            1, 0, 0);
    if (NULL == event1) {
        return -1;
    }

    event2 = ocnet_event_create(1,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR,
            1, 0, 0);
    if (NULL == event2) {
        rc = -1;
        goto L_ERROR_EVENT2_CREATE;
    }

    evgrp = ocnet_evgrp_create(32);
    if (NULL == evgrp) {
        rc = -1;
        goto L_ERROR_EVGRP_CREATE;
    }

    ocnet_evgrp_event_add(evgrp, event1);
    ocnet_evgrp_event_add(evgrp, event2);

    g_running = 1;

    thread = ocnet_thread_create(__evgrp_handler, evgrp);
    if (NULL == thread) {
        rc = -1;
        goto L_ERROR_THREAD_CREATE;
    }

    signal(SIGINT, __sigint_handler);

    do {
        printf("Wakeup event1\n");
        ocnet_event_wakeup(event1);
        sleep(1);
        printf("Wakeup event2\n");
        ocnet_event_wakeup(event2);
        sleep(1);
    } while (1 == g_running);

    ocnet_evgrp_wakeup(evgrp);
    ocnet_thread_join(thread);

    ocnet_evgrp_event_del(evgrp, event1);
    ocnet_evgrp_event_del(evgrp, event2);

L_ERROR_THREAD_CREATE:
    ocnet_evgrp_destroy(evgrp);
L_ERROR_EVGRP_CREATE:
    ocnet_event_destroy(event2);
L_ERROR_EVENT2_CREATE:
    ocnet_event_destroy(event1);
    return rc;
}
