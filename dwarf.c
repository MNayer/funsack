#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#ifdef __libdwarf_libdwarf_0
#include <libdwarf/libdwarf-0/dwarf.h>
#include <libdwarf/libdwarf-0/libdwarf.h>
#endif /* __libdwarf_libdwarf_0 */
#ifdef __libdwarf_0
#include <libdwarf-0/dwarf.h>
#include <libdwarf-0/libdwarf.h>
#endif /* __libdwarf_0 */
#ifdef __libdwarf
#include <libdwarf/dwarf.h>
#include <libdwarf/libdwarf.h>
#endif /* __libdwarf */

#define TRUE  1
#define FALSE 0

#define SIZEOF(ptr) (sizeof(*ptr))

int cu_version_stamp = 0;
int cu_offset_size = 0;

typedef struct die_info_s {
	unsigned long long high_pc;
	unsigned long long low_pc;
	const char *name;
	struct {
		const char *name;
		const char *lang;
	} cu;
} *Die_Info;

typedef void (*subprogram_cb_t)(Die_Info info);

int print_error(const char *format, ...)
{
	int res;
    va_list args;
    va_start(args, format);

	res = vfprintf(stderr, format, args);

    va_end(args);
	return res;
}

void create_die_info(Dwarf_Die cu_die, Dwarf_Die die, char **srcfiles, Dwarf_Signed filecount, subprogram_cb_t subprogram_cb)
{
	Dwarf_Addr low_pc = 0;
	Dwarf_Addr high_pc = 0;
	Dwarf_Half form = 0;
	//Dwarf_Unsigned filenum = 0;
	//Dwarf_Unsigned fileidx = 0;
	enum Dwarf_Form_Class form_class = 0;
	char *die_name = NULL;
	char *cu_die_name = NULL;
	//Dwarf_Attribute decl_file_attr = 0;
	int res = DW_DLV_ERROR;
	Dwarf_Error error = 0;
	Die_Info info = NULL;

	/* Low PC */
	res = dwarf_lowpc(die, &low_pc, &error);
	if (res == DW_DLV_ERROR) {
		print_error("Error dwarf_lowpc returned DW_DLV_ERROR.\n");
		exit(1);
	} else if (res == DW_DLV_NO_ENTRY) {
		print_error("Error dwarf_lowpc returned DW_DLV_NO_ENTRY.\n");
		return;
	}

	/* High PC */
	res = dwarf_highpc_b(die, &high_pc, &form, &form_class, &error);
	if (res == DW_DLV_ERROR) {
		print_error("Error dwarf_lowpc returned DW_DLV_ERROR.\n");
		exit(1);
	} else if (res == DW_DLV_NO_ENTRY) {
		print_error("Error dwarf_lowpc returned DW_DLV_NO_ENTRY.\n");
		exit(1);
	}
	if (form_class == DW_FORM_CLASS_CONSTANT) {
		high_pc = low_pc + high_pc;
	}

	/* DIE name */
	res = dwarf_diename(die, &die_name, &error);
	if (res == DW_DLV_ERROR) {
		print_error("Error dwarf_diename returned DW_DLV_ERROR.\n");
		exit(1);
	} else if (res == DW_DLV_NO_ENTRY) {
		print_error("Error dwarf_diename returned DW_DLV_NO_ENTRY.\n");
		die_name = NULL;
	}

	/* CU DIE name */
	res = dwarf_diename(cu_die, &cu_die_name, &error);
	if (res == DW_DLV_ERROR) {
		print_error("Error dwarf_diename returned DW_DLV_ERROR.\n");
		exit(1);
	} else if (res == DW_DLV_NO_ENTRY) {
		print_error("Error dwarf_diename returned DW_DLV_NO_ENTRY.\n");
		cu_die_name = NULL;
	}

	/* CU srclang */
	Dwarf_Unsigned lang;
	const char *lang_name = NULL;
	res = dwarf_srclang(cu_die, &lang, &error);
	if (res == DW_DLV_ERROR) {
		print_error("Error dwarf_srclang returned DW_DLV_ERROR.\n");
		exit(1);
	} else if (res == DW_DLV_NO_ENTRY) {
		print_error("Error dwarf_diename returned DW_DLV_NO_ENTRY.\n");
	} else if (res == DW_DLV_OK) {
		res = dwarf_get_LANG_name(lang, &lang_name);
		if (res != DW_DLV_OK) {
			print_error("Error dwarf_get_LANG_name did not return DW_DLV_OK.\n");
			exit(1);
		}
	}

	/* Declaration file */
	//res = dwarf_attr(die, DW_AT_decl_file, &decl_file_attr, &error);
	//if (res == DW_DLV_ERROR) {
	//	print_error("Error dwarf_attr returned DW_DLV_ERROR.\n");
	//	exit(1);
	//} else if (res == DW_DLV_NO_ENTRY) {
	//	print_error("Error dwarf_attr returned DW_DLV_NO_ENTRY.\n");
	//	return;
	//}
	//res = dwarf_formudata(decl_file_attr, &filenum, &error);
	//if (res == DW_DLV_ERROR) {
	//	print_error("Error dwarf_formudata returned DW_DLV_ERROR.\n");
	//	exit(1);
	//} else if (res == DW_DLV_NO_ENTRY) {
	//	print_error("Error dwarf_formudata returned DW_DLV_NO_ENTRY.\n");
	//	exit(1);
	//}
	//if (filenum == 0) {
	//	print_error("Error DW_AT_decl_file is 0 for subprogram %s.\n", die_name);
	//	exit(1);
	//}
	//fileidx = filenum - 1;
	//if (fileidx >= filecount) {
	//	print_error("Error DW_AT_decl_file is greater than filecount for subprogram %s.\n", die_name);
	//	exit(1);
	//}

	info = (Die_Info) calloc(1, SIZEOF(info));
	info->cu.name = cu_die_name == NULL ? "" : cu_die_name;
	info->cu.lang = lang_name == NULL ? "" : lang_name;
	info->name = die_name == NULL ? "" : die_name;
	info->low_pc = low_pc;
	info->high_pc = high_pc;
	subprogram_cb(info);
	
	/* Deallocation */
	free(info);
	//dwarf_dealloc_attribute(decl_file_attr);
}

