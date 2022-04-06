cdef struct Cu_Info:
    const char *name
    const char *lang

cdef struct Die_Info:
    unsigned long long high_pc
    unsigned long long low_pc
    const char *name
    Cu_Info cu

cdef extern from "dwarf.c":
    void iterate_subprograms(const char*, void (*)(Die_Info*))

dies = []

cdef print_die_info(Die_Info *info):
    die_info = {
        "low_pc": info.low_pc,
        "high_pc": info.high_pc,
        "name": info.name,
        "cu": {
            "name": info.cu.name,
            "lang": info.cu.lang,
        },
    }
    dies.append(die_info)

cpdef collect_subprogram_dies(bytes fpath):
    global dies
    dies.clear()
    iterate_subprograms(fpath, <void (*)(Die_Info*)>&print_die_info)
