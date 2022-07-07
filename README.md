# FunSack
Tool to acquire subroutine (function) information from an executable
binary via DWARF (v2-v5) debugging information.

## Requirements
- Requires libdwarf 1:0.3.3-1
- libdwarf.h and dwarf.h must be in \<includepath\>/libdwarf/libdwarf.h and
  \<includepath\>/libdwarf/dwarf.h, respectively. If they are placed anywhere else,
  a symbolic link at these locations should be created.

## Install
```
python -m pip install .
```

## Usage
CLI:
```
funs --help
funs -i </path/to/elffile>
```

Within python:
```python
from funsack.interface import iterate_functions

# Path to target ELF file
fpath = "/path/to/elffile"

for function in iterate_functions(fpath):
	print(function)
```

Using docker:
```bash
# Build image
docker build . -t funsack

# Show help
docker run --rm funsack funs --help

# Parse dwarf, print to stdout
docker run --rm -v <intput_dir>:/in funsack funs -i /in/<elf_file>

# Parse dwarf, write output to file
docker run --rm -v <input_dir>:/in -v <output_dir>:/out funs -i /in/<elf_file> -o /out/<jsonl_file>
```
