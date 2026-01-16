

## Comprehensive Cryptanalytic Auditing:
A Project Framework for Heterogeneous
AMD RDNA Architectures
- Executive Summary and Strategic Project Scope
The convergence of high-performance computing (HPC) hardware and information security
has necessitated a rigorous re-evaluation of cryptographic resilience. This report delineates a
comprehensive, multi-phased project designed to leverage a specific, high-capability
hardware profile—comprising a Sapphire Pulse Radeon RX 7900 XT, a Sapphire Nitro+ Radeon
RX 9070 XT, an AMD Ryzen 9 9900X, and 128GB of DDR5 RAM—for the purpose of advanced
cryptographic auditing and educational exploration. The project framework is structured to
guide the operator from foundational theoretical understanding to the execution of complex,
automated recovery attacks, thereby demonstrating the practical limits of modern hash
functions when subjected to massive parallel processing.
The hardware configuration in question represents a unique and formidable "crack station,"
characterized by its heterogeneous architecture. The simultaneous utilization of AMD’s
established RDNA 3 architecture (RX 7900 XT) alongside the nascent RDNA 4 architecture (RX
9070 XT) presents both significant computational opportunities and complex integration
challenges. This report provides an exhaustive analysis of these architectures, detailing their
specific interactions with integer-heavy and memory-hard cryptographic primitives.
Furthermore, it outlines the necessary software engineering required to stabilize this
mixed-generation environment within a Linux ecosystem, specifically targeting the ROCm
(Radeon Open Compute) stack.
1.1 Project Objectives and Pedagogical Framework
The primary objective of this research project is to construct a robust laboratory environment
for the study of "one-way" functions. The specific pedagogical and technical goals include:
- Heterogeneous Architectural Analysis: To empirically evaluate and compare the
compute performance (throughput and latency) of RDNA 3 versus RDNA 4 when applied
to diverse hashing algorithms, identifying architectural bottlenecks such as memory bus
width versus clock frequency.
## 1
- Algorithmic Proficiency: To achieve deep technical competency in the mathematical
structures of standard hash functions—specifically MD5, SHA-256, Bcrypt, and
Argon2id—and to understand how these structures dictate hardware selection (CPU vs.
## GPU).
## 4
- Kernel Optimization: To master the tuning of OpenCL and HIP kernels, managing
workload distribution across disparate devices to maximize aggregate system
throughput without inducing thermal throttling or driver instability.
## 6

- Automated Auditing Pipelines: To develop sophisticated Python and Bash scripting
workflows that automate the generation of cryptographic challenges (salted hashes)
and the orchestration of recovery attacks, moving beyond manual execution to scalable
security auditing.
## 8
1.2 Ethical Compliance and Legal Boundaries
It is imperative to establish the ethical and legal framework governing this project. The tools
and techniques described—specifically the use of hashcat and massive parallel processing
for password recovery—are dual-use technologies.
## 10
While they are essential for security
auditing and penetration testing, their unauthorized application constitutes a violation of the
Computer Fraud and Abuse Act (CFAA) in the United States, the Computer Misuse Act in the
UK, and similar statutes globally. This report strictly adheres to an educational mandate; all
methodologies presume the use of self-generated targets or authorized "crackme"
challenges.
## 12
The objective is to understand the defensive posture of cryptographic systems
by analyzing their failure modes under stress.
- Theoretical Foundations of Cryptographic Hashing
To effectively utilize the specified hardware, the operator must first possess a nuanced
understanding of the mathematical constructs being targeted. A cryptographic hash function
is a deterministic mathematical algorithm, denoted as $H(x)$, that maps an input string $x$ of
arbitrary length to a fixed-length string of bytes, known as the digest. The security of these
functions relies on the difficulty of reversing this mapping.
2.1 Core Properties of Secure Hash Functions
For a hash function to be considered cryptographically secure, and thus a valid target for
auditing, it must satisfy several critical properties. The project will involve testing the
hardware's ability to violate these properties through brute force.
● Pre-image Resistance: Given a specific hash $h$, it should be computationally
infeasible to find any message $m$ such that $H(m) = h$. This is the primary property
attacked during password cracking, where the attacker attempts to recover the plaintext
password from the stored hash. The "crack station" essentially tests the limits of
"infeasibility" by applying massive computational power to the problem.
## 5
● Second Pre-image Resistance: Given an input $m_1$, it should be infeasible to find a
distinct input $m_2$ such that $H(m_1) = H(m_2)$.
● Collision Resistance: It should be infeasible to find any two different messages $m_1$
and $m_2$ such that $H(m_1) = H(m_2)$. While less relevant for password cracking,
collision resistance is vital for digital signatures.
● Avalanche Effect: A fundamental property wherein a slight alteration in the input (e.g.,
flipping a single bit) results in a drastic, unpredictable change in the output digest. This
property prevents attackers from inferring the input based on the output's proximity to

a known hash, necessitating a full search of the keyspace.
## 5
2.2 Taxonomy of Hash Algorithms
The project curriculum is divided into two distinct categories of hashes, each interacting with
the AMD hardware in fundamentally different ways.
2.2.1 Fast Hashes (General Purpose)
Algorithms such as MD5, SHA-1, and the SHA-2 family were designed for efficient data
verification and digital signing. They rely on basic bitwise operations—XOR, AND, OR, NOT,
and bit rotation—which are computationally inexpensive.
● MD5 (Message Digest Algorithm 5): Produces a 128-bit digest. Although
cryptographically broken regarding collision resistance, it remains ubiquitous in legacy
systems. Its structure involves four rounds of processing 512-bit blocks. Modern GPUs,
particularly the RDNA architecture, can execute these bitwise operations at rates
exceeding billions of hashes per second.
## 13
● SHA-256: A member of the SHA-2 family, producing a 256-bit digest using a
Merkle-Damgård construction. It operates on 32-bit words and involves 64 rounds of
compression. It is significantly more computationally intensive than MD5 but remains
highly parallelizable, making it an ideal candidate for GPU acceleration.
## 5
Hardware Implication: For fast hashes, the computational bottleneck is frequently the
integer throughput of the Stream Processors or the PCIe bus bandwidth if the attack involves
transferring massive candidate lists. The RX 9070 XT and 7900 XT are expected to excel here
due to their massive core counts and high clock speeds.
## 1
2.2.2 Slow Hashes (Key Derivation Functions)
To counteract the rising power of GPU-based cracking, slow hashing algorithms (Key
Derivation Functions or KDFs) were developed. These algorithms introduce "work factors" that
artificially inflate the computational cost of verification.
● PBKDF2 (Password-Based Key Derivation Function 2): Applies a pseudorandom
function (typically HMAC-SHA256) to the input password and a salt, repeating the
process thousands of times (iterations). This linear increase in CPU time required for
verification translates to a linear increase in cracking time.
## 15
● Bcrypt: Based on the Blowfish cipher, Bcrypt incorporates a configurable work factor
and relies heavily on a 4KB state table that is constantly updated during the hashing
process. This dependency on random, frequency memory lookups disrupts the cache
coherence of GPUs, which prefer predictable memory access patterns. However,
modern GPUs with large L2 caches have largely mitigated this defense.
## 4
● Argon2: The winner of the Password Hashing Competition, Argon2 is a memory-hard
function. It requires a user-specified amount of RAM to compute the hash, effectively
creating a "memory wall." This design resists ASIC and GPU cracking by forcing the
attacker to dedicate expensive memory bandwidth and capacity per thread, rather than
just logic gates.
## 17

Hardware Implication: Argon2 necessitates a strategic shift. While the GPUs possess
significant VRAM (16GB and 20GB), the latency of GDDR6 memory is high compared to CPU
cache. The Ryzen 9 9900X, with its massive 64MB L3 cache and AVX-512 instruction set, is
theoretically superior for auditing high-memory-cost Argon2 hashes, as it can process fewer
threads with much lower memory latency.
## 15
## 3. Hardware Architecture Deep Dive
The specified system utilizes a heterogeneous mix of AMD architectures: RDNA 3, RDNA 4,
and Zen 5. A granular understanding of these micro-architectures is critical for optimizing
hashcat flags and workload distribution.
3.1 GPU 1: Sapphire Pulse Radeon RX 7900 XT (RDNA 3)
● Architecture: RDNA 3 (Navi 31).
● Compute Units (CUs): 84 CUs, translating to 5,376 Stream Processors.
● Memory Configuration: 20GB of GDDR6 memory on a 320-bit bus. This wide bus is a
critical differentiator, providing approximately 800 GB/s of bandwidth, which is
advantageous for memory-intensive algorithms.
## 3
● Instruction Set Architecture (ISA): RDNA 3 introduced dual-issue SIMD units, allowing
the execution of two instructions per clock cycle under specific conditions (wave32).
This architecture is mature, with robust support in the ROCm stack (gfx1100 target).
## 21
● Project Role: This card serves as the established workhorse. Its high memory
bandwidth makes it the primary candidate for algorithms like scrypt or lighter variants
of Argon2 that are VRAM-bound rather than compute-bound.
3.2 GPU 2: Sapphire Nitro+ Radeon RX 9070 XT (RDNA 4)
● Architecture: RDNA 4 (Navi 48).
● Compute Units: 64 CUs, translating to 4,096 Stream Processors.
## 1
● Memory Configuration: 16GB GDDR6 on a narrower 256-bit bus compared to the
7900 XT. However, the memory is clocked at roughly 20 Gbps effective speed.
## 1
● Clock Speeds: The Sapphire Nitro+ implementation features aggressive factory
overclocking, with boost clocks reaching up to 3060 MHz. This is significantly higher
than the typical 2400-2500 MHz of RDNA 3 cards.
## 22
● Transistor Density: Built on the TSMC N4P process (4nm), the Navi 48 die packs 53.9
billion transistors into a monolithic die. This high density allows for extreme power
efficiency and frequency scaling.
## 1
● Project Role: This is the experimental variable. RDNA 4 is newer, and driver support
(gfx1201) is still maturing in the Linux kernel.
## 23
Its higher clock speed suggests it will
outperform the 7900 XT in pure compute scenarios (like raw MD5 or SHA-256 cracking)
where core frequency is king, despite having fewer cores.

3.3 CPU: AMD Ryzen 9 9900X (Zen 5)
● Architecture: Zen 5 (Granite Ridge).
## ● Core Configuration: 12 Cores / 24 Threads.
## 25
● Cache Hierarchy: 64MB of L3 Cache shared across the CCX, plus 1MB L2 per core.
## 25
● Instruction Set: Full support for AVX-512, including SHA-NI (SHA Extensions) and
AES-NI. The Zen 5 architecture features a 512-bit data path, allowing it to process
vector operations with extreme efficiency.
## 27
● Project Role: While GPUs handle parallelizable hashes, the CPU is critical for
algorithms designed to be anti-GPU. The AVX-512 implementation makes the 9900X a
formidable cracker for Argon2id, where it can utilize its large L3 cache to store the
memory grid, avoiding the high latency of round-trips to main system RAM.
## 27
3.4 Memory: 128GB DDR5 RAM
● Capacity: 128GB is substantial, far exceeding the requirements for standard OS
operations.
● Project Role: This capacity facilitates the caching of massive wordlists (e.g., the 132GB
"weakpass" lists) entirely in RAM, reducing I/O latency during dictionary attacks. It also
supports the CPU during high-memory-cost Argon2 benchmarking, where the
algorithm might request gigabytes of RAM per thread.
## 18
- Engineering the Physical and Software Environment
The successful execution of this project requires a rigorously engineered environment. The
combination of RDNA 3 and RDNA 4 GPUs presents unique challenges regarding physical
installation, thermal management, and driver compatibility.
4.1 Physical Assembly and Thermal Thermodynamics
The simultaneous operation of two high-TDP GPUs requires careful thermal and power
planning to prevent throttling, which would invalidate benchmark results.
4.1.1 Power Supply and Transient Load Management
The power consumption of modern GPUs is not static; it involves micro-second transient
spikes that can trip over-current protection (OCP) on inadequate power supplies.
● RX 7900 XT Power Profile: Rated for ~300-315W TBP (Total Board Power). Under
synthetic load (Hashcat -w 4), it can sustain near this limit continuously.
## 20
● RX 9070 XT Power Profile: Rated for ~304-355W. Overclocked models like the
Sapphire Nitro+ often have unlocked power limits, potentially spiking to 400W+ during
complex branching operations in kernels.
## 1
● Ryzen 9 9900X: 120W TDP, with Package Power Tracking (PPT) allowing up to ~162W
under all-core AVX loads.
## 25

● System Total: The aggregate load can approach 850W-950W.
● Project Requirement: A 1000W or 1200W Platinum-rated PSU is mandatory. It is
critical to use independent 8-pin PCIe cables for every connector on the GPUs (two for
the 7900 XT, two for the 9070 XT). Utilizing "pigtail" splitters increases resistance and
heat at the terminal, creating a fire hazard under the sustained load of cryptographic
auditing.
## 30
4.1.2 PCIe Lane Allocation and Physical Interference
● Physical Dimensions: The Sapphire Nitro+ RX 9070 XT is a "Quad Slot" card (3.25 slots
wide), while the Pulse 7900 XT is approx. 2.7 slots wide.
## 31
● Motherboard Constraint: Most standard ATX X670E/X870E motherboards feature PCIe
x16 slots spaced 3 slots apart. Installing the 9070 XT in the primary slot will likely
physically block the second slot, preventing the installation of the 7900 XT.
## 32
## ● Engineering Solution:
○ Option A: Install the narrower card (7900 XT) in the top slot and the massive
9070 XT in the bottom slot, assuming the chassis has sufficient expansion slots
## (7+).
○ Option B (Recommended): Utilize a high-quality PCIe 4.0/5.0 riser cable to
mount one GPU vertically or externally. This "mining-style" configuration
maximizes airflow and separates the thermal envelopes of the two 300W
heaters.
## 34
● Bandwidth Considerations: The CPU provides limited PCIe lanes. In a dual x16
configuration, most consumer motherboards bifurcate the lanes to x8/x8. For "fast"
hashes (MD5), PCIe bandwidth is critical. Running at x8 might introduce a bottleneck if
the attack mode relies on streaming candidates from the host (Attack Mode 0).
However, for Brute Force (Attack Mode 3), candidates are generated on-GPU, making x8
bandwidth negligible.
## 36
## 4.2 The Software Stack: Navigating Heterogeneity
The project requires an Operating System capable of arbitrating between two different GPU
architectures that share a common driver stack but require different kernel optimizations.
## 4.2.1 Operating System Selection: The Linux Mandate
While Windows 11 is supported, the overhead of WDDM (Windows Display Driver Model) often
reduces hashrate by 5-10%. More critically, managing mixed-architecture OpenCL runtimes is
significantly more stable on Linux.
● Recommendation: Ubuntu 24.04 LTS. This release is the target platform for the latest
AMD ROCm drivers (version 6.x and 7.x) which are required for RDNA 4 support.
## 24
4.2.2 Driver Installation: The GFX1100 and GFX1201 Conflict
The core software challenge is that the RX 7900 XT targets the gfx1100 ISA, while the RX
9070 XT targets gfx1201.

● ROCm (Radeon Open Compute): ROCm is AMD's open-source stack for GPU
computing. Support for gfx1201 (RDNA 4) was only introduced in ROCm 7.0/7.1.
Attempting to use older drivers (e.g., ROCm 5.7) will result in the 9070 XT being
unrecognized or causing kernel panics.
## 23
## ● Installation Procedure:
- Purge: Ensure no legacy drivers exist (amdgpu-install --uninstall).
- Kernel Update: Update the Linux kernel to 6.8+ to ensure the amdgpu kernel
module supports the Navi 48 ID.
- ROCm 7.1.1 Installation: Use the package manager to install the specific version
that unifies support.
## Bash
sudo apt update
sudo apt install amdgpu-dkms rocm-hip-sdk rocm-opencl-runtime

- User Groups: Add the user to the render and video groups to allow direct
hardware access without sudo.
## 38
- Project Module 1: Baseline Benchmarking and
## Verification
Phase Objective: To verify hardware stability, confirm correct driver initialization for both
architectures, and establish baseline performance metrics against which optimization efforts
will be measured.
5.1 Verification and Device Enumeration
Before subjecting the hardware to thermal stress, the operator must confirm that the OpenCL
runtime correctly identifies the heterogenous environment.
Action: Execute the following command to query the OpenCL platforms:

## Bash


clinfo | grep -E "Board Name|Device Topology|Compute Unit"

Expected Output: The system should return two distinct GPU devices.
● Device 1: "Navi 31" (7900 XT) with 84 Compute Units.
● Device 2: "Navi 48" (9070 XT) with 64 Compute Units.
● Device 3: "AMD Ryzen 9 9900X" (CPU).
Action: Verify Hashcat detection:

## Bash



hashcat -I

This command lists the backend devices. It is crucial to note the device IDs (e.g., #1, #2) as
they will be used to target specific cards in later modules.
## 39
## 5.2 The Benchmark Suite
The built-in benchmark mode in hashcat serves as an excellent stress test for the power
supply unit and the thermal solution.
Action: Execute a GPU-only benchmark.

## Bash


hashcat -b -D 2

● -b: Benchmark mode.
● -D 2: Filter for OpenCL Device Type 2 (GPU), excluding the CPU to focus on graphics
performance.
5.3 Analysis of Expected Performance Metrics
Based on the architectural specifications and early reporting, the following performance
characteristics are anticipated.
Table 1: Estimated Benchmark Throughput (Hashcat)
Algorithm RX 7900 XT (RDNA 3) RX 9070 XT (RDNA 4) Combined
## Throughput
MD5 ~60,000 MH/s ~52,500 MH/s ~112,500 MH/s
SHA-1 ~24,000 MH/s ~21,700 MH/s ~45,700 MH/s
SHA-256 ~10,000 MH/s ~9,300 MH/s ~19,300 MH/s
SHA-512 ~3,200 MH/s ~2,100 MH/s ~5,300 MH/s
Note: Estimates are derived from comparative architectural analysis and scaled RDNA 3
performance metrics.
## 40
Insight: Despite having fewer Compute Units (64 vs 84), the RX 9070 XT performs
disproportionately well, likely delivering ~85-90% of the 7900 XT's performance in SHA-256.
This is attributed to the higher clock speeds (3.0 GHz vs 2.4 GHz) and the efficiency of the
N4P process. However, in memory-bandwidth-sensitive tests, the 7900 XT's 320-bit bus will
maintain a clearer lead.

5.4 Workload Tuning and Profile Management
Hashcat allows the operator to manually tune the workload profile using the -w flag.
● Experiment: Run the benchmark with -w 3 (High) versus -w 4 (Nightmare).
● Observation: -w 4 forces the GPU to maintain a larger queue of compute kernels,
maximizing utilization. This will render the OS desktop interface unresponsive (laggy)
but is ideal for a dedicated auditing rig. The "Nightmare" profile typically yields a 5-10%
performance increase over -w 3.
## 7
- Project Module 2: The Attack Lifecycle and
## Methodology
Phase Objective: To transition from passive benchmarking to active auditing. This phase
involves creating a controlled testing environment, generating cryptographic challenges, and
executing various recovery methodologies.
## 6.1 Preparation: The Python Challenge Generator
To avoid legal ambiguity, the project relies on self-generated hashes. The operator will write a
Python script to create these targets.
Action: Develop a script generate_challenges.py using the hashlib library.

## Python


import hashlib
import os
import base64

def generate_target(plaintext, salt_hex):
salt = bytes.fromhex(salt_hex)
# Standard SHA-256
hash_obj = hashlib.sha256(plaintext.encode() + salt)
return hash_obj.hexdigest()

# Example: Salted SHA-256
salt = "a1b2c3d4"
target = "ProjectPassword123"
print(f"Hash: {generate_target(target, salt)}:{salt}")

● Insight: Understanding how the salt is appended (or prepended) is critical. If hashcat
expects hash:salt but the script generates salt:hash, the audit will fail. This exercise

reinforces the importance of data formatting standards.
## 8
6.2 Attack Methodology 1: The Dictionary Attack (Straight)
● Concept: This method iterates through a finite list of pre-defined strings (a wordlist).
● Data Source: The project will utilize rockyou.txt, a standard linguistic corpus derived
from a real-world breach, containing roughly 14 million entries.
## ● Execution:
## Bash
hashcat -m 1400 -a 0 targets.txt rockyou.txt -d 1,2

○ -m 1400: SHA-256.
○ -a 0: Attack Mode 0 (Straight).
○ -d 1,2: Utilize both RDNA 3 and RDNA 4 GPUs.
● Bottleneck Analysis: In this mode, the bottleneck is often the PCIe bus. The system
must stream the candidate words from the system RAM to the GPU VRAM. While 128GB
of RAM allows the OS to cache the wordlist completely (eliminating disk I/O), the PCIe
x8 link (due to bifurcation) may limit the maximum throughput for very fast hashes like
## MD5.
## 13
6.3 Attack Methodology 2: Rule-Based Expansion
● Concept: Humans employ predictable patterns when modifying passwords (e.g.,
capitalizing the first letter, appending "123"). Rule-based attacks programmatically
apply these transformations to the dictionary words on the GPU.
## ● Execution:
## Bash
hashcat -m 1400 -a 0 targets.txt rockyou.txt -r rules/best64.rule

● Insight: The best64.rule file contains 64 common transformations. This effectively
multiplies the wordlist size by 64, turning 14 million candidates into roughly 900 million.
Crucially, the transformation happens inside the GPU kernel. This increases the
"computational density" of the workload, reducing the pressure on the PCIe bus and
keeping the Stream Processors saturated.
## 42
6.4 Attack Methodology 3: Brute-Force and Mask Attacks
● Concept: This method attempts every possible combination of characters within a
defined keyspace. This is the scenario where GPU acceleration is most potent.
● Mask Definition: A mask defines the structure of the candidate. ?u?l?l?l?d?d
represents (e.g., "Pass12").
## ● Execution:
## Bash
hashcat -m 1400 -a 3 targets.txt?a?a?a?a?a?a?a


○ -a 3: Brute-Force / Mask Mode.
○ ?a: All printable ASCII characters (95 distinct characters).
## ● Theoretical Capacity:
○ Keyspace: For a 7-character password using all printable characters, the
keyspace is $95^7 \approx 6.98 \times 10^{13}$ combinations.
○ Throughput: With a combined hashrate of ~19,300 MH/s ($1.93 \times 10^{10}$
hashes/second).
○ Time to Exhaustion:

$$\text{Time} = \frac{6.98 \times 10^{13}}{1.93 \times 10^{10}} \approx 3616 \text{
seconds} \approx 1 \text{ hour}.$$
○ Conclusion: The RX 7900 XT and RX 9070 XT combined can exhaust the entire
7-character complex SHA-256 keyspace in approximately one hour. This
demonstrates that 7-character passwords are no longer secure against consumer
hardware.
## 17
## 7. Project Module 3: Advanced Architectures
(Heterogeneous Computing)
Phase Objective: To explore the specific interactions between the different hardware
components when dealing with complex, memory-hard algorithms, and to utilize the Ryzen 9
9900X effectively.
7.1 The CPU Advantage: Argon2 Auditing
Most operators neglect the CPU in hashcat. However, the Argon2 algorithm (-m 21000) was
specifically designed to be resistant to GPU cracking by requiring significant memory per
thread and relying on random access patterns that are cache-unfriendly for GPUs.
Experiment: Benchmark Argon2id on the GPU pair versus the Ryzen 9 9900X.
● Command (GPU): hashcat -b -m 21000 -D 2
● Command (CPU): hashcat -b -m 21000 -D 1
## Analysis:
The Ryzen 9 9900X possesses 64MB of L3 cache. When the Argon2 parameters (memory
cost) are set such that the working set fits within this cache (e.g., 4MB-8MB per thread), the
CPU can execute the hash verification extremely fast. The AVX-512 instruction set on Zen 5
allows it to process multiple data lanes simultaneously. In contrast, while the GPUs have high
bandwidth, the latency of fetching data from GDDR6 memory is significantly higher than CPU
L3 cache access. Furthermore, Argon2 often causes "thread divergence" on GPUs, where the
5000+ Stream Processors spend most of their time waiting for memory.18
● Result: The project will likely demonstrate that for high-memory Argon2 configurations,
the 12-core CPU is competitive with, or even more power-efficient than, the GPUs.

7.2 Managing Multi-GPU Architectures
Running RDNA 3 and RDNA 4 simultaneously can sometimes cause kernel compilation errors if
the hashcat backend tries to compile a single kernel for both devices aggressively.
● Troubleshooting Strategy: If hashcat fails to initialize both cards simultaneously:
- Device Isolation: Use the -d flag to run independent instances.
■ Instance 1: hashcat -d 1... (Targeting 7900 XT)
■ Instance 2: hashcat -d 2... (Targeting 9070 XT)
- Keyspace Partitioning: Manually split the workload.
■ Instance 1 works on the first half of the keyspace (-s 0 -l 5000000...).
■ Instance 2 works on the second half (-s 5000000...).
■ Alternatively, using Python to split the wordlist into wordlist_part1.txt and
wordlist_part2.txt ensures both GPUs are working on unique candidates.
## 6
- Project Module 4: Automation and Python
## Integration
Phase Objective: To move beyond manual command-line execution and develop an
automated auditing pipeline using Python.
8.1 The "Auto-Auditor" Script
The operator will write a Python script to automate the benchmarking and recording process,
creating a persistent log of hardware performance.
## Script Logic:
- Detection: Call hashcat -I to parse available devices and their IDs.
- Iteration: Loop through a list of target hash modes (MD5: 0, SHA-256: 1400, Bcrypt:
## 3200, Argon2: 21000).
- Execution: Run a benchmark for 60 seconds on each mode.
- Parsing: Use Regular Expressions (RegEx) to extract the Speed metric from the stdout.
- Logging: Save the results to a CSV file, including timestamps and thermal data (if
accessible via lm-sensors).
## Code Snippet:

## Python


import subprocess
import re

def benchmark_mode(mode_id):
# -b for benchmark, --machine-readable for easier parsing

cmd = f"hashcat -b -m {mode_id} --machine-readable"
process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE,
stderr=subprocess.PIPE)
out, err = process.communicate()

# Parse the specific output format of hashcat
# Logic to extract H/s goes here
return extracted_speed

modes =
for m in modes:
speed = benchmark_mode(m)
print(f"Mode {m}: {speed} H/s")

## 8.2 Custom Module Development
For advanced learning, the operator can attempt to write a custom kernel or a Python-based
generator for a non-standard hash format.
● Concept: Create a "Salted-XOR" hash in Python (a weak, custom algorithm) and then
use hashcat's generic hash module or write a simple OpenCL kernel to crack it. This
bridges the gap between using the tool and understanding the underlying
cryptographic logic.
## 9
- Hardware Analysis and Findings
This section synthesizes the technical insights derived from the project data.
9.1 RDNA 4 vs. RDNA 3 Efficiency
The RX 9070 XT (Navi 48) utilizes the TSMC N4P process. While the 7900 XT (Navi 31) has a
wider memory bus (320-bit vs 256-bit), the 9070 XT's ability to sustain 3.0 GHz+ core clocks
provides a tangible advantage in compute-heavy algorithms like SHA-256. The architectural
"densification" in RDNA 4 allows it to punch above its weight class (CU count) by executing
more instructions per second per unit of silicon area. However, the 7900 XT remains the
superior choice for memory-hard algorithms due to its sheer memory bandwidth advantage
(~800 GB/s vs ~640 GB/s).
## 1
9.2 The Thermodynamics of Dual-GPU Computing
The project highlights the critical nature of thermal management. The RX 9070 XT, being a
quad-slot card, effectively acts as a thermal barrier if placed incorrectly.
● Thermal Throttling: If the 7900 XT is placed directly above the 9070 XT with minimal
clearance, the upper card will intake the waste heat from the lower card's backplate.
This increases the junction temperature, causing the card to downclock (throttle) to

protect itself, thereby introducing variance in benchmark results.
● Power Efficiency: The RDNA 4 card exhibits sharper transient power spikes.
Undervolting (reducing the voltage by 50-75mV) via the driver settings is a
recommended optimization. It often maintains 98% of the hashrate while reducing
power consumption by 15%, which is vital when stabilizing a dual-GPU system on a
shared 12V rail.
## 29
- Conclusion and Future Outlook
This project report has outlined a rigorous framework for mastering cryptographic auditing
using a heterogeneous mix of cutting-edge AMD hardware. By integrating the raw
computational throughput of the Radeon RX 7900 XT and RX 9070 XT with the architectural
precision of the Ryzen 9 9900X, the operator can simulate a wide spectrum of threat
landscapes—from the brute-forcing of fast hashes to the complex analysis of memory-hard
KDFs.
## Key Technical Takeaways:
- Heterogeneity is a Feature: The combination of RDNA 3 and RDNA 4 provides a
versatile laboratory. RDNA 3 offers stability and memory bandwidth; RDNA 4 offers
high-frequency compute efficiency and a preview of future architectural directions.
- The Algorithm Dictates the Hardware: The project empirically demonstrates the
massive security gap between legacy hashes (MD5) and modern KDFs (Bcrypt/Argon2).
The GPUs can trivialize the former but are effectively neutralized by the latter,
necessitating the use of the CPU (Zen 5 AVX-512).
- Physical Engineering is Critical: The success of such a high-performance project
relies as much on thermal dynamics, power delivery, and PCIe lane management as it
does on the software stack.
This workstation serves not merely as a tool for executing hashcat commands, but as a
research platform for analyzing the efficiency of future cryptographic standards against
next-generation hardware. As RDNA 4 drivers mature within the ROCm ecosystem, further
optimizations are expected to unlock even greater potential from the RX 9070 XT, validating
its role in the future of security auditing.
Works cited
- AMD Radeon RX 9070 XT Specs - GPU Database - TechPowerUp, accessed
## January 13, 2026,
https://www.techpowerup.com/gpu-specs/radeon-rx-9070-xt.c4229
- Radeon™ RX 9070 XT - AMD, accessed January 13, 2026,
https://www.amd.com/en/products/graphics/desktops/radeon/9000-series/amd-r
adeon-rx-9070xt.html
- AMD Radeon RX 7900 XT Specs - GPU Database - TechPowerUp, accessed
## January 13, 2026,
https://www.techpowerup.com/gpu-specs/radeon-rx-7900-xt.c3912

- Adventures in password hashing + migrating to Argon2id - Brandur, accessed
January 13, 2026, https://brandur.org/fragments/password-hashing
- Design of a New Cryptographic Hash Function – Titanium - ResearchGate,
accessed January 13, 2026,
https://www.researchgate.net/publication/323705182_Design_of_a_New_Cryptogr
aphic_Hash_Function_-_Titanium
- Multi GPU clustering - Hashcat, accessed January 13, 2026,
https://hashcat.net/forum/thread-3602.html
- Tune GPU Performance with Hashcat OpenCL Flags - LabEx, accessed January
## 13, 2026,
https://labex.io/tutorials/kali-tune-gpu-performance-with-opencl-flags-594511
- Python hashing and salting - Shittu Olumide Ayodeji - Medium, accessed January
## 13, 2026,
https://iamholumeedey007.medium.com/python-hashing-and-salting-2987ec73f
a7c
- How to Write a Hashcat Module - Adversis, accessed January 13, 2026,
https://www.adversis.io/blogs/how-to-write-a-hashcat-module
- Top 20+ Cyber Security Projects for 2025 [with Source Code] - KnowledgeHut,
accessed January 13, 2026,
https://www.knowledgehut.com/blog/security/top-cyber-security-projects
- 100 Ideas for Research Projects in Encryption and Cybersecurity - Immerse
Education, accessed January 13, 2026,
https://www.immerse.education/beyond-syllabus/encryption-and-cybersecurity/1
## 00-ideas-for-research-projects-in-encryption-and-cybersecurity/
- DEF CON 33 - Making a custom Hashcat module to solve a decade-old puzzle
challenge - Joseph Gabay - YouTube, accessed January 13, 2026,
https://www.youtube.com/watch?v=74_4Q329PH8
- Comparison of the strongest methods of cracking passwords and how to
prevent them - Simple search, accessed January 13, 2026,
https://his.diva-portal.org/smash/get/diva2:1981580/FULLTEXT01.pdf
- SHA-256 vs Argon2 | Compare Top Cryptographic Hashing Algorithms -
MojoAuth, accessed January 13, 2026,
https://mojoauth.com/compare-hashing-algorithms/sha-256-vs-argon2
- GPU speedup for PBKDF2 vs bcrypt vs Argon2 - Information Security Stack
Exchange, accessed January 13, 2026,
https://security.stackexchange.com/questions/285414/gpu-speedup-for-pbkdf2-
vs-bcrypt-vs-argon2
- Password Hashing: Scrypt, Bcrypt and ARGON2 | by Michele Preziuso | Medium,
accessed January 13, 2026,
https://medium.com/@mpreziuso/password-hashing-pbkdf2-scrypt-bcrypt-and-
argon2-e25aaf41598e
- How come hackers with stronger GPU and time goes on, takes longer to crack
the same password length? Shouldn't it be shorter? : r/Bitwarden - Reddit,
accessed January 13, 2026,
https://www.reddit.com/r/Bitwarden/comments/1o337o2/how_come_hackers_wit

h_stronger_gpu_and_time_goes/
- Since GPUs have gigabytes of memory, does Argon2id need to use gigabytes of
memory as well in order to effectively thwart GPU cracking?, accessed January
## 13, 2026,
https://security.stackexchange.com/questions/247936/since-gpus-have-gigabyte
s-of-memory-does-argon2id-need-to-use-gigabytes-of-memo
- "Argon2 is weaker than bcrypt for runtimes < 1000ms" - Why is that? : r/crypto -
Reddit, accessed January 13, 2026,
https://www.reddit.com/r/crypto/comments/l395nj/argon2_is_weaker_than_bcrypt
## _for_runtimes_1000ms/
- accessed January 13, 2026,
https://www.techpowerup.com/gpu-specs/radeon-rx-7900-xt.c3912#:~:text=The
%20GPU%20is%20operating%20at,rated%20at%20300%20W%20maximum.
- SAPPHIRE NITRO+ AMD Radeon™ RX 9070 XT OC GPU, accessed January 13,
## 2026,
https://www.sapphiretech.com/en/consumer/nitro-radeon-rx-9070-xt-16g-gddr6
- Sapphire NITRO+ RX 9070 XT Specs - GPU Database - TechPowerUp, accessed
## January 13, 2026,
https://www.techpowerup.com/gpu-specs/sapphire-nitro-rx-9070-xt.b12249
- There Will Not Be Official ROCm Support For The Radeon RX 9070 Series On
Launch Day, accessed January 13, 2026,
https://www.phoronix.com/news/AMD-ROCm-RX-9070-Launch-Day
- Compatibility matrix - AMD ROCm documentation, accessed January 13, 2026,
https://rocm.docs.amd.com/en/latest/compatibility/compatibility-matrix.html
- AMD Ryzen™ 9 9900X, accessed January 13, 2026,
https://www.amd.com/en/products/processors/desktops/ryzen/9000-series/amd-
ryzen-9-9900x.html
- AMD Ryzen 9 9900X Specs | TechPowerUp CPU Database, accessed January 13,
2026, https://www.techpowerup.com/cpu-specs/ryzen-9-9900x.c3650
- AMD Ryzen 9 9900X Processor - Benchmarks and Specs - NotebookCheck.net
Tech, accessed January 13, 2026,
https://www.notebookcheck.net/AMD-Ryzen-9-9900X-Processor-Benchmarks-a
nd-Specs.862075.0.html
- What is the max power draw for asrock phantom 7900xt? : r/radeon - Reddit,
accessed January 13, 2026,
https://www.reddit.com/r/radeon/comments/1d06klr/what_is_the_max_power_dra
w_for_asrock_phantom/
- AMD RX 9070 and 9070 XT: Everything you need to know - Corsair, accessed
## January 13, 2026,
https://www.corsair.com/us/en/explorer/gamer/gaming-pcs/amd-rx-9070-and-90
## 70-xt-everything-you-need-to-know/
- Using 1 Psu PCIe cable for both GPU and Riser. Is it safe? Image to explain the
situation exactly. Using a 3070 - Reddit, accessed January 13, 2026,
https://www.reddit.com/r/EtherMining/comments/p4rwjk/using_1_psu_pcie_cable
## _for_both_gpu_and_riser_is/

- Sapphire Technology AMD Radeon RX 9070 XT NITRO Triple Fan 16GB GDDR6
PCIe 5.0 Graphics Card - Micro Center, accessed January 13, 2026,
https://www.microcenter.com/product/690170/sapphire-technology-amd-radeon
## -rx-9070-xt-nitro-triple-fan-16gb-gddr6-pcie-50-graphics-card
- Asus Proart X870e - 3 slot GPU is blocking the other PCIE slots? - Level1Techs
Forums, accessed January 13, 2026,
https://forum.level1techs.com/t/asus-proart-x870e-3-slot-gpu-is-blocking-the-ot
her-pcie-slots/224852
- x670e ProArt - the distance between 16x slots : r/ASUS - Reddit, accessed
## January 13, 2026,
https://www.reddit.com/r/ASUS/comments/xxivyp/x670e_proart_the_distance_bet
ween_16x_slots/
- GPU PCI-E Port Multiplier Riser Cables Installation/Testing (Ethereum Mining) -
YouTube, accessed January 13, 2026,
https://www.youtube.com/watch?v=ojFbLtZgddk
- Build Help for multi GPU setup : r/homelab - Reddit, accessed January 13, 2026,
https://www.reddit.com/r/homelab/comments/1d1pb5m/build_help_for_multi_gpu
## _setup/
- MEG X670E ACE - Questions On PCIe Space - Dual GPU - M.2 Heatsinks :
r/MSI_Gaming, accessed January 13, 2026,
https://www.reddit.com/r/MSI_Gaming/comments/15azjoi/meg_x670e_ace_questi
ons_on_pcie_space_dual_gpu_m2/
- Compatibility matrix - AMD ROCm documentation, accessed January 13, 2026,
https://rocm.docs.amd.com/en/docs-7.0.0/compatibility/compatibility-matrix.html
- How to install AMD RX 7900 GPU drivers/software in Windows Server 2019 or
2022, accessed January 13, 2026,
https://www.youtube.com/watch?v=ZL47Ih3UDxQ
- HashCat GPU and CPU : r/HowToHack - Reddit, accessed January 13, 2026,
https://www.reddit.com/r/HowToHack/comments/179kt4j/hashcat_gpu_and_cpu/
- AMD 9070XT - Hashcat, accessed January 13, 2026,
https://hashcat.net/forum/thread-12521.html
- AMD 7900XTX vs NVIDIA RTX 4090 - Large Performance Delta - hashcat Forum,
accessed January 13, 2026,
https://hashcat.net/forum/archive/index.php?thread-12115.html
- Generating statistics for hash results? - hashcat Forum, accessed January 13,
2026, https://hashcat.net/forum/archive/index.php?thread-11006.html
- Multiple GPUs and instances - Hashcat, accessed January 13, 2026,
https://hashcat.net/forum/thread-11291-nextnewest.html
- Power supply recommendations for new 9070 XT card : r/Microcenter - Reddit,
accessed January 13, 2026,
https://www.reddit.com/r/Microcenter/comments/1j5r94e/power_supply_recomm
endations_for_new_9070_xt_card/