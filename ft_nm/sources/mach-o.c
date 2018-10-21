/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach-o.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2018/10/21 15:15:12 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static int	symtab_read(t_data *data, int nsyms, int symoff, int stroff)
{
	int				i;
	struct nlist_64	*el;
	char			*stringtable;
	t_symbol		*sym_list;
	int				res;

	i = 0;
	el = (void *)data->ptr + symoff;
	stringtable = (void *)data->ptr + stroff;
	sym_list = make_sym_list(stringtable, el, nsyms);
	if ((res = display(sym_list, data)) == _DISPLAY_OK)
		return (free_all(sym_list, data, _EXIT_SUCCESS, NULL));
	return (res);
}

char		get_symbol_type(uint8_t n_type, uint8_t n_sect)
{
	if ((n_type & N_SECT) == N_SECT)
	{
		if (n_sect == NO_SECT)
			return ('X');
		return ('S');
	}
	else if ((n_type & N_UNDF) == N_UNDF)
		return ('U');
	else if ((n_type & N_ABS) == N_ABS)
		return ('A');
	else if ((n_type & N_PBUD) == N_PBUD)
		return ('U');
	else if ((n_type & N_INDR) == N_INDR)
		return ('I');
	return ('X');
}

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
		return (symtab_read(data, symtab->nsyms, symtab->symoff, symtab->stroff));
	}
	free(data);
	return (error(_NO_SYMTAB_FAILURE, NULL));
}