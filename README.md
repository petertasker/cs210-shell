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
* `echo` <br>
* `pwd`,  `getpath` <br>
* `cd`, `setpath` <br>
* `exit` <br>
* `history`<br>
* `!!`,<br>
* `!<n>`, where `n` is the nth newest command <br>
* `!-<n>`, where `n` is the nth oldest command<br>
* `alias`<br>
* `alias <aliasName> <arg1> <arg2> ...`<br>
* `unalias <aliasName>`<br>

Other features:
* External command piping
* Saving / loading history to disk

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