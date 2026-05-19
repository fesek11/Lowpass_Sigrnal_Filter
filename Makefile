CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2
LDFLAGS =
FILTER ?= ma
SRC_DIR = src

ifeq ($(FILTER),ma)
FILTER_SRC = $(SRC_DIR)/filter_ma.c
else ifeq ($(FILTER),ema)
FILTER_SRC = $(SRC_DIR)/filter_ema.c
else ifeq ($(FILTER),median)
FILTER_SRC = $(SRC_DIR)/filter_median.c
else ifeq ($(FILTER),sg)
FILTER_SRC = $(SRC_DIR)/filter_sg.c
else
$(error Unknown FILTER: $(FILTER). Use ma, ema, median, or sg)
endif

FILTER_OBJ = $(notdir $(FILTER_SRC:.c=.o))
OBJS = main.o csv_reader.o csv_writer.o $(FILTER_OBJ)

OUT ?= pipeline.exe
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LDFLAGS)

main.o: $(SRC_DIR)/main.c $(SRC_DIR)/csv_reader.h $(SRC_DIR)/csv_writer.h $(SRC_DIR)/filter.h Makefile
csv_reader.o: $(SRC_DIR)/csv_reader.c $(SRC_DIR)/csv_reader.h
csv_writer.o: $(SRC_DIR)/csv_writer.c $(SRC_DIR)/csv_writer.h
filter_ma.o: $(SRC_DIR)/filter_ma.c $(SRC_DIR)/filter.h
filter_ema.o: $(SRC_DIR)/filter_ema.c $(SRC_DIR)/filter.h
filter_median.o: $(SRC_DIR)/filter_median.c $(SRC_DIR)/filter.h
filter_sg.o: $(SRC_DIR)/filter_sg.c $(SRC_DIR)/filter.h

$(OBJS): %.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	cmd /C del /Q *.o pipeline*.exe 2>NUL || exit 0

.PHONY: clean
