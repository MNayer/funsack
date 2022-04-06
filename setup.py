from setuptools import Extension, setup

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

#from setuptools import Extension, setup
#from Cython.Build import cythonize
#
#ext_modules = [
#    Extension("demo",
#              sources=["demo.pyx"],
#              libraries=["m"]  # Unix-like specific
#              )
#]
#
#setup(name="Demos",
#      ext_modules=cythonize(ext_modules))
