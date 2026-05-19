#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <stddef.h>

/* t,value from body_lines + one filtered column. */
int csv_writer_write_with_filtered_column(const char *out_path, const char *header_line,
    const char *const *body_lines, const double *filtered, size_t nrows);

#endif
