
#ifndef __DFREETYPE_H__
#define __DFREETYPE_H__

#include <ft_engine.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    ft_engine *fte;
    fte_info *fti;
}ft_font;


ft_font *ft_open(const char *font, uint16_t size);
void ft_close(ft_font *f);

#ifdef __cplusplus
}
#endif

#endif

