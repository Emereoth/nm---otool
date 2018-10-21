/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:32 by acottier          #+#    #+#             */
/*   Updated: 2018/10/21 15:51:22 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

#include "../../libft/libft.h"
#include "/usr/include/mach-o/loader.h"
#include "/usr/include/mach-o/nlist.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

enum errcodes
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

enum filetypes
{
	_BIN,
	_BIN64,
	_FAT,
	_DYLIB,
	_OBJ,
	_SHARED_OBJ,
	_LIB
};

typedef struct s_symbol
{
	struct nlist_64		*s_info;
	char				*name;
	struct s_symbol		*next;
	struct s_symbol		*prev;
}				t_symbol;

typedef struct	s_data
{
	int							filetype;
	char						*ptr;
	int							ncmds;
	struct load_command			*lc;
	struct symtab_command		*symtab;
}				t_data;

/*
** MAIN.c
*/

/*
** MACH-O.C
*/
int			bin64(char *ptr, char *file, int nb_args);
char		get_symbol_type(uint8_t n_type, uint8_t n_sect);

/*
** SYM_LIST.C
*/
t_symbol	*make_sym_list(char *stringtable, struct nlist_64*el, int nysms);
void		free_sym_list(t_symbol *list);

/*
** DISPLAY.C
*/
int			display(t_symbol *list, t_data *data);

/*
** DATA.C
*/
int			fill_data(char *ptr, t_data **data);
int			free_all(t_symbol *list, t_data *data, int errcode, char *str);

/*
** SECTOR_BROWSING.C
*/
char		browse_sector_bin64(t_data *data, t_symbol *list);

/*
** ERRORS.C
*/
int			error(int errcode, char *file);

#endif