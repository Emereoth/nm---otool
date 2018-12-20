/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2018/12/20 15:52:05 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
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

static void					read_section(struct section_64 *section)
{
	unsigned int	length;

	length = -1;
	ft_putendl("ayy lmao reading section");
	ft_putnbr(section->size);
	while (++length < section->size)
	{
		if (length % 16 == 0)
		{
			ft_putchar('\n');
			show_address((char *)section + length);
			ft_putstr("        ");
		}
		ft_putchar(' ');
		ft_putstr(ft_to_hex((uint64_t)section + length));
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
