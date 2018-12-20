/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 12:33:17 by acottier          #+#    #+#             */
/*   Updated: 2018/12/19 11:38:42 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Destroy and free data struct
*/

int		free_all(t_symbol *list, t_data *data, int errcode, char *str)
{
	free_sym_list(list);
	if (data)
		free(data);
	return (errcode == _EXIT_SUCCESS ? 0 : error(errcode, str));
}

/*
** Create and fill data structure for current file
*/

int		fill_data(char *ptr, t_data **data)
{
	struct mach_header_64	*header;
	struct load_command		*lc_cursor;
	struct symtab_command	*symtab;
	unsigned int			i;

	i = 0;
	symtab = NULL;
	header = (struct mach_header_64 *)ptr;
	lc_cursor = (void *)ptr + sizeof(*header);
	(*data)->filetype = _BIN64;
	(*data)->ptr = ptr;
	(*data)->ncmds = header->ncmds;
	(*data)->lc = lc_cursor;
	while (i < header->ncmds && !symtab)
	{
		if (lc_cursor->cmd == LC_SYMTAB)
			symtab = (struct symtab_command *)lc_cursor;
		i++;
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!symtab)
		return (_NO_SYMTAB_FAILURE);
	(*data)->symtab = symtab;
	return (_DATA_OK);
}

/*
** Create and fill data structure for 32-bit arch
*/

int		fill_data_32(char *ptr, t_data **data)
{
	struct mach_header		*header;
	struct load_command		*lc_cursor;
	struct symtab_command	*symtab;
	unsigned int			i;

	i = 0;
	symtab = NULL;
	header = (struct mach_header *)ptr;
	lc_cursor = (void *)ptr + sizeof(*header);
	(*data)->filetype = _BIN32;
	(*data)->ptr = ptr;
	(*data)->ncmds = header->ncmds;
	(*data)->lc = lc_cursor;
	while (i < header->ncmds && !symtab)
	{
		if (lc_cursor->cmd == LC_SYMTAB)
			symtab = (struct symtab_command *)lc_cursor;
		i++;
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!symtab)
		return (_NO_SYMTAB_FAILURE);
	(*data)->symtab = symtab;
	return (_DATA_OK);
}
