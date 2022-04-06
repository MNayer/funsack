# FunSack
Tool to acquire subroutine (function) information from an executable
binary via DWARF (v2-v5) debugging information.

## Install
```
python -m pip install .
```

## Build (no install)
```
python setup.py build_ext --inplace
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
