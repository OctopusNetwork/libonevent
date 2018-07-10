#include <unistd.h>
#include <stdlib.h>

#include "ocnet_malloc.h"
#include "ocnet_math.h"

#include "onevent.h"

#include "onlfds_internal.h"

typedef struct {
    unsigned char   need_drain:         1,
                    internal_event:     1,
                    internal_listen:    1;

    int             read_fd;
    int             write_fd;
    int             listen_events;
} ocnet_event_s_t;

void *ocnet_event_create(int internal_event, int events,
        int need_drain, int read_end, int write_end)
{
    ocnet_event_s_t *ev = NULL;
    int pipefd[2];

    if (0 != internal_event) {
        if (pipe(pipefd) < 0) {
            return NULL;
        }
    }

    ev = ocnet_malloc(sizeof(ocnet_event_s_t));
    if (NULL == ev) {
        goto L_ERROR_EVENT_MALLOC;
    }

    ev->need_drain = need_drain;
    ev->internal_event = internal_event;
    ev->listen_events = events;

    if (1 == internal_event) {
        ev->read_fd = pipefd[0];
        ev->write_fd = pipefd[1];
    } else {
        ev->read_fd = read_end;
        ev->write_fd = write_end;
    }

    return (void *)ev;

L_ERROR_EVENT_MALLOC:
    if (1 == internal_event) {
        close(pipefd[0]);
        close(pipefd[1]);
    }
    return NULL;
}

void ocnet_event_destroy(void *event)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;

    if (1 == ev->internal_event) {
        close(ev->read_fd);
        close(ev->write_fd);
    }
    ocnet_free(ev);
}

static int __event_drain(ocnet_event_s_t *event, void *lfds)
{
    char buf[1024] = {0};

    if (0 != ocnet_lfds_readable(lfds, event->read_fd)) {
        if (read(event->read_fd, buf, sizeof(buf)) < 0) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

int ocnet_event_process(void *event, void *lfds)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;

    if (1 == ev->need_drain) {
        return __event_drain(ev, lfds);
    }

    return 0;
}

int ocnet_event_enroll(void *event, void *lfds, int *max_fd)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;

    if (0 != (ev->listen_events & ocnet_EVENT_READ)) {
        ocnet_lfds_enroll(lfds, ev->read_fd, ocnet_EVENT_READ);
    }
    if (0 != (ev->listen_events & ocnet_EVENT_WRITE)) {
        ocnet_lfds_enroll(lfds, ev->write_fd, ocnet_EVENT_WRITE);
    }
    if (0 != (ev->listen_events & ocnet_EVENT_ERROR)) {
        ocnet_lfds_enroll(lfds, ev->write_fd, ocnet_EVENT_ERROR);
        ocnet_lfds_enroll(lfds, ev->read_fd, ocnet_EVENT_ERROR);
    }

    *max_fd = __MAX2(*max_fd, ev->read_fd);

    return 0;
}

int ocnet_event_happen(void *event, void *lfds, int events)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;
    int event_happen = 0;

    if (0 != (ev->listen_events & ocnet_EVENT_READ) &&
            0 != (events & ocnet_EVENT_READ)) {
        if (0 != ocnet_lfds_readable(lfds, ev->read_fd)) {
            event_happen |= ocnet_EVENT_READ;
        }
    }

    if (0 != (ev->listen_events & ocnet_EVENT_WRITE) &&
            0 != (events & ocnet_EVENT_WRITE)) {
        if (0 != ocnet_lfds_writable(lfds, ev->write_fd)) {
            event_happen |= ocnet_EVENT_WRITE;
        }
    }

    if (0 != (ev->listen_events & ocnet_EVENT_ERROR) &&
            0 != (events & ocnet_EVENT_ERROR)) {
        if (0 != ocnet_lfds_error(lfds, ev->read_fd) ||
                0 != ocnet_lfds_error(lfds, ev->write_fd)) {
            event_happen |= ocnet_EVENT_ERROR;
        }
    }

    return event_happen;
}

int ocnet_event_add(void *event, int evbit)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;
    ev->listen_events |= evbit;
    return 0;
}

int ocnet_event_del(void *event, int evbit)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;
    ev->listen_events &= ~evbit;
    return 0;
}

int ocnet_event_wait(void *event, void *lfds, int millseconds)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;
    int rc = 0;
    int max_fd = 0;

    ocnet_lfds_zero(lfds);
    ocnet_event_enroll(ev, lfds, &max_fd);

    rc = ocnet_lfds_wait(lfds, max_fd + 1, millseconds);
    if (rc < 0) {
        /* Error: let upper process */
    } else if (0 < rc) {
        if (ocnet_event_process(ev, lfds) < 0) {
            return -1;
        }
    } else {
        /* Timeout: let upper process */
    }

    return rc;
}

int ocnet_event_wakeup(void *event)
{
    ocnet_event_s_t *ev = (ocnet_event_s_t *)event;
    if (1 == ev->internal_event) {
        char c = 0x5A;
        return write(ev->write_fd, &c, 1);
    }
    return 0;
}
