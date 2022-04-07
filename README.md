# FunSack
Tool to acquire subroutine (function) information from an executable
binary via DWARF (v2-v5) debugging information.

## Requirements
- Requires libdwarf 1:0.3.3-1

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
from funsack import collect_subprogram_dies, dies
collect_subprogram_dies("/tmp/test/test".encode("ascii"))
print(dies)
```
