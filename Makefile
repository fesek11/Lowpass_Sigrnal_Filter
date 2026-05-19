CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2
LDFLAGS =
FILTER ?= ma

ifeq ($(FILTER),ma)
FILTER_SRC = filter_ma.c
else ifeq ($(FILTER),ema)
FILTER_SRC = filter_ema.c
else ifeq ($(FILTER),median)
FILTER_SRC = filter_median.c
else ifeq ($(FILTER),sg)
FILTER_SRC = filter_sg.c
else
$(error Unknown FILTER: $(FILTER). Use ma, ema, median, or sg)
endif

FILTER_OBJ = $(FILTER_SRC:.c=.o)
OBJS = main.o csv_reader.o csv_writer.o $(FILTER_OBJ)

OUT ?= pipeline.exe
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LDFLAGS)

main.o: main.c csv_reader.h csv_writer.h filter.h Makefile
csv_reader.o: csv_reader.c csv_reader.h
csv_writer.o: csv_writer.c csv_writer.h
filter_ma.o: filter_ma.c filter.h
filter_ema.o: filter_ema.c filter.h
filter_median.o: filter_median.c filter.h
filter_sg.o: filter_sg.c filter.h

clean:
	cmd /C del /Q *.o pipeline.exe 2>NUL || exit 0

.PHONY: clean
