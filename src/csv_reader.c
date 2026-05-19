#include "csv_reader.h"

#include <stdlib.h>
#include <string.h>

enum { CSV_LINE_MAX = 4096 };

static void chomp(char *s)
{
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

int csv_reader_read_row_text(const char *path, unsigned line_number_1based, char **out_line)
{
    if (path == NULL || out_line == NULL || line_number_1based == 0) {
        return -1;
    }

    *out_line = NULL;

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }

    char buf[CSV_LINE_MAX];
    for (unsigned skip = 1; skip < line_number_1based; ++skip) {
        if (fgets(buf, (int)sizeof buf, fp) == NULL) {
            fclose(fp);
            return -2;
        }
    }

    if (fgets(buf, (int)sizeof buf, fp) == NULL) {
        fclose(fp);
        return -2;
    }

    chomp(buf);
    size_t len = strlen(buf) + 1;
    char *copy = (char *)malloc(len);
    fclose(fp);

    if (copy == NULL) {
        return -1;
    }
    memcpy(copy, buf, len);

    *out_line = copy;
    return 0;
}

typedef struct {
    /* Зберігаємо повний текст рядка, щоб writer міг додати нову колонку справа. */
    char *line;
    /* Числове значення з CSV-колонки value, яке передається у фільтр. */
    double raw;
} RowPair;

static int append_row(RowPair **items, size_t *n, size_t *cap, char *line_owned, double v)
{
    if (*n >= *cap) {
        size_t ncap = *cap == 0 ? 64U : *cap * 2U;
        RowPair *ni = (RowPair *)realloc(*items, ncap * sizeof(RowPair));
        if (ni == NULL) {
            free(line_owned);
            return -1;
        }
        *items = ni;
        *cap = ncap;
    }
    (*items)[*n].line = line_owned;
    (*items)[*n].raw = v;
    ++*n;
    return 0;
}

int csv_reader_read_signal_with_body(const char *path, char **out_header, char ***out_body_lines,
    double **out_raw, size_t *out_n)
{
    if (path == NULL || out_header == NULL || out_body_lines == NULL || out_raw == NULL
        || out_n == NULL) {
        return -1;
    }

    *out_header = NULL;
    *out_body_lines = NULL;
    *out_raw = NULL;
    *out_n = 0;

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }

    char buf[CSV_LINE_MAX];
    if (fgets(buf, (int)sizeof buf, fp) == NULL) {
        fclose(fp);
        return -2;
    }

    chomp(buf);
    size_t hlen = strlen(buf) + 1;
    char *hdr = (char *)malloc(hlen);
    if (hdr == NULL) {
        fclose(fp);
        return -1;
    }
    memcpy(hdr, buf, hlen);

    RowPair *items = NULL;
    size_t n = 0;
    size_t cap = 0;

    while (fgets(buf, (int)sizeof buf, fp) != NULL) {
        const char *p = buf;
        while (*p == ' ' || *p == '\t') {
            ++p;
        }
        if (*p == '\0' || *p == '\r' || *p == '\n') {
            break;
        }

        chomp(buf);
        /*
         * Reader бере значення після першої коми як семпл сигналу.
         * Якщо CSV вже має відфільтровані колонки, вони зберігаються в linecopy,
         * але не використовуються як вхідні дані для фільтра.
         */
        char *comma = strchr(buf, ',');
        if (comma == NULL) {
            continue;
        }

        char *end = NULL;
        double v = strtod(comma + 1, &end);
        if (end == comma + 1) {
            continue;
        }

        size_t blen = strlen(buf) + 1;
        char *linecopy = (char *)malloc(blen);
        if (linecopy == NULL) {
            for (size_t i = 0; i < n; ++i) {
                free(items[i].line);
            }
            free(items);
            free(hdr);
            fclose(fp);
            return -1;
        }
        memcpy(linecopy, buf, blen);

        if (append_row(&items, &n, &cap, linecopy, v) != 0) {
            for (size_t i = 0; i < n; ++i) {
                free(items[i].line);
            }
            free(items);
            free(hdr);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);

    if (n == 0) {
        free(hdr);
        free(items);
        return -2;
    }

    char **rows = (char **)malloc(n * sizeof(char *));
    double *raw = (double *)malloc(n * sizeof(double));
    if (rows == NULL || raw == NULL) {
        free(rows);
        free(raw);
        for (size_t i = 0; i < n; ++i) {
            free(items[i].line);
        }
        free(items);
        free(hdr);
        return -1;
    }
    for (size_t i = 0; i < n; ++i) {
        rows[i] = items[i].line;
        raw[i] = items[i].raw;
    }
    free(items);

    *out_header = hdr;
    *out_body_lines = rows;
    *out_raw = raw;
    *out_n = n;
    return 0;
}
