

#ifndef __FT_ENGINE_H__
#define __FT_ENGINE_H__

#include <inttypes.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t top;
    uint16_t left;
    uint16_t xadvance;
    uint16_t yadvance;
    uint8_t* bitmap;
}fte_symbol;


typedef struct fte_info fte_info;
typedef struct ft_engine ft_engine;

ft_engine * fte_open(const char *file);
void fte_close(ft_engine *fte, int);
const char *fte_get_name(ft_engine *fte);
int fte_clear_cache(ft_engine *fte);

fte_info *fte_open_cache_by_metrics(ft_engine *fte, uint16_t size);
int fte_close_cache_metrics(fte_info *fti);
int fte_cache_symbol(fte_info *fti, uint16_t letter);
fte_symbol *fte_get_symbol(fte_info *fti, uint16_t letter);
int fte_clear_cache_by_metrics(fte_info *fti);

FT_Library fte_library_handler(ft_engine *fte);
FT_Face fte_face_handler(ft_engine *fte);
void fte_set_flags(ft_engine *fte, FT_Int32 flag);
FT_Int32 fte_flags(ft_engine *fte);

#ifdef __cplusplus
}
#endif

#endif
