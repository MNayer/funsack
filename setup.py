from setuptools import setup, find_packages, Extension

extensions = [
    Extension("funsack.interface", ["src/funsack/interface.pyx"], libraries=["dwarf"]),
]

setup(
    ext_modules=extensions,
    install_requires=[
        "setuptools>=18.0",
        "Cython",
        "pyelftools",
        "click",
    ],
    entry_points={
        "console_scripts": [
            "funs = funsack.tools:funs",
        ],
    },
)
