/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 12:33:17 by acottier          #+#    #+#             */
/*   Updated: 2019/03/08 13:34:30 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Destroy and free data struct
*/

int			free_all(t_data *data, int errcode)
{
	if (data)
		free(data);
	return (errcode);
}

/*
** Fills t_data struct with generic fields
*/

void		prefill(t_data **data, char *ptr, unsigned int ncmds,
					struct load_command *lc)
{
	(*data)->ptr = ptr;
	(*data)->ncmds = ncmds;
	(*data)->lc = lc;
	(*data)->symtab = NULL;
}

/*
** Create and fill data structure for current file
*/

int			fill_data_64(char *ptr, t_data **data, t_meta *file)
{
	struct load_command		*lc_cursor;
	unsigned int			i;
	unsigned int			pos;

	i = 0;
	if (check_bounds(file, sizeof(struct mach_header_64)))
		return (_OUT_OF_BOUNDS);
	lc_cursor = (void *)ptr + sizeof(struct mach_header_64);
	prefill(data, ptr, ((struct mach_header_64 *)ptr)->ncmds,
		(void *)ptr + sizeof(struct mach_header_64));
	(*data)->filetype = _BIN64;
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		pos = (sizeof(struct mach_header_64) + i * sizeof(lc_cursor)->cmdsize);
		if (lc_cursor->cmd == LC_SYMTAB)
			(*data)->symtab = (struct symtab_command *)lc_cursor;
		i++;
		if (check_bounds(file, pos))
			return (_OUT_OF_BOUNDS);
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!(*data)->symtab)
		return (_NO_SYMTAB_FAILURE);
	return (_EXIT_SUCCESS);
}

/*
** Create and fill data structure for 32-bit arch
*/

int			fill_data_32(char *ptr, t_data **data, t_meta *file)
{
	struct load_command		*lc_cursor;
	unsigned int			i;
	unsigned int			pos;
	struct symtab_command	*tmp;

	i = 0;
	if (check_bounds(file, sizeof(struct mach_header)))
		return (_OUT_OF_BOUNDS);
	lc_cursor = (void *)ptr + sizeof(struct mach_header);
	prefill(data, ptr, ((struct mach_header*)ptr)->ncmds,
		(void *)ptr + sizeof(struct mach_header));
	(*data)->filetype = _BIN32;
	pos = (sizeof(struct mach_header));
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		pos += (i > 0 ? lc_cursor->cmdsize : 0);
		if (lc_cursor->cmd == LC_SYMTAB)
		{
			(*data)->symtab = (struct symtab_command *)lc_cursor;
			tmp = (*data)->symtab;
		}
		i++;
		if (check_bounds(file, pos))
			return (_OUT_OF_BOUNDS);
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!(*data)->symtab)
		return (_NO_SYMTAB_FAILURE);
	return (_EXIT_SUCCESS);
}

/*
** Create new meta structure (info about file size and pointers),
** needed for corruption protection
*/

t_meta		*new_master(char *name, char *ptr, u_long size)
{
	t_meta	*res;

	res = (t_meta *)malloc(sizeof(t_meta));
	res->ptr = ptr;
	res->name = name;
	res->size = size;
	return (res);
}
