/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:32 by acottier          #+#    #+#             */
/*   Updated: 2018/10/17 17:02:07 by acottier         ###   ########.fr       */
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
	_EXIT_NO_FILE
};

typedef struct s_symbol
{
	char				*name;
	uint64_t			value;
	struct s_symbol		*next;
	struct s_symbol		*prev;
}				t_symbol;

/*
** MAIN.c
*/

/*
** MACH-O.C
*/

int			bin64(char *ptr, char *file);

/*
** SYM_LIST.C
*/

t_symbol	*make_sym_list(char *stringtable, struct nlist_64*el, int nysms);
void		free_sym_list(t_symbol *list);

/*
** DISPLAY.C
*/

void		display(t_symbol *list);

/*
** ERRORS.C
*/

int			error(int errcode, char *file);

#endif