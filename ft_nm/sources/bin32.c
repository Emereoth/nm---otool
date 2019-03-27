/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/03/27 13:26:17 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define NLIST struct nlist

/*
** Create symbol list, display it and free
*/

static int			symtab_read_32(t_data *data, struct symtab_command *symtab,
									u_long size, int swap)
{
	t_nlist			*el;
	char			*stringtable;
	t_symbol		*sym_list;
	int				res;

	el = (t_nlist *)malloc(sizeof(t_nlist));
	el->list32 = (void*)data->ptr + symtab->symoff;
	// dump((void*)el->list32, sizeof(NLIST), symtab->symoff);
	// ft_putstr("n_strx (4) : ");
	// ft_putnbr(el->list32->n_un.n_strx);
	// ft_putstr(" | n_type (1) : ");
	// ft_putnbr(el->list32->n_type);
	// ft_putstr(" | n_sect (1) : ");
	// ft_putnbr(el->list32->n_sect);
	// ft_putstr(" | n_desc (2) : ");
	// ft_putnbr(el->list32->n_desc);
	// ft_putstr(" | n_value (4) : ");
	// ft_putnbr(el->list32->n_value);
	// ft_putchar('\n');
	if (swap)
	{
		endian_swap((void*)el->list32, symtab->nsyms * sizeof(NLIST));
		// ft_putstr("n_strx (4) : ");
		// ft_putnbr(el->list32->n_un.n_strx);
		// ft_putstr(" | n_type (1) : ");
		// ft_putnbr(el->list32->n_type);
		// ft_putstr(" | n_sect (1) : ");
		// ft_putnbr(el->list32->n_sect);
		// ft_putstr(" | n_desc (2) : ");
		// ft_putnbr(el->list32->n_desc);
		// ft_putstr(" | n_value (4) : ");
		// ft_putnbr(el->list32->n_value);
		// ft_putchar('\n');
	}
	// dump((void*)el->list32, sizeof(NLIST), symtab->symoff);
	stringtable = data->ptr + symtab->stroff;
	if (stringtab_check(stringtable, symtab->strsize, size, symtab->stroff)
		== _STRINGTAB_CORRUPTED)
	{
		free(el);
		free(data);
		return (_STRINGTAB_CORRUPTED);
	}
	sym_list = make_sym_list(stringtable, el, symtab->nsyms, _BIN32);
	free(el);
	if ((res = display(sym_list, data)) == _DISPLAY_OK)
		return (free_all(sym_list, data, _EXIT_SUCCESS));
	return (res);
}

/*
** General function for 64bit binaries
*/

int					bin32(t_meta *file, int nb_args, int swap)
{
	t_data					*data;
	struct symtab_command	*symtab;
	int						ret;

	file->arch = _BIN32;
	data = (t_data *)malloc(sizeof(t_data));
	if ((ret = fill_data_32(file->ptr, &data, file, swap)) != _DATA_OK)
		return (free_all(NULL, data, ret));
	symtab = data->symtab;
	if (symtab)
	{
		if (nb_args > 1)
		{
			ft_putchar('\n');
			ft_putstr(file->name);
			ft_putendl(":");
		}
		return (symtab_read_32(data, symtab, file->size, swap));
	}
	free(data);
	return (_NO_SYMTAB_FAILURE);
}
