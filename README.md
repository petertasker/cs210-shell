# cs210-shell

A simplified shell for Unix type systems written in C.  

This project is for CS210: Computer Systems and Architecture.  

Written By:  

Kyle Ashley <br>
Brian McKenzie <br>
Callum McManus <br>
Peter Tasker <br>
Jay Thompson <br>

## Overview

This simplified shell implements the following commands:
* echo <br>
* pwd <br>
* cd <br>
* exit <br>

Other features:
* External command piping
* A history of recently used commands, and the ability to recall previously used commands
* An ability to bind and unbind aliases.

## Requirements

A Unix type system (e.g. Linux or macOS) is required to run as we use Unix system calls.

## Installation

Clone the repository and compile the project:
```bash
git clone https://github.com/petertasker/cs210-shell.git
cd cs210-shell
https://github.com/petertasker/cs210-shell.git
```
We recommend you use the gcc compiler
```bash
gcc main.c internalCommands.c shfunc.c -o shell
```

Once compiled, run using
```bash
./shell
```



