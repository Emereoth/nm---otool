/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/02/08 13:16:56 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"
#include <stdio.h>

/*
** Returns first section (__text) from the first segment (__TEXT)
*/

static struct section	*get_section(t_data *data)
{
	struct segment_command	*seg;
	struct load_command		*lc;
	struct section			*sect;
	unsigned int			i;

	i = 0;
	lc = data->lc;
	while (i++ < data->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (struct segment_command *)lc;
			sect = (struct section *)(seg + 1);
			if (!ft_strcmp(sect->segname, "__TEXT") &&
				!ft_strcmp(sect->sectname, "__text"))
				return ((struct section *)(seg + 1));
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (NULL);
}

/*
** Display adresses and content of __text section for 32bit
*/

static void				read_section(char *ptr, struct section *section)
{
	unsigned int	length;
	unsigned int	size;

	length = -1;
	if (!section)
		return ;
	size = section->size;
	while (++length < size)
	{
		if (length % 16 == 0)
		{
			ft_putchar('\n');
			display_value((char *)(uintptr_t)section->addr, length, _BIN32);
			ft_putchar('\t');
		}
		show_hex(ptr + section->offset + length);
		ft_putchar(' ');
	}
}

/*
** General function for 32bit binaries
*/

int						bin32(char *ptr, char *file, int swap, int fat)
{
	t_data			*data;
	struct section	*section;
	int				ret;

	data = (t_data *)malloc(sizeof(t_data));
	if ((ret = fill_data_32(ptr, &data)) != _DATA_OK)
		return (free_all(data, ret, file));
	if (swap)
		endian_swap(ptr + data->symtab->stroff, data->symtab->strsize);
	if (!fat)
	{
		ft_putstr(file);
		ft_putendl(":");
	}
	ft_putstr("Contents of (__TEXT,__text) section");
	section = get_section(data);
	read_section(ptr, section);
	ft_putchar('\n');
	free(data);
	return (0);
}
