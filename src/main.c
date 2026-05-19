/*
 * data/signal.csv -> output/signal_filtered.csv
 * Filter is selected at build time:
 *   make FILTER=ma
 *   make FILTER=ema
 *   make FILTER=median
 *   make FILTER=sg
 *
 * Run:
 *   pipeline [input.csv]
 */

#include <stdio.h>
#include <stdlib.h>

#include "csv_reader.h"
#include "csv_writer.h"
#include "filter.h"

int main(int argc, char *argv[])
{
    const char *path = "data/signal.csv";
    const char *out_path = "output/signal_filtered.csv";

    if (argc >= 2) {
        path = argv[1];
    }
    if (argc > 2) {
        fprintf(stderr, "usage: %s [input.csv]\n", argv[0]);
        return 1;
    }

    char *header = NULL;
    char **body_lines = NULL;
    double *samples = NULL;
    size_t n = 0;

    int rc = csv_reader_read_signal_with_body(path, &header, &body_lines, &samples, &n);
    if (rc != 0) {
        if (rc == -2) {
            fprintf(stderr, "%s: empty or no data rows\n", path);
        } else {
            perror(path);
        }
        return 1;
    }

    double *filtered = (double *)malloc(n * sizeof(double));
    if (filtered == NULL) {
        fprintf(stderr, "allocation failed\n");
        rc = 1;
    } else {
        size_t i;

        filter_init();
        for (i = 0; i < n; ++i) {
            filtered[i] = filter_push(samples[i]);
        }
    }

    if (rc == 0
        && csv_writer_write_with_filtered_column(out_path, header,
            (const char *const *)body_lines, filtered, n)
            != 0) {
        perror(out_path);
        rc = 1;
    }

    free(filtered);
    free(header);
    for (size_t i = 0; i < n; ++i) {
        free(body_lines[i]);
    }
    free(body_lines);
    free(samples);
    return rc;
}
