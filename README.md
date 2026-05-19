# Lowpass_Sigrnal_Filter
Here is the test task for the Airlogix company, including the raw input data in a .csv file and the expected output. The task was to implement it in the simplest way possible, while keeping it cross-platform so it could be used with STM32 or TI microcontrollers without changing anything in the filter logic.

## Compiler and Build Tool

The project is written in C and is built with `gcc` through the root `Makefile`.
The current compiler flags are:

```cmd
gcc -std=c11 -Wall -Wextra -O2
```

Run all build commands from the project root folder.

## Project Structure

```text
src/     C source and header files
data/    input or reference CSV files
output/  generated CSV files
```

The generated output file is ignored by Git, so it can be recreated locally without polluting the repository.

## Filter Interface

To keep a module-oriented approach, the project uses `src/filter.h` as a small interface layer for the filter API.
The main pipeline only calls:

```c
filter_init();
filter_push(sample);
```

Each filter implementation provides the same API, so the selected filter can be changed at build time without changing the pipeline logic.

## Available Filters

The `Makefile` currently supports these filter options:

```text
ma      Moving average filter
ema     Exponential moving average filter
median  Median filter
sg      Savitzky-Golay filter
```

## How to Build

Use the `FILTER` option to choose the filter implementation and `OUT` to choose the executable name.

Example for the moving average filter:

```cmd
make FILTER=ma OUT=pipeline_ma.exe
```

Other examples:

```cmd
make FILTER=ema OUT=pipeline_ema.exe
make FILTER=median OUT=pipeline_median.exe
make FILTER=sg OUT=pipeline_sg.exe
```

To remove generated object files and executables:

```cmd
make clean
```

## How to Run

After building an executable, run it and pass the CSV file that contains the data.
The program reads the input CSV and adds one new filtered column on the right side of the output file.

Example:

```cmd
.\pipeline_ma.exe .\data\signal.csv
```

By default, the result is written to:

```text
output/signal_filtered.csv
```

To compare filters in one file, build and run each filter one by one. After the first run, use the generated output file as the input for the next filter:

```cmd
make FILTER=ma OUT=pipeline_ma.exe
.\pipeline_ma.exe .\data\signal.csv

make FILTER=ema OUT=pipeline_ema.exe
.\pipeline_ema.exe .\output\signal_filtered.csv

make FILTER=median OUT=pipeline_median.exe
.\pipeline_median.exe .\output\signal_filtered.csv

make FILTER=sg OUT=pipeline_sg.exe
.\pipeline_sg.exe .\output\signal_filtered.csv
```

After these steps, `output/signal_filtered.csv` contains the original data plus filtered columns. This file can then be used to build graphs or analyze the data to understand which filter is better for the current case.
