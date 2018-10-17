/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach-o.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2018/10/17 17:01:46 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static int	symtab_read(char *ptr, int nsyms, int symoff, int stroff)
{
	int				i;
	struct nlist_64	*el;
	char			*stringtable;
	t_symbol		*sym_list;

	i = 0;
	el = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	sym_list = make_sym_list(stringtable, el, nsyms);
	display(sym_list);
	free_sym_list(sym_list);
	return (_EXIT_SUCCESS);
}

int			bin64(char *ptr, char *file)
{
	struct mach_header_64	*header;
	struct load_command		*lc_cursor;
	struct symtab_command	*symtab;
	unsigned int			i;

	i = 0;
	header = (struct mach_header_64 *)ptr;
	lc_cursor = (void *) ptr + sizeof(*header);
	symtab = NULL;
	while (i < header->ncmds && !symtab)
	{
		if (lc_cursor->cmd == LC_SYMTAB)
			symtab = (struct symtab_command *)lc_cursor;
		i++;
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (symtab)
	{
		ft_putstr(file);
		ft_putendl(":");
		return (symtab_read(ptr, symtab->nsyms, symtab->symoff, symtab->stroff));
	}
	return (error(_NO_SYMTAB_FAILURE, NULL));
}