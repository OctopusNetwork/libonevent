#ifndef __ONC_EVENT____H__
#define __ONC_EVENT____H__

#include "onlfds.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ocnet_event;
typedef struct ocnet_event  ocnet_event_t;

ocnet_event_t  *ocnet_event_create(int internal_event, int events,
                    int need_drain, int read_end, int write_end);
void            ocnet_event_destroy(ocnet_event_t *event);
int             ocnet_event_enroll(ocnet_event_t *event, ocnet_lfds_t *lfds, int *max_fd);
int             ocnet_event_wait(ocnet_event_t *event, ocnet_lfds_t *lfds, int millseconds);
int             ocnet_event_happen(ocnet_event_t *event, ocnet_lfds_t *lfds, int events);
/* Dynamic event support */
int             ocnet_event_add(ocnet_event_t *event, int evbit);
int             ocnet_event_del(ocnet_event_t *event, int evbit);
int             ocnet_event_process(ocnet_event_t *event, ocnet_lfds_t *lfds);
int             ocnet_event_wakeup(ocnet_event_t *event);

#ifdef __cplusplus
}
#endif

#endif
