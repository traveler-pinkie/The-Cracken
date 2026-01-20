1. Project Logic & Architecture

Your program, let's call it CrackerC, will follow a Producer-Consumer model.

    The Host (CPU): Acts as the orchestrator. It reads the wordlist from the disk, applies "rules" (like capitalizing letters), and loads the work into a buffer.

    The Dispatcher: Distributes "work units" to available devices (CPU threads or GPU kernels).

    The Device (GPU/Kernel): Performs the heavy lifting. It executes the hashing algorithm and compares the result to the target.

    The Reporter: If a match is found, it signals the host to stop all threads and print the result.

2. Core Functionality & Components
A. The Input Parser

    Hash Type Detection: Use a switch-case or function pointer table to select the correct hashing function based on user input (e.g., -m 0 for MD5).

    Wordlist Streamer: Do not load a 10GB wordlist into RAM. Use fgets() or mmap() to stream words efficiently.

B. The Hashing Engine (The Kernels)

    For the CPU version: Use OpenSSL (libcrypto) or write your own implementation of MD5/SHA256 in C.

    For the GPU version: You must use OpenCL (since you have AMD GPUs). You will write .cl files which are C-like programs that run on the GPU cores.

C. Multithreading Logic

    POSIX Threads (pthreads): Use these to manage your Ryzen 9 7900X cores.

    Work Stealing: If one thread finishes its batch of words early, it should grab more from the shared buffer.

Building your own hashcat-style program is an ambitious and rewarding project, especially for a Computer Science student focused on Systems Administration and Cyber Security. Since you are proficient in C, have experience with multithreading/mutexes, and own a workstation with dual Radeon 7900 series GPUs, you are in the perfect position to tackle this.

To build a "Mini-Hashcat," you will focus on three domains: File I/O, Thread Management (CPU), and GPGPU Programming (OpenCL).
1. Project Logic & Architecture

Your program, let's call it CrackerC, will follow a Producer-Consumer model.

    The Host (CPU): Acts as the orchestrator. It reads the wordlist from the disk, applies "rules" (like capitalizing letters), and loads the work into a buffer.

    The Dispatcher: Distributes "work units" to available devices (CPU threads or GPU kernels).

    The Device (GPU/Kernel): Performs the heavy lifting. It executes the hashing algorithm and compares the result to the target.

    The Reporter: If a match is found, it signals the host to stop all threads and print the result.

2. Core Functionality & Components
A. The Input Parser

    Hash Type Detection: Use a switch-case or function pointer table to select the correct hashing function based on user input (e.g., -m 0 for MD5).

    Wordlist Streamer: Do not load a 10GB wordlist into RAM. Use fgets() or mmap() to stream words efficiently.

B. The Hashing Engine (The Kernels)

    For the CPU version: Use OpenSSL (libcrypto) or write your own implementation of MD5/SHA256 in C.

    For the GPU version: You must use OpenCL (since you have AMD GPUs). You will write .cl files which are C-like programs that run on the GPU cores.

C. Multithreading Logic

    POSIX Threads (pthreads): Use these to manage your Ryzen 9 7900X cores.

    Work Stealing: If one thread finishes its batch of words early, it should grab more from the shared buffer.

3. The Script (Code Structure)

Your project structure should look like this:
Bash

CrackerC/
├── src/
│   ├── main.c        # CLI Parsing & Orchestration
│   ├── dispatch.c    # Thread/GPU management
│   ├── loader.c      # Wordlist/Hash file I/O
│   └── utils.c       # Common helpers
├── include/
│   ├── crackerc.h    # Struct definitions & Headers
│   └── hashes.h      # Function prototypes for hash logic
├── kernels/
│   └── md5_kernel.cl # OpenCL code for AMD GPUs
└── Makefile

4. Step-by-Step To-Do List
Phase 1: The CPU "Straight" Cracker (Foundation)

    [ ] Task 1: Create a CLI that accepts a target hash and a wordlist path.

    [ ] Task 2: Implement a basic md5_verify(char* candidate, char* target) function using the OpenSSL library.

    [ ] Task 3: Write the loop that reads the wordlist and calls the verify function. Test it on a simple MD5 hash.

Phase 2: High-Performance Multithreading

    [ ] Task 4: Implement pthreads. Divide the wordlist into "chunks" so your 12-core CPU can process different parts of the file simultaneously.

    [ ] Task 5: Implement a Shared Progress Counter using stdatomic.h so you can display "Hashes per second" (H/s) in the terminal.

Phase 3: GPU Acceleration (The "Hashcat" Secret Sauce)

    [ ] Task 6: Set up the OpenCL environment. Detect your 7900 XT and 7900 XTX as OpenCL "devices."

    [ ] Task 7: Write a basic MD5 kernel in OpenCL. This code will run on the GPU's Compute Units.

    [ ] Task 8: Buffer Management. Write the logic to move batches of words from System RAM to VRAM using clEnqueueWriteBuffer.

Phase 4: Rules & Optimizations

    [ ] Task 9: Add a "Rule Engine." Create a function that takes a word like apple and generates Apple, apple123, and @pple.

    [ ] Task 10: Implement Bit-Slicing. This is an advanced C optimization where you process multiple hashes in a single CPU/GPU instruction (SIMD).

5. System-Level Challenges to Expect

Since you are a Systems Admin student, pay close attention to these:

    Memory Bottlenecks: The speed of your DDR5 RAM might actually be the bottleneck when feeding two high-end GPUs. You may need to implement a double-buffering strategy.

    PCIe Bandwidth: Moving data to the 7900 XTX is expensive. You want to send large batches (millions of words) at once to keep the GPU busy.

    Heat & Power: Running your 7900X and dual GPUs at 100% load will draw massive power. Ensure your PSU can handle the sustained "Hard Time" load.