#ifndef __ONC_LFDS____H__
#define __ONC_LFDS____H__

#define ONC_EVENT_READ      0x00000001
#define ONC_EVENT_WRITE     0x00000002
#define ONC_EVENT_ERROR     0x00000004

#ifdef __cplusplus
extern "C" {
#endif

void   *onc_lfds_new(void);
void    onc_lfds_del(void *lfds);

#ifdef __cplusplus
}
#endif

#endif
