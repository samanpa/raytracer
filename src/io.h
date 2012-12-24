#ifndef BPRAY_IO_H
#define BPRAY_IO_H

#include <stdio.h>
FILE *bpray_file_open(const char *filename, const char *mode);
void bpray_library_path_add (const char *path);

#endif

