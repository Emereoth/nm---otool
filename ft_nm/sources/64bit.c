/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   64bit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2018/11/27 13:48:32 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Create temporary nlist structure corresponding to filetype
*/

// static t_newnlist	*make_newnlist_64(char *ptr, int symoff)
// {
// 	struct nlist_64	*el;
// 	t_newnlist		*res;

// 	el = (void *)ptr + symoff;
// 	res = (t_newnlist *)malloc(sizeof(t_newnlist));
// 	res->n_un = el->n_un.n_strx;
// 	res->n_type = el->n_type;
// 	res->n_sect = el->n_sect;
// 	res->n_desc = el->n_desc;
// 	res->n_value = el->n_value;
// 	return (res);
// }

/*
** Create symbol list, display it and free
*/

static int	symtab_read_64(t_data *data, int nsyms, int symoff, int stroff)
{
	t_nlist			*el;
	char			*stringtable;
	t_symbol		*sym_list;
	int				res;

	el = (t_nlist *)malloc(sizeof(t_nlist));
	el->list32 = NULL;
	el->list64 = (void *)data->ptr + symoff;
	stringtable = (void *)data->ptr + stroff;
	sym_list = make_sym_list(stringtable, el, nsyms, _BIN64);
	free(el);
	if ((res = display(sym_list, data)) == _DISPLAY_OK)
		return (free_all(sym_list, data, _EXIT_SUCCESS, NULL));
	return (res);
}

/*
** General function for 64bit binaries
*/

int			bin64(char *ptr, char *file, int nb_args)
{
	t_data					*data;
	struct symtab_command	*symtab;

	data = (t_data *)malloc(sizeof(t_data));
	fill_data(ptr, &data);
	symtab = data->symtab;
	if (symtab)
	{
		if (nb_args > 1)
		{
			ft_putstr(file);
			ft_putendl(":");
		}
		return (symtab_read_64(data, symtab->nsyms, symtab->symoff,
				symtab->stroff));
	}
	free(data);
	return (error(_NO_SYMTAB_FAILURE, NULL));
}