/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 11:04:55 by acottier          #+#    #+#             */
/*   Updated: 2019/01/24 12:11:58 by acottier         ###   ########.fr       */
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
	return ("undefined cpu type");
}

/*
** Show architecture name
*/

void		show_name(cpu_type_t cpu, char *file, int to_show)
{
	if (to_show == _HIDE)
		return ;
	ft_putstr(file);
	if (to_show == _SHOW_WITH_ARCH)
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
** Assigns priority to each architecture, telling if it should be displayed, displayed with archname
** or not shown at all
*/

int			determine_priority(unsigned int magicnb, int **tab,
								int *bin32, int i)
{
	if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
	{
		if (*bin32 == -1)
		{
			*bin32 = i;
			return (_SHOW);
		}
		return (_HIDE);
	}
	if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
	{
		if (*bin32 != -1)
			*tab[*bin32] = _HIDE;
		*bin32 = _DISABLE_32;
		return (_SHOW);
	}
	return (_SHOW_WITH_ARCH);
}