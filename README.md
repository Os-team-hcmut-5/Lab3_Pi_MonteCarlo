# Performance Analysis of Multi-Threaded Monte Carlo $\pi$ Estimation

This repository contains the source code, experimental data, and performance analysis for estimating the value of $\pi$ using the Monte Carlo method. The project investigates the impact of thread synchronization overhead and memory contention by comparing three distinct parallel computing architectures using POSIX Threads (`pthreads`) in C.

## Team Members
* **Le Hoang Long** 
* **Nguyen Huu Phuoc**
* **Nguyen Gia An**
* **Nguyen Hoang Minh**
* **Institution:** Ho Chi Minh City University of Technology (HCMUT)

---

## The Approaches

The project evaluates $N = 10^8$ points using a 64-bit Mersenne Twister PRNG, exploring the following architectures:

1. **Approach 1 (Sequential):** A single-threaded baseline execution. Serves as the control for $1.00\times$ speedup calculations.
2. **Approach 2 (Local Accumulation):** Threads compute hits in strictly local variables and merge them only once at the end (`pthread_join`). This eliminates shared-state contention, achieving near-linear speedup up to hardware saturation.
3. **Approach 3 (Global Mutex):** All threads share a single global counter protected by `pthread_mutex_lock`. This demonstrates catastrophic lock contention, serializing the execution and causing performance to fall below the sequential baseline.
4. **Trade-off Analysis (Modified Approach 3):** Tests the impact of varying the synchronization frequency (batching updates every $k$ iterations) to bridge the gap between extreme contention and local accumulation.

---

## Prerequisites

To compile and run this project, ensure your system has the following installed:
* **Linux Environment** (Tested on Ubuntu 22.04 LTS / 24.04 LTS)
* **GCC (GNU Compiler Collection)** supporting C99 or later.
* **POSIX Threads** (`libpthread`)
* **Python 3** & **Matplotlib** (Only required if you want to regenerate the graphs).

---

## Compilation & Execution

### 1. Compiling the Code
We have provided a shell script to automate the compilation of all approaches using the `-O3` optimization flag. Run the following commands in the root directory:

```bash
# Make the build script executable
chmod +x build.sh

# Run the build script
./build.sh
```
*Note: This will generate the compiled executables (`run1`, `run2`, `run3`, `run3_mod`, and `ts`) and place them inside the `bin/` directory.*

### 2. Running the Experiments
You can run the generated executables individually from the `bin/` folder. Each program will output a formatted table showing the Thread Count, $\pi$ Estimate, Execution Time, and Speedup.

```bash
# Run the Sequential Baseline
./bin/run1

# Run Local Accumulation (Approach 2)
./bin/run2

# Run Global Mutex Contention (Approach 3)
./bin/run3

# Run the Synchronization Frequency Trade-off Experiment
./bin/run3_mod

# Run the Extreme Thread Scaling Experiment (up to 1024 threads)
./bin/ts
```

### 3. Generating Graphs
If you wish to visualize the data, you can use the provided Python scripts. Ensure you have `matplotlib` installed (`pip install matplotlib`).

Run the following commands from the root directory:

```bash
# Generate the Synchronization Frequency Trade-off graphs
python3 scripts/approach3Modify.py

# Generate the main Performance Comparison graphs (Approach 2 vs Approach 3)
python3 scripts/resultGraph.py

# Generate the Extreme Thread Scaling graphs
python3 scripts/threadScalling.py