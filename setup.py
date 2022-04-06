from setuptools import Extension, setup
import os

# Pre-setup adjustments
# Search for libdwarf header files
include_dirs = ["/usr/include", "/usr/local/include"]
dwarf_paths = {
    "libdwarf/libdwarf-0/libdwarf.h": "__libdwarf_libdwarf_0",
    "libdwarf-0/libdwarf.h": "__libdwarf_0",
    "libdwarf/libdwarf.h": "__libdwarf",
}
paths = (os.path.join(include_dir, dwarf_path) for include_dir in include_dirs for dwarf_path in dwarf_paths.keys())
paths = filter(os.path.exists, paths)
paths = list(paths)
if len(paths) == 0:
    raise Exception("Cannot find libdwarf header files.")
if len(paths) > 1:
    raise Exception("Too many libdwarf header files. Cannot decide which one to take.")

# Find correct preprecessor directive
header_path = paths[0]
paths = map(lambda dwarf_path: (dwarf_path, len(dwarf_path)) if dwarf_path in header_path else (dwarf_path, 0), dwarf_paths)
path,_ = sorted(paths, key=lambda el: el[1])[-1]
directive = dwarf_paths[path]

# Set preprecessor directive
with open("dwarf.c", "r+") as f:
    src = f.read()
    f.seek(0)
    f.write(f"#define {directive}\n")
    f.write(src)

# Begin setup
ext_modules = [
    Extension("funsack",
              sources=["funsack.pyx"],
              libraries=["dwarf"]
              )
]

setup(
    name="FunSack",
    version="0.0.1",
    packages=[
        "tools",
    ],
    install_requires=[
        "setuptools>=18.0",
        "Cython",
        "pyelftools",
        "click",
    ],
    ext_modules=ext_modules,
    zip_safe=False,
    entry_points={
        "console_scripts": [
            "funs = tools.tools:funs",
        ],
    },
    license="GPL v3",
    author="Felix Weissberg",
    author_email="f.weissberg@tu-bs.de",
    description="FunSack: A framework to acquire functions from an executable binary via DWARF (v2-v5) debugging information.",
)
