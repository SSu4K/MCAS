# MCAS - Microprogrammed Computer Architecture Simulator

MCAS is a desktop application for simulating a microprogrammed computer architecture at the micro-instruction level. It is designed as an educational tool that exposes the internal workings of a processor while maintaining a modern, intuitive user interface. The exact simulated architecture is DLX - an educational CISC architecture proposed by *Hennesey and Patterson* in *"Computer Architecture: A Quantitative Approach"*.

The project combines low-level architectural transparency with usability, addressing limitations of both legacy academic tools and modern high-level simulators.


## Highlights

- Micro-instruction level simulation with deterministic execution
- Fully configurable instruction set and microcode
- Multiple execution modes (step, micro-step, run, rewind)
- Real-time visualization of processor state
- Modern, cross-platform Qt-based interface
- Designed specifically for teaching and understanding computer architecture

## Screenshots
### Simulation Window
<p align="center">
  <img src="https://github.com/user-attachments/assets/708c475e-8823-49f5-8d42-9468d06bc419" width="600"/>
</p>

### Instruction Editor
<p align="center">
  <img src="https://github.com/user-attachments/assets/6f3944ab-7a65-44b5-b7aa-8720b49c7ccd" width="40%"/>
  <img src="https://github.com/user-attachments/assets/3ab86849-cce2-4fdc-9758-2000d2a29463" width="50%"/>
</p>

### Memory Editor
<p align="center">
  <img src="https://github.com/user-attachments/assets/bed7d75a-be16-4c80-85b6-903b195a5caf" width="600"/>
</p>

### Microcode Editor
<p align="center">
  <img src="https://github.com/user-attachments/assets/1e93dc5d-24dc-46ed-a635-a945a8ccb5d7" width="800"/>
</p>

## Feature Overview

### Simulation Engine
- Single-clock micro-instruction execution
- Accurate control flow via microcode and jump tables
- Full machine state visibility (registers, memory, control state)
- Deterministic assembly and disassembly pipeline supporting informative error messages

### Execution Control
- Micro-instruction stepping
- Instruction-level stepping
- Continuous execution with adjustable frequency
- Execution rewind
- System reset

### Editing Tools
- Instruction editor with live validation and error reporting
- Memory editor with multiple data representations (byte/half/word)
- Microcode and jump table editor with constrained input
- Instruction set configuration editor

### User Interface
- Multi-window desktop application (Qt)
- Synchronized views using signal–slot architecture
- Internationalization (English / Polish)
- Theme support

---
## Technologies

- C++17
- Qt (GUI framework)
- Qt Model–View–Delegate
- Qt Linguist (i18n)
- CMake

---

## Build

### Requirements
- C++17 compiler
- Qt6 (at least 6.8.3)
- CMake
