<!-- START OF FILE -->

# ELO321-MASP

## Memory Allocation Simulator Project

This project is a C-based simulator developed for the ELO321 – *Teoría de Sistemas Operativos* course.  
It models and compares memory allocation algorithms — **First-Fit** and **Best-Fit** — under a unified simulation engine.  

---

## Overview

The simulator runs in two selectable modes:  

- 'first' -> applies the **First-Fit** allocation algorithm;  
- 'best' -> applies the **Best-Fit** allocation algorithm.  

Each model executes over the same event trace, allowing direct comparison of:  

- External fragmentation;  
- Allocation efficiency;  
- Execution time.  

---

## Repository Structure

``` text
ELO321-MASP/

    src/
        first_fit.c  
        best_fit.c 
        main.c 

    include/
        first_fit.h
        best_fit.h
        memory.h

    traces/ 
        sample_trace.txt

    docs/
        report.tex
        report.pdf

    Makefile
    .gitignore
    README.md
```

---

## Input Trace File

The simulator reads a **trace file** containing a chronological list of allocation (`A`) and release (`F`) events.  
Each event specifies:

- **Process ID**;
- **Memory Block Size (in KB or blocks)**.

### Example: `traces/sample_trace.txt`

``` text
A 1 64
A 2 128
F 1
A 3 32
F 3
```

- Process 1 requests 64 blocks;
- Process 2 requests 128 blocks;
- Process 1 frees its memory;
- Process 3 requests 32 blocks;
- Process 3 frees its memory.

---

## Branch Organization


---

## Branch Organization

| Branch | Purpose | Responsible |
|--------|----------|--------------|
| `feature/first-fit` | Implementation of **First-Fit** algorithm | Romina Cornejo |
| `feature/best-fit`  | Implementation of **Best-Fit** algorithm | Josué Leiva |
| `main-engine`       | Main simulator engine and documentation | Joaquín Jara |
| `main`              | Stable integrated version | All |

---

## Build and Run

### Requirments

- GCC or Clang compiler;
- Make utility.

### Build
<!--Have to create Makefile first, so probably this section will be on stand-by up to the end of the project-->
### Run
<!--To do-->

---

## Outputs
<!--To do-->
The simulator provides metrics such as:

- Total allocated memory;
- External fragmentation percentage;
- Number of allocation operations;
- Execution time per algorithm.
<!--We still have to decide how those metrics will be returned. Maybe as txt or csv, must check-->

---

## Documentation

---

## Contributors

- **Romina Cornejo** (rol 202230522-3); 
- **Josué Leiva** (rol 202430501-8);
- **Joaquín Jara** (rol 202221028-1) `alexjokyn`.

---

## License

This project is for academic use under UTFSM ELO321 (2025) guidelines.

<!-- END OF FILE -->
