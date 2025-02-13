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
* `echo` - print to the screen<br>
* `getpath` - get enviroment PATH <br>
* `setpath` - set enviroment PATH <br>
* `cd` - change directory <br>
* `history` - print most recent history<br>
* `!!` - invoke most recent command <br>
* `!<n>` - invoke the `n`th newest command <br>
* `!-<n>` - invoke the `n`th oldest command <br>
* `alias` - print all aliases <br>
* `alias <aliasName> <arg1> <arg2> ...` - bind an alias <br>
* `unalias <aliasName>` - unbind an alias <br>
* `exit` - exit the shell <br>

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
