#ifndef __ONC_EVENT_GROUP____H__
#define __ONC_EVENT_GROUP____H__

#ifdef __cplusplus
extern "C" {
#endif

void   *onc_evgrp_create(int max_events);
void    onc_evgrp_destroy(void *evgrp);
int     onc_evgrp_event_add(void *evgrp, void *event);
int     onc_evgrp_event_del(void *evgrp, void *event);
int     onc_evgrp_wait(void *evgrp, int millseconds, void *lfds);
int     onc_evgrp_wakeup(void *evgrp);

#ifdef __cplusplus
}
#endif

#endif
