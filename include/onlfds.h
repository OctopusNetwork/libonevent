#ifndef __ONC_LFDS____H__
#define __ONC_LFDS____H__

#define OCNET_EVENT_READ      0x00000001
#define OCNET_EVENT_WRITE     0x00000002
#define OCNET_EVENT_ERROR     0x00000004

#ifdef __cplusplus
extern "C" {
#endif

struct ocnet_lfds;
typedef struct ocnet_lfds   ocnet_lfds_t;

ocnet_lfds_t   *ocnet_lfds_new(void);
void            ocnet_lfds_del(ocnet_lfds_t *lfds);

#ifdef __cplusplus
}
#endif

#endif
