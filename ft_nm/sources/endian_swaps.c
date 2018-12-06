/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian_swaps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 16:43:56 by acottier          #+#    #+#             */
/*   Updated: 2018/12/06 17:20:15 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

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
	char				*start;
	struct fat_header	*h;

	start = endian_swap(ptr, 8);
	h = (struct fat_header *)start;
	ptr = endian_swap(start + 8, h->nfat_arch * sizeof(struct fat_arch));
	return (start);
}
