/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:32 by acottier          #+#    #+#             */
/*   Updated: 2019/02/08 12:08:19 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include "../../libft/libft.h"
# include "/usr/include/mach-o/loader.h"
# include "/usr/include/mach-o/nlist.h"
# include "/usr/include/mach-o/fat.h"
# include "/usr/include/mach/machine.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <errno.h>
# include <stdint.h>

# define MH_STATIC_LIB 0x72613c21
# define HEADER_SIZE 60

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
	_NONE,
	_32_ONLY,
	_64_ONLY,
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

typedef struct				s_archive
{
	int						obj_off;
	int						str_off;
	struct s_archive		*prev;
	struct s_archive		*next;
}							t_archive;

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
int							magic_reader(char *ptr, char *file, int nb_args,
								char fat);

/*
** 64BIT.C
*/

int							bin64(char *ptr, char *file, int nb_args,
								int swap);

/*
** 32BIT.C
*/

int							bin32(char *ptr, char *file, int nb_args,
								int swap);

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
int							free_all(t_symbol *list, t_data *data,
								int errcode, char *str);

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
int							check_duplicate_nodes(t_archive *list, int offset);

/*
** ARCHIVE_LIST.C
*/

t_archive					*mk_archive_list(struct ranlib *symtab, int symtab_size);

/*
** UTILITIES.C
*/

int							determine_priority(int *prio,
								unsigned int magicnb, int *bin32, int **tab);
void						show_arch(int archnb, cpu_type_t cpu, char *file);

/*
** ERRORS.C
*/

int							error(int errcode, char *file);

#endif
