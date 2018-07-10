#ifndef __OCNET_LFDS_INTERNAL____H__
#define __OCNET_LFDS_INTERNAL____H__

#include "onlfds.h"

#ifdef __cplusplus
extern "C" {
#endif

int     ocnet_lfds_readable(ocnet_lfds_t *lfds, int fd);
int     ocnet_lfds_writable(ocnet_lfds_t *lfds, int fd);
int     ocnet_lfds_error(ocnet_lfds_t *lfds, int fd);
int     ocnet_lfds_enroll(ocnet_lfds_t *lfds, int fd, int flags);
void    ocnet_lfds_zero(ocnet_lfds_t *lfds);
int     ocnet_lfds_wait(ocnet_lfds_t *lfds, int nfds, int millseconds);

#ifdef __cplusplus
}
#endif

#endif
