/*
 * data/signal.csv -> output/signal_filtered.csv
 *
 * Для кожного фільтра збираємо окремий exe-файл, наприклад:
 *   make FILTER=ema OUT=pipeline_ema.exe
 *
 * Потім запускаємо цей exe-файл і передаємо йому вхідний CSV:
 *   pipeline_ema.exe data/signal.csv
 *
 * Програма читає рядки CSV, фільтрує числову колонку value
 * і записує новий CSV з додатковою колонкою "filtered".
 *
 * Фільтр вибирається під час збірки:
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
    /* Ці шляхи використовуються, якщо запустити програму без аргументів. */
    const char *path = "data/signal.csv";
    const char *out_path = "output/signal_filtered.csv";

    if (argc >= 2) {
        /* Перший аргумент командного рядка замінює дефолтний вхідний файл. */
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

    /*
     * Читаємо заголовок і всі рядки CSV.
     * samples[] містить числові значення, які підуть у фільтр,
     * а body_lines[] зберігає оригінальний текст рядків для запису результату.
     */
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

        /*
         * filter_init() скидає стан вибраного фільтра.
         * filter_push() обробляє по одному семплу і повертає відфільтроване значення.
         */
        filter_init();
        for (i = 0; i < n; ++i) {
            filtered[i] = filter_push(samples[i]);
        }
    }

    /* Записуємо оригінальні рядки CSV плюс нову колонку з результатом фільтра. */
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
