# cs210-shell

A simplified shell for Unix type systems written in C.  

This project is for CS210: Computer Systems and Architecture.  

Written By:  

[Kyle Ashley](https://github.com/Ziggyzag07)<br>
[Brian McKenzie](https://github.com/BrianSMckenzie)<br>
[Callum McManus](https://github.com/Callumxm)<br>
[Peter Tasker](https://github.com/petertasker)<br>
[Jay Thompson](https://github.com/nosferatuus)<br>

## Overview

This simplified shell implements the following commands:
| Command        | Description |
|---------------|-------------|
| `echo`        | Print to the screen |
| `getpath`     | Get environment `PATH` |
| `setpath`     | Set environment `PATH` |
| `cd`          | Change directory |
| `history`     | Print most recent history |
| `!!`          | Invoke the most recent command |
| `!<n>`        | Invoke the `n`th newest command |
| `!-<n>`       | Invoke the `n`th oldest command |
| `alias`       | Print all aliases |
| `alias <aliasName> <arg1> <arg2> ...` | Bind an alias |
| `unalias <aliasName>` | Unbind an alias |
| `exit`        | Exit the shell |

Other features:
* External command piping
* Saving / loading history to disk
* Restoration of PATH and directory after session
  
## Requirements

A Unix type system (e.g. Linux or macOS) is required to run as we use Unix system calls.

## Installation

Clone the repository or download the ZIP. To compile the shell, use:
```bash
make
```

to run the shell, use
```bash
shell
```
