/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 12:33:17 by acottier          #+#    #+#             */
/*   Updated: 2019/02/15 15:21:06 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Destroy and free data struct
*/

int			free_all(t_symbol *list, t_data *data, int errcode)
{
	free_sym_list(list);
	if (data)
		free(data);
	return (errcode);
}

/*
** Fills t_data struct with generic fields
*/

static void	prefill(t_data **data, char *ptr, unsigned int ncmds,
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

int			fill_data(char *ptr, t_data **data, t_meta *file)
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
	return (_DATA_OK);
}

/*
** Create and fill data structure for 32-bit arch
*/

int			fill_data_32(char *ptr, t_data **data, t_meta *file)
{
	struct load_command		*lc_cursor;
	unsigned int			i;
	unsigned int			pos;

	i = 0;
	if (check_bounds(file, sizeof(struct mach_header)))
		return (_OUT_OF_BOUNDS);
	lc_cursor = (void *)ptr + sizeof(struct mach_header);
	prefill(data, ptr, ((struct mach_header*)ptr)->ncmds,
		(void *)ptr + sizeof(struct mach_header));
	(*data)->filetype = _BIN32;
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		pos = (sizeof(struct mach_header) + i * sizeof(lc_cursor)->cmdsize);
		if (lc_cursor->cmd == LC_SYMTAB)
			(*data)->symtab = (struct symtab_command *)lc_cursor;
		i++;
		if (check_bounds(file, pos))
			return (_OUT_OF_BOUNDS);
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!(*data)->symtab)
		return (_NO_SYMTAB_FAILURE);
	return (_DATA_OK);
}
