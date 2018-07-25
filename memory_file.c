#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LOG_FILE_SIZE   26
#define NUM_FILES       5

#ifdef __MEMORY_FILE_H__
#define decl static
#else
#define decl
#endif

#ifndef FILE
#define FILE    void
#endif
#ifndef EOF
#define EOF     ((int) -1)
#endif
#ifndef SEEK_SET
#define SEEK_SET    ((int) 0)
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    ((int) 1)
#endif
#ifndef SEEK_END
#define SEEK_END    ((int) 2)
#endif
#define FILE_SIZE   (1 << LOG_FILE_SIZE)

typedef unsigned char byte;
typedef const char *string;

typedef struct mem_file {
    long int seek;
    string   name;
    byte     data[1 << LOG_FILE_SIZE];
} mem_file;

// static data is zero-initialized
static char     s_buffer[FILE_SIZE * 3 + FILE_SIZE / 64];
static mem_file s_files[NUM_FILES];
static long int s_fd;

decl FILE *mem_fopen(string filename, string mode) {
    (void) mode;
    FILE *ret = NULL;
    mem_file *file = NULL;
    if (NULL != filename && s_fd < NUM_FILES) {
        file = &s_files[s_fd++];
        file->name = filename;
        ret = (FILE *) file;
    }
    return ret;
}

decl void mem_fclose(FILE *fd) {
    (void) fd;
}

decl int mem_fgetc(FILE *fd) {
    int ret = EOF;
    mem_file *file = (mem_file *) fd;
    if (NULL != file && file->seek < FILE_SIZE) {
        ret = (int) file->data[file->seek++];
    }
    return ret;
}

decl int mem_fputc(int chr, FILE *fd) {
    int ret = EOF;
    mem_file *file = (mem_file *) fd;
    if (NULL != file && file->seek < FILE_SIZE) {
        file->data[file->seek++] = (byte) chr;
        ret = chr;
    }
    return ret;
}

decl int mem_fscanf(FILE *fd, string fmt, ...) {
    int ret = EOF;
    mem_file *file = (mem_file *) fd;
    va_list args;
    if (NULL != file) {
        va_start(args, fmt);
        ret = vsscanf((string) (file->data + file->seek), fmt, args);
        va_end(args);
    }
    return ret;
}

decl int mem_fprintf(FILE *fd, string fmt, ...) {
    int ret = EOF;
    mem_file *file = (mem_file *) fd;
    va_list args;
    if (NULL != file) {
        va_start(args, fmt);
        ret = vsprintf((char *) (file->data + file->seek), fmt, args);
        file->seek += ret;
        va_end(args);
    }
    return ret;
}

decl int mem_fseek(FILE *fd, long int offset, int origin) {
    int ret = EOF;
    mem_file *file = (mem_file *) fd;
    if (NULL != file) {
        switch (origin) {
        case SEEK_SET:
            file->seek = offset;
            break;
        case SEEK_CUR:
            file->seek += offset;
            break;
        case SEEK_END:
            file->seek = FILE_SIZE + offset;
            break;
        }
        ret = 0;
    }
    return ret;
}

decl long int mem_ftell(FILE *fd) {
    long int ret = (long int) EOF;
    mem_file *file = (mem_file *) fd;
    if (NULL != file && file->seek < FILE_SIZE) {
        ret = file->seek;
    }
    return ret;
}

decl void mem_rewind(FILE *fd) {
    mem_file *file = (mem_file *) fd;
    if (NULL != file) {
        file->seek = 0;
    }
}

decl size_t mem_fread(void *ptr, size_t size, size_t count, FILE *fd) {
    size_t ret = 0;
    size_t num = 0;
    mem_file *file = (mem_file *) fd;
    if (NULL != file) {
        num = size * count;
        memcpy(ptr, file->data + file->seek, num);
        file->seek += num;
        ret = count;
    }
    return ret;
}

decl size_t mem_fwrite(void *ptr, size_t size, size_t count, FILE *fd) {
    size_t ret = 0;
    size_t num = 0;
    mem_file *file = (mem_file *) fd;
    if (NULL != file) {
        num = size * count;
        memcpy(file->data + file->seek, ptr, num);
        file->seek += num;
        ret = count;
    }
    return ret;
}

void ascii_dump(FILE *fd, FILE *out) {
    mem_file *file = (mem_file *) fd;
    if (NULL != file) {
        fwrite(file->data, sizeof(byte), (size_t) file->seek, out);
    }
}


static char hex_chr[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};

void hex_dump(FILE *fd, FILE *out) {
    mem_file *file = (mem_file *) fd;

    byte *data = NULL;
    size_t seek = 0;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t w = 0;
    if (NULL != file) {
        data = file->data;
        seek = (size_t) file->seek;
        for (; k < seek;) {
            for (i = 0; k < seek && i < (1 << 18);) {
                for (j = 0; k < seek && j < 64; ++j, ++i, ++k) {
                    w = i * 3 + i / 64;
                    s_buffer[w + 0] = hex_chr[data[k] / 16];
                    s_buffer[w + 1] = hex_chr[data[k] % 16];
                    s_buffer[w + 2] = ',';
                }
                s_buffer[i * 3 + (i - 1) / 64] = '\n';
            }
            fwrite(s_buffer, sizeof(byte), i, out);
            fflush(out);
        }
    }
}

void heavy_dump(FILE *fd, FILE *out) {
    mem_file *file = (mem_file *) fd;

    byte *data = NULL;
    size_t seek = 0;
    size_t i = 0;
    size_t j = 0;
    if (NULL != file) {
        data = file->data;
        seek = (size_t) file->seek;
        for (i = 0; i < seek; ++i) {
            if (i % 64 == 0) {
                s_buffer[j++] = '\n';
            }
            s_buffer[j++] = hex_chr[data[i] / 16];
            s_buffer[j++] = hex_chr[data[i] % 16];
            s_buffer[j++] = ',';
        }
        fwrite(s_buffer, sizeof(byte), sizeof(s_buffer), out);
    }
}
