# ProcessWatcher

## Introduction
ProcessWatcher is a simple process monitoring tool designed for Linux operating systems. It provides real-time insights into various process metrics such as status, memory usage, CPU usage, execution time, priority, process ID (PID), and name. Additionally, it offers an overview of the total CPU load and memory usage on the system.

## Features
- Display of individual process statistics:
  - Status
  - Memory usage (in %)
  - CPU usage (in %)
  - Execution time
  - Priority
  - Process ID (PID)
  - Process name
- Displays total CPU load and memory usage of the system.
- Process list sorting options with both ascending and descending orders:
  - By Process ID
  - By Memory Usage (%)
  - By CPU Usage (%)
  - By Process Status
  - By Process Name
- Default sorting by CPU usage (Descending).
- Help command (`-h`, `--help`) to display usage information.

## Installation
To install ProcessWatcher, clone the repository and compile the application:
```bash
git clone [repository-url]
cd ProcessWatcher
make
```

## Usage
Run `ProcessWatcher` in the terminal to start monitoring processes. The application supports various sorting options to customize the view:

```
Sorting Order Options:
  'p'  Sort by Process ID (Ascending)
  'P'  Sort by Process ID (Descending)
  'm'  Sort by Memory Usage (%) (Ascending)
  'M'  Sort by Memory Usage (%) (Descending)
  'u'  Sort by CPU Usage (%) (Ascending)
  'U'  Sort by CPU Usage (%) (Descending)
  't'  Sort by Process Status (Ascending)
  'T'  Sort by Process Status (Descending)
  'n'  Sort by Process Name (Ascending)
  'N'  Sort by Process Name (Descending)
```

To display help and exit the program:
```bash
./ProcessWatcher -h
```

To exit the program during runtime, press `'q'`.