void
examine_die_data(Dwarf_Debug dbg,
    int is_info,
	Dwarf_Die cu_die,
    Dwarf_Die die,
    int level,
	subprogram_cb_t subprogram_cb,
	char **srcfiles,
	Dwarf_Signed filecount,
	Dwarf_Line_Context linectx,
    Dwarf_Error *errp)
{
    Dwarf_Half tag = 0;
    int res = 0;

    res = dwarf_tag(die,&tag,errp);
    if (res != DW_DLV_OK) {
        print_error("Error in dwarf_tag , level %d \n",level);
        exit(1);
    }

    if (tag == DW_TAG_subprogram) {
		create_die_info(cu_die,die,srcfiles,filecount,subprogram_cb);
    }
}

/*  Recursion, following DIE tree.
    On initial call in_die is a cu_die (in_level 0 )
*/
void
get_die_and_siblings(Dwarf_Debug dbg, Dwarf_Die cu_die, Dwarf_Die in_die,
    int is_info,int in_level,int cu_number,
	subprogram_cb_t subprogram_cb,
	char **srcfiles, Dwarf_Signed filecount,
	Dwarf_Line_Context linectx,
    Dwarf_Error *errp)
{
    int res = DW_DLV_ERROR;
    Dwarf_Die cur_die=in_die;
    Dwarf_Die child = 0;

    examine_die_data(dbg,is_info,cu_die,in_die,in_level,
			subprogram_cb,srcfiles,filecount,linectx,errp);

    /*  Now look at the children of the incoming DIE */
    for (;;) {
        Dwarf_Die sib_die = 0;
        res = dwarf_child(cur_die,&child,errp);
        if (res == DW_DLV_ERROR) {
            print_error("Error in dwarf_child, level %d \n", in_level);
            exit(1);
        }
        if (res == DW_DLV_OK) {
            get_die_and_siblings(dbg,cu_die,child,is_info,
                in_level+1,cu_number,subprogram_cb,
				srcfiles,filecount,linectx,errp);
			dwarf_dealloc(dbg, child, DW_DLA_DIE);
            child = 0;
        }
        res = dwarf_siblingof_b(dbg,cur_die,is_info,
            &sib_die,errp);
        if (res == DW_DLV_ERROR) {
            char *em = dwarf_errmsg(*errp);
            print_error("Error in dwarf_siblingof_b, level %d :%s \n",
                in_level,em);
            exit(1);
        }
        if (res == DW_DLV_NO_ENTRY) {
            /* Done at this level. */
            break;
        }
        /* res == DW_DLV_OK */
        if (cur_die != in_die) {
            dwarf_dealloc(dbg,cur_die,DW_DLA_DIE);
            cur_die = 0;
        }
        cur_die = sib_die;
        examine_die_data(dbg,is_info,cu_die,cur_die,in_level,
				subprogram_cb,srcfiles,filecount,linectx,errp);
    }
}

