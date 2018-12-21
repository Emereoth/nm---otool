/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2018/12/21 15:59:55 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"
#include <stdio.h>

/*
** Returns first section (__text) from the first segment (__TEXT)
*/

static struct section_64	*get_section_64(t_data *data)
{
	struct section_64			*res;
	struct segment_command_64	*seg;
	struct load_command			*lc;
	unsigned int				i;

	i = 0;
	lc = data->lc;
	while (i++ < data->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			res = (struct section_64 *)(seg + 1 + i);
			return (res);
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (NULL);
}

static void					display_value(char *addr, unsigned int length)
{
	int		padding;

	padding = 16 - ft_strlen(show_address(addr + length));
	while (padding-- > 0)
		ft_putchar('0');
	ft_putstr(show_address(addr + length));
}

static void					show_hex(char *cursor)
{
	// static char	table[16] = "0123456789abcdef";

	ft_putnbr(*cursor);
	// write(0, &table[*cursor / 16], 1);
	// write(0, &table[*cursor % 16], 1);
}

static void					read_section(struct section_64 *section)
{
	unsigned int	length;

	length = -1;
	while (++length < section->size)
	{
		if (length % 16 == 0)
		{
			ft_putchar('\n');
			display_value((char *)section->addr, length);
			ft_putstr("        ");
		}
		ft_putchar(' ');
		show_hex((char *)section + length);
	}
}

/*
** General function for 64bit binaries
*/

int							bin64(char *ptr, char *file, int nb_args, int swap)
{
	t_data					*data;
	struct section_64		*section;

	data = (t_data *)malloc(sizeof(t_data));
	fill_data(ptr, &data);
	if (swap)
		endian_swap(ptr + data->symtab->stroff, data->symtab->strsize);
	ft_putstr(file);
	ft_putchar('\n');
	ft_putendl("Contents of (__TEXT,__text) section");
	section = get_section_64(data);
	ft_putstr("section: ");
	ft_putendl(section->segname);
	read_section(section);
	free(data);
	(void)nb_args;
	return (0);
	// return (error(_NO_SYMTAB_FAILURE, NULL));
}
