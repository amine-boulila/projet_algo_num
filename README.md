# Scalar, Vector, and Matrix Clocks

This project demonstrates the implementation of Scalar, Vector, and Matrix clocks for distributed systems. These logical clocks are used to track the causal relationships between events in distributed processes.

## Project Structure

The project contains the following files and directories:

- **scalar_clock/**: Contains the source code for the implementation of Scalar, Vector, and Matrix clocks.
  - `process_scalar.c`: Implementation of Scalar clocks.
  - `process_vector.c`: Implementation of Vector clocks.
  - `process_matrix.c`: Implementation of Matrix clocks.
  - `run.sh`: Script to compile and run the processes.
  - `stop.sh`: Script to stop all running processes.
- **README.md**: This file, providing an overview of the project.

## How to Run

1. **Compile and Run**:
   Use the `run.sh` script to compile and run the processes. The script accepts one argument to specify the type of clock:

   - `1`: Scalar clock
   - `2`: Vector clock
   - `3`: Matrix clock

   Example:

   ```bash
   ./run.sh 1
   ```

   This will compile and run the Scalar clock implementation.

2. **Stop Processes**:
   Use the `stop.sh` script to stop all running processes:
   ```bash
   ./stop.sh
   ```

## Requirements

- **Linux OS**
- **GCC Compiler**
- **xterm**: Ensure `xterm` is installed. You can install it using:
  ```bash
  sudo apt install xterm
  ```

## Implementation Details

### Scalar Clock

- Tracks a single integer value for each process.
- Simple and efficient but does not capture causal relationships fully.

### Vector Clock

- Maintains a vector of integers, one for each process.
- Captures causal relationships between events.

### Matrix Clock

- Maintains a matrix of integers, representing the causal relationships between all pairs of processes.
- Provides the most detailed information but is computationally expensive.

## Authors

- **Amin Boulila**
