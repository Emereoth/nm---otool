/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 11:04:55 by acottier          #+#    #+#             */
/*   Updated: 2019/01/14 10:16:47 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include <stdio.h>

int			determine_priority(int *prio, unsigned int magicnb, int *bin32,
			int **tab)
{
	if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
	{
		if (*prio != _64_ONLY)
		{
			*bin32 = 1;
			*prio = _32_ONLY;
			return (1);
		}
		return (0);
	}
	if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
	{
		if (*bin32 == 1)
			*(tab[0]) -= 1;
		*prio = _64_ONLY;
		return (1);
	}
	return (1);
}

static char	*cpu_name_list(cpu_type_t cpu)
{
	if (cpu == CPU_TYPE_I386)
		return ("i386");
	if (cpu == CPU_TYPE_X86_64)
		return ("x86_64");
	return ("undefined cpu type");
}

void		show_arch(int archnb, cpu_type_t cpu, char *file)
{
	if (archnb <= 1)
		return ;
	ft_putchar('\n');
	ft_putstr(file);
	ft_putstr(" (for achitecture ");
	ft_putstr(cpu_name_list(cpu));
	ft_putendl("):");
}
