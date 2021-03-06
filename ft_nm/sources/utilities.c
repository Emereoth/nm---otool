/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 11:04:55 by acottier          #+#    #+#             */
/*   Updated: 2019/03/17 12:04:14 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include <stdio.h>

int			determine_priority(unsigned int magicnb, int *bin64)
{
	if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
	{
		*bin64 = 1;
		return (_SHOW);
	}
	return (_SHOW_WITH_TYPE);
	// if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
	// {
	// 	if (*bin32 == -1)
	// 	{
	// 		*bin32 = i;
	// 		return (_SHOW);
	// 	}
	// 	return (_HIDE);
	// }
	// if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
	// {
	// 	if (*bin32 != -1)
	// 		*tab[*bin32] = _HIDE;
	// 	*bin32 = _DISABLE_32;
	// 	return (_SHOW);
	// }
	// return (_SHOW_WITH_TYPE);
}

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

void		show_arch(int archnb, cpu_type_t cpu, char *file)
{
	if (archnb <= 1 || archnb == _SHOW_AS_ARCH)
		return ;
	ft_putchar('\n');
	ft_putstr(file);
	ft_putstr(" (for architecture ");
	ft_putstr(cpu_name_list(cpu));
	ft_putendl("):");
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

/*
** Create new meta structure (info about file size and pointers),
** needed for corruption protection
*/

t_meta		*new_master(char *name, char *ptr, u_long size)
{
	t_meta	*res;

	res = (t_meta *)malloc(sizeof(t_meta));
	res->ptr = ptr;
	res->name = name;
	res->size = size;
	return (res);
}
