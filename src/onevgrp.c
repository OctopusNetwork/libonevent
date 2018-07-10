#include <stdlib.h>
#include <stdio.h>

#include "ocnet_malloc.h"

#include "onevent.h"
#include "onevgrp.h"

#include "onlfds_internal.h"

typedef struct {
    unsigned char   used:   1;
    void           *event;
} ocnet_evslot_s_t;

typedef struct {
    int             max_events;
    ocnet_evslot_s_t *evslots;
} ocnet_evgrp_s_t;

void *ocnet_evgrp_create(int max_events)
{
    ocnet_evgrp_s_t *evgrp = NULL;

    evgrp = ocnet_malloc(sizeof(ocnet_evgrp_s_t));
    if (NULL == evgrp) {
        return NULL;
    }

    evgrp->evslots = ocnet_malloc(max_events * sizeof(ocnet_evslot_s_t));
    if (NULL == evgrp->evslots) {
        ocnet_free(evgrp);
        return NULL;
    }

    evgrp->max_events = max_events;
    ocnet_memset(evgrp->evslots, 0x0, max_events * sizeof(ocnet_evslot_s_t));

    return (void *)evgrp;
}

void ocnet_evgrp_destroy(void *evgrp)
{
    ocnet_evgrp_s_t *eg = (ocnet_evgrp_s_t *)evgrp;

    ocnet_free(eg->evslots);
    ocnet_free(eg);
}

int ocnet_evgrp_event_add(void *evgrp, void *event)
{
    int i = 0;
    ocnet_evgrp_s_t *eg = (ocnet_evgrp_s_t *)evgrp;

    for (i = 0; i < eg->max_events; i++) {
        ocnet_evslot_s_t *evslot = &eg->evslots[i];
        if (0 == evslot->used) {
            evslot->event = event;
            evslot->used = 1;
            return 0;
        }
    }

    return -1;
}

int ocnet_evgrp_event_del(void *evgrp, void *event)
{
    int i = 0;
    ocnet_evgrp_s_t *eg = (ocnet_evgrp_s_t *)evgrp;

    for (i = 0; i < eg->max_events; i++) {
        ocnet_evslot_s_t *evslot = &eg->evslots[i];
        if (1 == evslot->used && event == evslot->event) {
            evslot->event = NULL;
            evslot->used = 0;
            return 0;
        }
    }

    return -1;
}

static int __event_enroll(ocnet_evgrp_s_t *evgrp, void *lfds)
{
    int i = 0;
    int max_fd = 0;

    for (i = 0; i < evgrp->max_events; i++) {
        ocnet_evslot_s_t *evslot = &evgrp->evslots[i];
        if (1 == evslot->used) {
            ocnet_event_enroll(evslot->event, lfds, &max_fd);
        }
    }

    return max_fd + 1;
}

static int __event_process(ocnet_evgrp_s_t *evgrp, void *lfds)
{
    int i = 0;

    for (i = 0; i < evgrp->max_events; i++) {
        ocnet_evslot_s_t *evslot = &evgrp->evslots[i];
        if (1 == evslot->used) {
            ocnet_event_process(evslot->event, lfds);
        }
    }

    return 0;
}

static int __event_wakeup(ocnet_evgrp_s_t *evgrp)
{
    int i = 0;

    for (i = 0; i < evgrp->max_events; i++) {
        ocnet_evslot_s_t *evslot = &evgrp->evslots[i];
        if (1 == evslot->used) {
            ocnet_event_wakeup(evslot->event);
        }
    }

    return 0;
}

int ocnet_evgrp_wait(void *evgrp, int millseconds, void *lfds)
{
    ocnet_evgrp_s_t *eg = (ocnet_evgrp_s_t *)evgrp;
    int nfds = 0;
    int rc = 0;

    ocnet_lfds_zero(lfds);
    nfds = __event_enroll(eg, lfds);

    rc = ocnet_lfds_wait(lfds, nfds, millseconds);
    if (rc < 0) {
        /* Error: let upper process */
    } else if (0 < rc) {
        if (__event_process(eg, lfds) < 0) {
            rc = 0;
        }
    } else {
        /* Timeout: let upper process */
    }

    return rc;
}

int ocnet_evgrp_wakeup(void *evgrp)
{
    return __event_wakeup(evgrp);
}
