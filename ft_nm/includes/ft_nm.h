/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:32 by acottier          #+#    #+#             */
/*   Updated: 2018/11/29 16:00:23 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include "../../libft/libft.h"
# include "/usr/include/mach-o/loader.h"
# include "/usr/include/mach-o/nlist.h"
# include "/usr/include/mach-o/fat.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <errno.h>

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
	uint64_t				n_value;
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
int							magic_reader(char *ptr, char *file, int nb_args,
								char fat);
void						endian_swap(char *trp, size_t size);

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

/*
** ERRORS.C
*/

int							error(int errcode, char *file);

#endif