void
find_subprograms(Dwarf_Debug dbg,
	subprogram_cb_t subprogram_cb,
    Dwarf_Error *errp)
{
    Dwarf_Unsigned cu_header_length = 0;
    Dwarf_Unsigned abbrev_offset = 0;
    Dwarf_Half     address_size = 0;
    Dwarf_Half     version_stamp = 0;
    Dwarf_Half     offset_size = 0;
    Dwarf_Half     extension_size = 0;
    Dwarf_Unsigned typeoffset = 0;
    Dwarf_Half     header_cu_type = DW_UT_compile;
    Dwarf_Bool     is_info = TRUE;
    int cu_number = 0;

    for (;;++cu_number) {
        Dwarf_Die no_die = 0;
        Dwarf_Die cu_die = 0;
        int res = DW_DLV_ERROR;
        Dwarf_Sig8     signature;
		char **srcfiles = NULL;
		Dwarf_Signed filecount = 0;
		int i = 0;

        memset(&signature,0, sizeof(signature));
        res = dwarf_next_cu_header_d(dbg,is_info,&cu_header_length,
            &version_stamp, &abbrev_offset,
            &address_size, &offset_size,
            &extension_size,&signature,
            &typeoffset, 0,
            &header_cu_type,errp);
        if (res == DW_DLV_ERROR) {
            char *em = dwarf_errmsg(*errp);
            print_error("Error in dwarf_next_cu_header: %s\n",em);
            exit(1);
        }
        if (res == DW_DLV_NO_ENTRY) {
            /* Done. */
			break;
        }
        cu_version_stamp = version_stamp;
        cu_offset_size   = offset_size;
        /* The CU will have a single sibling, a cu_die. */
        res = dwarf_siblingof_b(dbg,no_die,is_info,
            &cu_die,errp);
        if (res == DW_DLV_ERROR) {
            char *em = dwarf_errmsg(*errp);
            print_error("Error in dwarf_siblingof_b on CU die: %s\n",em);
            exit(1);
        }
        if (res == DW_DLV_NO_ENTRY) {
            /* Impossible case. */
            printf("no entry! in dwarf_siblingof on CU die \n");
            exit(1);
        }


		/* Initialize dwarf line context */
		Dwarf_Unsigned ltversion = 0;
		Dwarf_Small tablecount = 0;
		Dwarf_Line_Context linectx;
		res = dwarf_srclines_b(cu_die, &ltversion, &tablecount,
				&linectx, errp);
        if (res == DW_DLV_ERROR) {
            print_error("Error dwarf_srclines_b returned DW_DLV_ERROR.\n");
            exit(1);
        }
        if (res == DW_DLV_NO_ENTRY) {
            print_error("Error dwarf_srclines_b returned DW_DLV_NO_ENTRY.\n");
            exit(1);
        }

		/* srcfiles list for CU */
		res = dwarf_srcfiles(cu_die, &srcfiles, &filecount, errp);
        if (res == DW_DLV_ERROR) {
            print_error("Error dwarf_srcfiles returned DW_DLV_ERROR.\n");
            exit(1);
        }
        if (res == DW_DLV_NO_ENTRY) {
            print_error("Error dwarf_srcfiles returned DW_DLV_NO_ENTRY.\n");
            exit(1);
        }

        get_die_and_siblings(dbg,cu_die,cu_die,is_info,0,cu_number,
            subprogram_cb,srcfiles,filecount,linectx,errp);

		/* Deallocation of srcfiles */
		dwarf_srclines_dealloc_b(linectx);
		for (i = 0; i < filecount; i++) {
			dwarf_dealloc(dbg, srcfiles[i], DW_DLA_STRING);
		}
		dwarf_dealloc(dbg, srcfiles, DW_DLA_LIST);
    }
}

void iterate_subprograms(const char *path, subprogram_cb_t subprogram_cb)
{
    static char true_pathbuf[FILENAME_MAX];
    unsigned tpathlen = FILENAME_MAX;
    Dwarf_Handler errhand = 0;
    Dwarf_Ptr errarg = 0;
    Dwarf_Error error = 0;
    Dwarf_Debug dbg = 0;
    int res = 0;

    res = dwarf_init_path(path,true_pathbuf,
        tpathlen,DW_GROUPNUMBER_ANY,errhand,
        errarg,&dbg, &error);
    if (res == DW_DLV_ERROR) {
        /* Valid call even though dbg is null! */
        dwarf_dealloc_error(dbg,error);
        return;
    }
    if (res == DW_DLV_NO_ENTRY) {
        /*  Nothing we can do */
        return;
    }
    //printf("The file we actually opened is %s\n",
    //    true_pathbuf);
	find_subprograms(dbg, subprogram_cb, &error);
    dwarf_finish(dbg);
}
