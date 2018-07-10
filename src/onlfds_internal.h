#ifndef __OCNET_LFDS_INTERNAL____H__
#define __OCNET_LFDS_INTERNAL____H__

#include "onlfds.h"

#ifdef __cplusplus
extern "C" {
#endif

int     ocnet_lfds_readable(void *lfds, int fd);
int     ocnet_lfds_writable(void *lfds, int fd);
int     ocnet_lfds_error(void *lfds, int fd);
int     ocnet_lfds_enroll(void *lfds, int fd, int flags);
void    ocnet_lfds_zero(void *lfds);
int     ocnet_lfds_wait(void *lfds, int nfds, int millseconds);

#ifdef __cplusplus
}
#endif

#endif
