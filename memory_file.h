#ifndef __MEMORY_FILE_H__
#define __MEMORY_FILE_H__

#include <stdint.h>

#ifdef REPLACE_CALLS
#define fopen   mem_fopen
#define fclose  mem_fclose
#define fgetc   mem_fgetc
#define fputc   mem_fputc
#define fscanf  mem_fscanf
#define fprintf mem_fprintf
#define fseek   mem_fseek
#define ftell   mem_ftell
#define rewind  mem_rewind
#define fread   mem_fread
#define fwrite  mem_fwrite
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *   mem_fopen  (const char *filename, const char *mode);
extern void     mem_fclose (FILE *file);
extern int      mem_fgetc  (FILE *file);
extern int      mem_fputc  (int chr, FILE *file);
extern int      mem_fscanf (FILE *file, const char *fmt, ...);
extern int      mem_fprintf(FILE *file, const char *fmt, ...);
extern int      mem_fseek  (FILE *file, long int offset, int origin);
extern long int mem_ftell  (FILE *file);
extern void     mem_rewind (FILE *file);
extern size_t   mem_fread  (void *ptr, size_t size, size_t count, FILE *file);
extern size_t   mem_fwrite (void *ptr, size_t size, size_t count, FILE *file);

extern void ascii_dump(FILE *file, FILE *out);
extern void heavy_dump(FILE *file, FILE *out);
extern void   hex_dump(FILE *file, FILE *out);

#ifdef __cplusplus
}
#endif

#endif
