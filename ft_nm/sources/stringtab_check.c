/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringtab_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 09:26:39 by acottier          #+#    #+#             */
/*   Updated: 2019/02/13 17:26:06 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Checks stringtable integrity
*/

int		stringtab_check(char *stringtable, uint32_t strtab_size,
			u_long filesize, int stroff)
{
	u_long		i;
	int			zeroes;
	int			entry_padding;

	i = 0;
	zeroes = 0;
	entry_padding = 0;
	while (i < strtab_size && stroff + i <= filesize)
	{
		if (entry_padding && *(stringtable + i) == 0)
			zeroes++;
		if (*(stringtable + i) != 0)
			entry_padding++;
		i++;
	}
	if (zeroes == 0 || i != strtab_size)
		return (_STRINGTAB_CORRUPTED);
	return (_STRINGTAB_OK);
}
