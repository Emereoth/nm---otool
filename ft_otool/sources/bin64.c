/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/02/15 15:23:24 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"
#include <stdio.h>

/*
** Returns first section (__text) from the first segment (__TEXT)
*/

static struct section_64	*get_section_64(t_data *data)
{
	struct segment_command_64	*seg;
	struct load_command			*lc;
	struct section_64			*sect;
	unsigned int				i;

	i = 0;
	lc = data->lc;
	while (i++ < data->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			sect = (struct section_64 *)(seg + 1);
			if (!ft_strcmp(sect->segname, "__TEXT") &&
				!ft_strcmp(sect->sectname, "__text"))
				return ((struct section_64 *)(seg + 1));
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (NULL);
}

/*
** Show adress of current emplacement in section
*/

void						display_value(char *addr, unsigned int length,
											int int_size)
{
	int		padding;

	padding = (int_size == _BIN64 ? 16 : 8);
	padding -= ft_strlen(show_address(addr + length));
	while (padding-- > 0)
		ft_putchar('0');
	ft_putstr(show_address(addr + length));
}

/*
** Display current byte in hex
*/

void						show_hex(char *cursor)
{
	int				sign;
	static char		table[16] = "0123456789abcdef";
	unsigned char	nb;

	sign = *(cursor) >= 0 ? 1 : -1;
	nb = *cursor;
	ft_putchar(table[nb / 16]);
	ft_putchar(table[nb % 16]);
}

/*
** Display adresses and content of __text section for 64bit
*/

static void					read_section_64(char *ptr,
							struct section_64 *section)
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
			display_value((char *)section->addr, length, _BIN64);
			ft_putchar('\t');
		}
		show_hex(ptr + section->offset + length);
		ft_putchar(' ');
	}
}

/*
** General function for 64bit binaries
*/

int							bin64(t_meta *file, int swap, int fat)
{
	t_data					*data;
	struct section_64		*section;
	int						ret;
	struct symtab_command	*symtab;

	data = (t_data *)malloc(sizeof(t_data));
	if ((ret = fill_data(file->ptr, &data, file)) != _DATA_OK)
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
	section = get_section_64(data);
	read_section_64(file->ptr, section);
	ft_putchar('\n');
	free(data);
	return (0);
}
