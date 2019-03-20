/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 12:33:17 by acottier          #+#    #+#             */
/*   Updated: 2019/03/20 16:15:39 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define _LC struct load_command
#define _HEADER struct mach_header
#define _HEADER_64 struct mach_header_64
#define _SYMTAB struct symtab_command

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
** Advance load_comands cursor and updates position data in lc list
*/

static int	move_lc(struct load_command **lc, unsigned int *pos, t_meta *file,
				int swap)
{
	int		header;

	header = (file->arch == _BIN64 ? sizeof(_HEADER_64) : sizeof(_HEADER));
	if (check_bounds(file, header + *pos + (*lc ? (*lc)->cmdsize : 0)))
		return (_OUT_OF_BOUNDS);
	if (!(*lc))
		*lc = (void*)file->ptr + header;
	else
		*lc = (void*)*lc + (*lc)->cmdsize;
	if (swap)
		*lc = (struct load_command*)endian_swap((void*)*lc, sizeof(_LC));
	*pos += (*lc)->cmdsize;
	return (_EXIT_SUCCESS);
}

/*
** Create and fill data structure for current file
*/

int			fill_data(char *ptr, t_data **data, t_meta *file, int swap)
{
	struct load_command		*lc_cursor;
	struct mach_header_64	*header;
	unsigned int			i;
	unsigned int			pos;

	if (check_bounds(file, sizeof(_HEADER_64)))
		return (_OUT_OF_BOUNDS);
	header = (_HEADER_64*)(swap ? endian_swap(ptr, sizeof(_HEADER_64)) : ptr);
	lc_cursor = NULL;
	prefill(data, ptr, header->ncmds, (void*)ptr + sizeof(_HEADER_64));
	(*data)->filetype = _BIN64;
	i = 0;
	pos = 0;
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		if (move_lc(&lc_cursor, &pos, file, swap) == _OUT_OF_BOUNDS)
			return (_OUT_OF_BOUNDS);
		if (lc_cursor->cmd == LC_SYMTAB)
			(*data)->symtab = (_SYMTAB *)lc_cursor;
		i++;
	}
	if ((*data)->symtab && swap)
		(*data)->symtab = (_SYMTAB *)endian_swap((void*)(*data)->symtab,
			sizeof(_SYMTAB));
	return ((*data)->symtab ? _DATA_OK : _NO_SYMTAB_FAILURE);
}

/*
** Create and fill data structure for 32-bit arch
*/

int			fill_data_32(char *ptr, t_data **data, t_meta *file, int swap)
{
	struct load_command		*lc_cursor;
	struct mach_header		*header;
	unsigned int			i;
	unsigned int			pos;

	if (check_bounds(file, sizeof(_HEADER)))
		return (_OUT_OF_BOUNDS);
	header = (_HEADER*)(swap ? endian_swap(ptr, sizeof(_HEADER)) : ptr);
	lc_cursor = NULL;
	prefill(data, ptr, header->ncmds, (void *)ptr + sizeof(_HEADER));
	(*data)->filetype = _BIN32;
	i = 0;
	pos = 0;
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		if (move_lc(&lc_cursor, &pos, file, swap) == _OUT_OF_BOUNDS)
			return (_OUT_OF_BOUNDS);
		if (lc_cursor->cmd == LC_SYMTAB)
			(*data)->symtab = (_SYMTAB *)lc_cursor;
		i++;
	}
	if ((*data)->symtab && swap)
		(*data)->symtab = (_SYMTAB *)endian_swap((void*)(*data)->symtab,
			sizeof(_SYMTAB));
	return ((*data)->symtab ? _DATA_OK : _NO_SYMTAB_FAILURE);
}
