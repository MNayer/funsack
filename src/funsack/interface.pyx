from elftools.elf.elffile import ELFFile

cdef struct Cu_Info:
    const char *name
    const char *compdir
    const char *lang

cdef struct Die_Info:
    unsigned long long high_pc
    unsigned long long low_pc
    unsigned long long linenum
    const char *name
    const char *decl_file
    Cu_Info cu

cdef extern from "dwarf.c":
    void iterate_subprograms(const char*, void (*)(Die_Info*))

dies = []

cdef print_die_info(Die_Info *info):
    die_info = {
        "low_pc": info.low_pc,
        "high_pc": info.high_pc,
        "name": info.name.decode("ascii"),
        "linenum": info.linenum,
        "decl_file": info.decl_file.decode("ascii"),
        "cu": {
            "name": info.cu.name.decode("ascii"),
            "compdir": info.cu.compdir.decode("ascii"),
            "lang": info.cu.lang.decode("ascii"),
        },
    }
    dies.append(die_info)

cpdef collect_subprogram_dies(bytes fpath):
    global dies
    dies.clear()
    iterate_subprograms(fpath, <void (*)(Die_Info*)>&print_die_info)

def iterate_functions(fpath):
    # Initializes global >>dies<<
    collect_subprogram_dies(fpath.encode("ascii"))

    with open(fpath, "rb") as f:
        elffile = ELFFile(f)

        code = elffile.get_section_by_name(".text")
        sh_addr = code["sh_addr"]
        operations = code.data()

        for die in dies:
            low_pc = die["low_pc"]
            high_pc = die["high_pc"]
            ops = operations[low_pc - sh_addr: high_pc - sh_addr]
            die["ops"] = ops.hex()
            yield die
