# Compare Packages Utility

## Overview

Implement a client-server application in C with the following functionality:
- The client sends a 6x6 matrix with random double values to the server by timer every second, in the range from -10 to 10.
- Server - when receiving the matrix from the client, calculates and outputs the following values to the console:
 - The determinant of the accepted matrix
 - The arithmetic mean of the determinant among the last 5 matrices received (if less than 5 matrices are accepted, output "N/A")
- The determinant of the matrix received 5 matrices ago (if less than 5 matrices are accepted, output "N/A")


## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/orange3107/robopro_test.git

2. **Build and Install:**

   ```bash
   cd robopro_test
   make


## Usage

**Run the server**

   ```bash
    make run_server

**Run the client**

   ```bash
    make run_server
    
