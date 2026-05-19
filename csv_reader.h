#ifndef CSV_READER_H
#define CSV_READER_H

#include <stddef.h>
#include <stdio.h>

/* Returns 0 on success; -1 on error; -2 if the line does not exist. Caller must free() *out_line. */
int csv_reader_read_row_text(const char *path, unsigned line_number_1based, char **out_line);

/*
 * Reads the whole signal file: line 1 -> *out_header (chomped), then each non-empty data line
 * until a blank line or EOF. *out_body_lines are full line copies (time,value as in the file);
 * *out_raw is the value after the first comma per row (for filtering). Caller must free *out_header,
 * each out_body_lines[i], out_body_lines, and *out_raw.
 * Returns 0 on success, -1 on I/O/alloc error, -2 if there is no header or no data rows.
 */
int csv_reader_read_signal_with_body(const char *path, char **out_header, char ***out_body_lines,
    double **out_raw, size_t *out_n);

#endif
