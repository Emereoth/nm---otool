/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bounds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:20:53 by acottier          #+#    #+#             */
/*   Updated: 2019/02/18 14:16:28 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Checks if moving pointer will exceed file bounds
*/

int		check_bounds(t_meta *file, u_long offset)
{
	if (offset >= file->size)
		return (_OUT_OF_BOUNDS);
	return (_EXIT_SUCCESS);
}

/*
** Checks bounds before jumping to next structure in archive
*/

int		arch_structures(t_meta *file, int *cputype, int i,
			unsigned int *magicnb)
{
	struct fat_arch		*arch;

	if (check_bounds(file, sizeof(struct fat_header) + sizeof(*arch) * i))
		return (_OUT_OF_BOUNDS);
	arch = (struct fat_arch *)(file->ptr + sizeof(struct fat_header) +
		sizeof(*arch) * i);
	if (check_bounds(file, arch->offset))
		return (_OUT_OF_BOUNDS);
	*magicnb = *(unsigned int *)(file->ptr + arch->offset);
	*cputype = arch->cputype;
	return (_EXIT_SUCCESS);
}

/*
** Checks bounds before sending next object in archive to magic_reader()
*/

int		check_object_bounds(t_meta *file, u_long obj, int namesize)
{
	if (check_bounds(file, (u_long)obj + 60 + namesize))
		return (_OUT_OF_BOUNDS);
	if (check_bounds(file, (u_long)obj + 48))
		return (_OUT_OF_BOUNDS);
	return (_EXIT_SUCCESS);
}
