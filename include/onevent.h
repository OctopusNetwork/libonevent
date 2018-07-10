#ifndef __ONC_EVENT____H__
#define __ONC_EVENT____H__

#ifdef __cplusplus
extern "C" {
#endif

void   *onc_event_create(int internal_event, int events,
            int need_drain, int read_end, int write_end);
void    onc_event_destroy(void *event);
int     onc_event_enroll(void *event, void *lfds, int *max_fd);
int     onc_event_wait(void *event, void *lfds, int millseconds);
int     onc_event_happen(void *event, void *lfds, int events);
/* Dynamic event support */
int     onc_event_add(void *event, int evbit);
int     onc_event_del(void *event, int evbit);
int     onc_event_process(void *event, void *lfds);
int     onc_event_wakeup(void *event);

#ifdef __cplusplus
}
#endif

#endif
