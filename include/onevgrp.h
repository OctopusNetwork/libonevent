#ifndef __ONC_EVENT_GROUP____H__
#define __ONC_EVENT_GROUP____H__

#include "onlfds.h"
#include "onevent.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ocnet_evgrp;
typedef struct ocnet_evgrp  ocnet_evgrp_t;

ocnet_evgrp_t  *ocnet_evgrp_create(int max_events);
void            ocnet_evgrp_destroy(ocnet_evgrp_t *evgrp);
int             ocnet_evgrp_event_add(ocnet_evgrp_t *evgrp, ocnet_event_t *event);
int             ocnet_evgrp_event_del(ocnet_evgrp_t *evgrp, ocnet_event_t *event);
int             ocnet_evgrp_wait(ocnet_evgrp_t *evgrp, int millseconds, ocnet_lfds_t *lfds);
int             ocnet_evgrp_wakeup(ocnet_evgrp_t *evgrp);

#ifdef __cplusplus
}
#endif

#endif
