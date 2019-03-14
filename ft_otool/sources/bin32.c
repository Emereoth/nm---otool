/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:38:24 by acottier          #+#    #+#             */
/*   Updated: 2019/03/14 14:32:20 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Returns first section (__text) from the first segment (__TEXT)
*/

static struct section	*get_section(t_data *data, int *rval)
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
			{
				*rval = _EXIT_SUCCESS;
				return ((struct section *)(seg + 1));
			}
		} 
		lc = (void *)lc + lc->cmdsize;
	}
	*rval = _SECTION_NOT_FOUND;
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
	int						rval;
	
	rval = 0;
	data = (t_data *)malloc(sizeof(t_data));
	if (swap)
	{
		if ((file = full_swap_32(file, &data, 0)) && rval != _EXIT_SUCCESS)
			return (free_all(data, rval));
	}
	else if ((rval = fill_data_32(file->ptr, &data, file)))
		return (free_all(data, rval));
	if ((rval = stringtab_check(file, data->symtab)))
		return (free_all(data, rval));
	if (!fat)
		ft_doublestr(file->name, ":\n");
	if (!(section = get_section(data, &rval)))
		return (free_all(data, rval));
	ft_putstr("Contents of (__TEXT,__text) section");
	read_section(file->ptr, section);
	ft_putchar('\n');
	free(data);
	return (_EXIT_SUCCESS);
}
