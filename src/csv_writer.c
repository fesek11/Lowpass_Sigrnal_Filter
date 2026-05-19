#include "csv_writer.h"

#include <stdio.h>

int csv_writer_write_with_filtered_column(const char *out_path, const char *header_line,
    const char *const *body_lines, const double *filtered, size_t nrows)
{
    if (out_path == NULL || header_line == NULL || body_lines == NULL || filtered == NULL
        || nrows == 0) {
        return -1;
    }

    FILE *fp = fopen(out_path, "wb");
    if (fp == NULL) {
        return -1;
    }

    if (fprintf(fp, "%s,filtered\n", header_line) < 0) {
        fclose(fp);
        return -1;
    }

    for (size_t i = 0; i < nrows; ++i) {
        if (fprintf(fp, "%s,%.6f\n", body_lines[i], filtered[i]) < 0) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) != 0) {
        return -1;
    }

    return 0;
}
