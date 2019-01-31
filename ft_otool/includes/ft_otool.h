/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:32 by acottier          #+#    #+#             */
/*   Updated: 2019/01/29 10:09:23 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include "../../libft/libft.h"
# include "/usr/include/mach-o/loader.h"
# include "/usr/include/mach-o/nlist.h"
# include "/usr/include/mach-o/fat.h"
# include "/usr/include/mach/machine.h"
# include "/usr/include/mach-o/ranlib.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <errno.h>
# include <stdint.h>

# define MH_STATIC_LIB 0x72613c21

enum						e_errcodes
{
	_EXIT_FAILURE = -1,
	_EXIT_SUCCESS,
	_OPEN_FAILURE,
	_MMAP_FAILURE,
	_NO_SYMTAB_FAILURE,
	_FILE_NOT_FOUND,
	_BAD_FMT,
	_SCTR_NOT_FOUND,
	_EXIT_NO_FILE,
	_DISPLAY_OK,
	_DATA_OK
};

enum						e_arch_display_prio
{
	_DISABLE_32 = -2,
	_HIDE = 0,
	_SHOW,
	_SHOW_WITH_ARCH
};

enum						e_filetypes
{
	_BIN32,
	_BIN64,
	_FAT,
	_DYLIB,
	_OBJ,
	_SHARED_OBJ,
	_LIB
};

typedef union				u_nlist
{
	struct nlist			*list32;
	struct nlist_64			*list64;
}							t_nlist;

typedef struct				s_archive
{
	int						obj_off;
	int						str_off;
	struct s_archive		*prev;
	struct s_archive		*next;
}							t_archive;

typedef struct				s_info
{
	uint32_t				n_un;
	uint8_t					n_type;
	uint8_t					n_sect;
	uint16_t				n_desc;
	union {
		uint64_t			val_64;
		uint32_t			val_32;
	}						n_value;
}							t_info;

typedef struct				s_symbol
{
	t_info					*s_info;
	char					*name;
	char					type;
	struct s_symbol			*next;
	struct s_symbol			*prev;
}							t_symbol;

typedef struct				s_data
{
	int						filetype;
	char					swap;
	char					*ptr;
	unsigned int			ncmds;
	struct load_command		*lc;
	struct symtab_command	*symtab;
}							t_data;

/*
** MAIN.c
*/
int							magic_reader(char *ptr, char *file, char fat);

/*
** BIN64.C
*/

int							bin64(char *ptr, char *file, int swap, int fat);
void						display_value(char *addr, unsigned int length, int int_size);
void						show_hex(char *cursor);

/*
** BIN32.C
*/

int							bin32(char *ptr, char *file, int swap, int fat);

/*
** SYM_LIST.C
*/

t_symbol					*make_sym_list(char *stringtable,
								t_nlist *el, int nysms, char type);
void						free_sym_list(t_symbol *list);

/*
** DISPLAY.C
*/

int							display(t_symbol *list, t_data *data);
char						get_symbol_type(t_symbol *list);

/*
** DATA.C
*/

int							fill_data(char *ptr, t_data **data);
int							fill_data_32(char *ptr, t_data **data);
int							free_all(t_data *data, int errcode, char *str);

/*
** SECTOR_BROWSING.C
*/

char						browse_sector_bin64(t_data *data,
								uint8_t n_sect, struct load_command *lc);
char						browse_sector_bin32(t_data *data,
								uint8_t n_sect, struct load_command *lc);

/*
** ENDIAN_SWAPS.C
*/

char						*endian_swap(char *ptr, size_t size);
char						*fat_swap(char *ptr);

/*
** STATIC_LIB.C
*/

int							static_lib(char *ptr, char *file);

/*
** ARCHIVE_LIST.C
*/

t_archive					*mk_archive_list(struct ranlib *symtab, int symtab_size);

/*
** UTILITIES.C
*/

void						show_name(cpu_type_t cpu, char *file, int to_show);
char						*show_address(char*address);
int							determine_priority(unsigned int magicnb, int **tab,
								int *bin32, int i);

/*
** ERRORS.C
*/

int							error(int errcode, char *file);

#endif
