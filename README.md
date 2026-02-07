# Traffic Flow Analyser (C++)

## Overview
This project is a C++ console application that analyses traffic flow data collected from a
single-lane road over a period of multiple days. The program processes structured traffic
records to identify peak traffic periods, congestion levels, and vehicle-type-specific trends
to assist with traffic planning and analysis.

The project was originally developed as part of a university practical assessment and has
been adapted for portfolio purposes.

## Project Context
During the original assessment, input data was provided by the lecturer during an online
proctored test. For this repository, a representative sample input file is included to
demonstrate the expected data format and the program’s functionality.

## Design and Reasoning
- Traffic records are read from an external text file and stored in a suitable container for
  efficient processing.
- A `deque` was chosen as the primary data structure to allow fast random access while also
  supporting efficient insertion and removal operations when analysing time-based data.
- The solution was designed to generate multiple independent reports from the same dataset,
  keeping the logic modular and readable.
- Error handling is included to manage invalid input, file access issues, and user selection
  errors.
- Extensive in-code comments were used to document both implementation steps and design
  decisions, as required by the assessment.

## Reports Generated
The program generates the following reports:
- **Peak Traffic Times Report**  
  Identifies overall and daily peak traffic periods, including hourly traffic volumes.
- **Congestion Level Report**  
  Analyses congestion trends, categorises congestion levels, and identifies the most
  congested periods.
- **Vehicle Type-Specific Report**  
  Provides traffic trends, peak times, and average speeds for each vehicle type
  (CAR, BUS, MOTORCYCLE, TRUCK).

User interaction allows reports to be generated in full or selectively.

## Input and Output
- Input data is read from `trafficdata.txt`
- Output is written to a separate file, `trafficReport.txt`, to preserve the original data
  and reflect real-world best practices

## Technologies
- C++
- Standard Library (fstream, string, iostream, containers)

## How to Run
1. Ensure `trafficdata.txt` is located in the same directory as the executable.
2. Compile the program using a C++11-compatible compiler.
3. Run the program.
4. The generated reports will be written to `trafficReport.txt`.

