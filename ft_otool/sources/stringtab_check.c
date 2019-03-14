/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringtab_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 09:26:39 by acottier          #+#    #+#             */
/*   Updated: 2019/02/19 16:25:05 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Checks stringtable integrity
*/

int		stringtab_check(t_meta *file, struct symtab_command *symtab)
{
	u_long		i;
	int			zeroes;
	int			entry_padding;
	char		*stringtable;

	i = 0;
	zeroes = 0;
	entry_padding = 0;
	stringtable = file->ptr + symtab->stroff;
	if (check_bounds(file, symtab->stroff + symtab->strsize))
		return (_OUT_OF_BOUNDS);
	while (i < symtab->strsize && symtab->stroff + i <= file->size)
	{
		if (entry_padding && *(stringtable + i) == 0)
			zeroes++;
		if (*(stringtable + i) != 0)
			entry_padding++;
		i++;
	}
	if (zeroes == 0 || i != symtab->strsize)
		return (_STRINGTAB_CORRUPTED);
	return (_EXIT_SUCCESS);
}
