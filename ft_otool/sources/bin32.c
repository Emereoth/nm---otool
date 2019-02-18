/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/02/18 16:22:08 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

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

int						bin32(t_meta *file, int swap, int fat)
{
	t_data					*data;
	struct section			*section;
	int						ret;
	struct symtab_command	*symtab;

	data = (t_data *)malloc(sizeof(t_data));
	if ((ret = fill_data_32(file->ptr, &data, file)) != _DATA_OK)
		return (free_all(data, ret));
	if (swap)
		endian_swap(file->ptr + data->symtab->stroff, data->symtab->strsize);
	symtab = data->symtab;
	if (stringtab_check((void*)data->ptr + symtab->stroff, symtab->strsize,
			file->size, symtab->stroff) == _STRINGTAB_CORRUPTED)
		return (free_all(data, _STRINGTAB_CORRUPTED));
	if (!fat)
	{
		ft_putstr(file->name);
		ft_putendl(":");
	}
	ft_putstr("Contents of (__TEXT,__text) section");
	section = get_section(data);
	read_section(file->ptr, section);
	ft_putchar('\n');
	free(data);
	return (0);
}
