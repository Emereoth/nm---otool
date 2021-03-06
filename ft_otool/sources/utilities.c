/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 11:04:55 by acottier          #+#    #+#             */
/*   Updated: 2019/03/15 15:32:27 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"
#include <stdio.h>

/*
** Returns name of the CPU type
*/

static char	*cpu_name_list(cpu_type_t cpu)
{
	if (cpu == CPU_TYPE_I386)
		return ("i386");
	if (cpu == CPU_TYPE_X86_64)
		return ("x86_64");
	if (cpu == CPU_TYPE_POWERPC)
		return ("ppc");
	return ("undefined cpu type");
}

/*
** Shows architecture name
*/

void		show_name(cpu_type_t cpu, char *file, int to_show)
{
	if (to_show == _SHOW_AS_ARCH)
		return ;
	ft_putstr(file);
	if (to_show == _SHOW_WITH_TYPE || cpu == CPU_TYPE_POWERPC)
	{
		ft_putstr(" (architecture ");
		ft_putstr(cpu_name_list(cpu));
		ft_putchar(')');
	}
	ft_putstr(":\n");
}

/*
** Displays (in hexadecimal) the address of the specified pointer.
*/

char		*show_address(char *address)
{
	return (ft_to_hex((uint64_t)address));
}

/*
** Assigns priority to each architecture, telling if it should be:
** - displayed,
** - displayed with archname
** - or not shown at all
*/

int			determine_priority(unsigned int magicnb, int **tab,
								int *highest, int tabsize)
{
	int	i;

	i = -1;
	if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
	{
		if (*highest == 64)
			return (_HIDE);
		return (_SHOW_32);
	}
	if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
	{
		if (*highest != 64)
		{
			while (++i < tabsize)
			{
				if ((*tab)[i] == _SHOW_32)
					(*tab)[i] = _HIDE;
			}
			*highest = 64;
			return (_SHOW);
		}
		return (_HIDE);
	}
	return (_SHOW_WITH_TYPE);
}

/*
** Determine if archive must be shown (fats will only display one archive)
*/

int			archive_priority(void)
{
	static int	archive_nb = 0;

	if (archive_nb > 0)
		return (_HIDE);
	archive_nb++;
	return (_SHOW_AS_ARCH);
}
