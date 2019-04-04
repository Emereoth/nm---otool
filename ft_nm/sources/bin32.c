/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/04/04 14:25:04 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define NLIST struct nlist

static void			swap_nlist(void *el32, uint32_t nsyms)
{
	uint32_t		i;

	i = 0;
	while (i < nsyms)
	{
		endian_swap(el32, 4);
		el32 += 8;
		endian_swap(el32, 4);
		el32 += 4;
		i++;
	}
}

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
	stringtable = data->ptr + symtab->stroff;

	// for (uint32_t i = 0 ; i < symtab->nsyms ; i++)
	// {
	// 	if (swap)
	// 		endian_swap((void*)el->list32 + sizeof(NLIST) * i, sizeof(NLIST));
	// 	ft_putendl(stringtable + el->list32[i].n_un.n_strx);
	// 	if (swap)
	// 		endian_swap((void*)el->list32 + sizeof(NLIST) * i, sizeof(NLIST));
	// 	dump((void*)el->list32 + sizeof(NLIST) * i, sizeof(NLIST), 0);
	// 	dump_bin((void*)el->list32 + sizeof(NLIST) * i, sizeof(NLIST), 0);
	// 	if (swap)
	// 	{
	// 		swap_nlist((void*)el->list32, symtab->nsyms);
	// 		dump((void*)el->list32 + sizeof(NLIST) * i, sizeof(NLIST), 0);
	// 		dump_bin((void*)el->list32 + sizeof(NLIST) * i, sizeof(NLIST), 0);
	// 		swap_nlist((void*)el->list32, symtab->nsyms);
	// 	}
	// }

	// dump_bin((void*)el->list32, sizeof(NLIST), symtab->symoff);
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
		// endian_swap((void*)el->list32, symtab->nsyms * sizeof(NLIST));
		swap_nlist((void*)el->list32, symtab->nsyms);
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
	// dump_bin((void*)el->list32, sizeof(NLIST), symtab->symoff);
	// dump((void*)el->list32, sizeof(NLIST), symtab->symoff);
	if (stringtab_check(stringtable, symtab->strsize, size, symtab->stroff)
		== _STRINGTAB_CORRUPTED)
	{
		free(el);
		free(data);
		return (_STRINGTAB_CORRUPTED);
	}
	sym_list = make_sym_list(stringtable, el, symtab->nsyms, _BIN32);
	free(el);
	if ((res = display(sym_list, data, swap)) == _DISPLAY_OK)
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
	// dump(file->ptr, file->size, 0);
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
