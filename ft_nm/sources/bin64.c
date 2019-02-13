/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/02/13 16:19:13 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include <stdio.h>

/*
** Create symbol list, display it and free
*/

static int	symtab_read_64(t_data *data, struct symtab_command *symtab,
							u_long size)
{
	t_nlist			*el;
	char			*stringtable;
	t_symbol		*sym_list;
	int				res;

	el = (t_nlist *)malloc(sizeof(t_nlist));
	el->list64 = (void *)data->ptr + symtab->symoff;
	stringtable = (void *)data->ptr + symtab->stroff;
	if (stringtab_check(stringtable, symtab->strsize, size, symtab->stroff)
		== _STRINGTAB_CORRUPTED)
	{
		free(el);
		return (free_all(NULL, data, _STRINGTAB_CORRUPTED, NULL));
	}
	sym_list = make_sym_list(stringtable, el, symtab->nsyms, _BIN64);
	free(el);
	if ((res = display(sym_list, data)) == _DISPLAY_OK)
		return (free_all(sym_list, data, _EXIT_SUCCESS, NULL));
	return (res);
}

/*
** General function for 64bit binaries
*/

int			bin64(t_meta *file, int nb_args, int swap)
{
	t_data					*data;
	struct symtab_command	*symtab;
	int						ret;

	data = (t_data *)malloc(sizeof(t_data));
	if ((ret = fill_data(file->ptr, &data)) != _DATA_OK)
		return (free_all(NULL, data, ret, file->name));
	if (swap)
		endian_swap(file->ptr + data->symtab->stroff, data->symtab->strsize);
	symtab = data->symtab;
	if (symtab)
	{
		if (nb_args > 1)
		{
			ft_putchar('\n');
			ft_putstr(file->name);
			ft_putendl(":");
		}
		return (symtab_read_64(data, symtab, file->size));
	}
	free(data);
	return (error(_NO_SYMTAB_FAILURE, NULL));
}
