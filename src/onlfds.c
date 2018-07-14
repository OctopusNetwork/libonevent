#include <sys/select.h>

#include "libonplatform/ocnet_malloc.h"

#include "onlfds_internal.h"

struct ocnet_lfds {
    fd_set          rfds;
    fd_set          wfds;
    fd_set          efds;
};

ocnet_lfds_t *ocnet_lfds_new(void)
{
    return ocnet_malloc(sizeof(ocnet_lfds_t));
}

void ocnet_lfds_del(ocnet_lfds_t *lfds)
{
    ocnet_free(lfds);
}

int ocnet_lfds_readable(ocnet_lfds_t *lfds, int fd)
{
    ocnet_lfds_t *ocnet_lfds = (ocnet_lfds_t *)lfds;
    return FD_ISSET(fd, &ocnet_lfds->rfds);
}

int ocnet_lfds_writable(ocnet_lfds_t *lfds, int fd)
{
    ocnet_lfds_t *ocnet_lfds = (ocnet_lfds_t *)lfds;
    return FD_ISSET(fd, &ocnet_lfds->wfds);
}

int ocnet_lfds_error(ocnet_lfds_t *lfds, int fd)
{
    ocnet_lfds_t *ocnet_lfds = (ocnet_lfds_t *)lfds;
    return FD_ISSET(fd, &ocnet_lfds->efds);
}

int ocnet_lfds_enroll(ocnet_lfds_t *lfds, int fd, int events)
{
    ocnet_lfds_t *ocnet_lfds = (ocnet_lfds_t *)lfds;
    int ltype_matched = 0;

    if (0 != (OCNET_EVENT_READ & events)) {
        FD_SET(fd, &ocnet_lfds->rfds);
        ltype_matched = 1;
    }
    if (0 != (OCNET_EVENT_WRITE & events)) {
        FD_SET(fd, &ocnet_lfds->wfds);
        ltype_matched = 1;
    }
    if (0 != (OCNET_EVENT_ERROR & events)) {
        FD_SET(fd, &ocnet_lfds->efds);
        ltype_matched = 1;
    }

    if (0 == ltype_matched) {
        return -1;
    }

    return 0;
}

void ocnet_lfds_zero(ocnet_lfds_t *lfds)
{
    ocnet_lfds_t *ocnet_lfds = (ocnet_lfds_t *)lfds;
    FD_ZERO(&ocnet_lfds->rfds);
    FD_ZERO(&ocnet_lfds->wfds);
    FD_ZERO(&ocnet_lfds->efds);
}

int ocnet_lfds_wait(ocnet_lfds_t *lfds, int nfds, int millseconds)
{
    struct timeval tv;
    ocnet_lfds_t *ocnet_lfds = (ocnet_lfds_t *)lfds;

    tv.tv_sec = millseconds / 1000;
    tv.tv_usec = millseconds % 1000 * 1000;

    return select(nfds, &ocnet_lfds->rfds,
            &ocnet_lfds->wfds, &ocnet_lfds->efds, &tv);
}
