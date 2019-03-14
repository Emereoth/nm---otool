/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian_swaps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 16:43:56 by acottier          #+#    #+#             */
/*   Updated: 2019/03/14 18:03:16 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Swaps the entire mapped zone
*/

char	*endian_swap(char *ptr, size_t size)
{
	size_t		i;
	uint32_t	*bin;

	i = 0;
	size = size / 4;
	bin = (void *)ptr;
	while (i < size)
	{
		bin[i] = OSSwapConstInt32(bin[i]);
		i++;
	}
	return ((char *)bin);
}

/*
** For FAT files: swaps fat headers but leaves rest untouched
*/

char	*fat_swap(char *ptr)
{
	struct fat_header	*h;

	endian_swap(ptr, sizeof(struct fat_header));
	h = (struct fat_header *)ptr;
	endian_swap(ptr + sizeof(struct fat_header),
		h->nfat_arch * sizeof(struct fat_arch));
	return (ptr);
}

/*
** Full- then stringtab- reswap for 32bit archs
*/

t_meta	*full_swap_32(t_meta *file, t_data **data, int *rval)
{
	int						tmp;

	if (check_bounds(file, file->size))
	{
		*rval = _OUT_OF_BOUNDS;
		return (file);
	}
	file->ptr = endian_swap(file->ptr, file->size);
	tmp = fill_data_32(file->ptr, data, file);
	if (tmp)
	{
		*rval = tmp;
		return (file);
	}
	endian_swap(file->ptr + (*data)->symtab->stroff, (*data)->symtab->strsize);
	return (file);
}

/*
** Full- then header- and stringtab- reswap for 64bit archs
*/

int		full_swap_64(t_meta *file, t_data **data)
{
	int		rval;

	rval = 0;
	if (check_bounds(file, file->size))
		return (_OUT_OF_BOUNDS);
	file->ptr = endian_swap(file->ptr, file->size);
	if (check_bounds(file, sizeof(struct mach_header_64)))
		return (_OUT_OF_BOUNDS);
	file->ptr = endian_swap(file->ptr, sizeof(struct mach_header_64));
	if ((rval = fill_data_64(file->ptr, data, file)))
		return (rval);
	endian_swap(file->ptr + (*data)->symtab->stroff, (*data)->symtab->strsize);
	return (_EXIT_SUCCESS);
}
