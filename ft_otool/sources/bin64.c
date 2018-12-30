/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2018/12/30 15:01:27 by acottier         ###   ########.fr       */
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
	int				sign;
	static char	table[16] = "0123456789abcdef";
	unsigned char	nb;

	sign = *(cursor)>= 0 ? 1 : -1;
	nb = *cursor;
	ft_putchar(table[nb / 16]);
	ft_putchar(table[nb % 16]);
}

static void					read_section(char *ptr, struct section_64 *section)
{
	unsigned int	length;
	unsigned int	size;

	length = -1;
	size = section->size;
	while (++length < size)
	{
		if (length % 16 == 0)
		{
			ft_putchar('\n');
			display_value((char *)section->addr, length);
			ft_putchar('\t');
		}
		show_hex(ptr + section->offset + length);
		ft_putchar(' ');
	}
	if (length % 16 != 0)
		ft_putchar('\n');
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
	ft_putendl(":");
	ft_putstr("Contents of (__TEXT,__text) section");
	section = get_section_64(data);
	read_section(ptr, section);
	free(data);
	(void)nb_args;
	return (0);
	// return (error(_NO_SYMTAB_FAILURE, NULL));
}
