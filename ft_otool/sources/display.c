/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 17:01:06 by acottier          #+#    #+#             */
/*   Updated: 2018/12/19 11:38:42 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include <stdio.h>

/*
** Display symbol value
*/

static void	display_value(t_info *info, int filetype)
{
	char	*value;
	int		padding;

	if (filetype == _BIN32)
		value = ft_to_hex_32(info->n_value.val_32);
	else
		value = ft_to_hex(info->n_value.val_64);
	padding = (filetype == _BIN32 ? 8 : 16 );
	if ((info->n_type & N_TYPE) == N_SECT)
	{
		padding -= ft_strlen(value);
		while (padding-- > 0)
			ft_putchar('0');
		ft_putstr(value);
	}
	else
		ft_putstr(filetype == _BIN32 ? "        " : "                ");
	ft_putchar(' ');
}

/*
** Use appropriate function to filetype
*/

static char	get_sector_type(t_data *data, t_symbol *list)
{
	static char	(*f[2]) (t_data *data, uint8_t n_sect, struct load_command *lc);
	char		segment;

	f[0] = &browse_sector_bin32;
	f[1] = &browse_sector_bin64;
	segment = f[data->filetype](data, list->s_info->n_sect, data->lc);
	if (segment != 'T' && segment != 'D' && segment != 'B')
		return ('S');
	return (segment);
}

/*
** Fetch letter corresponding to symbol type
*/

char		get_symbol_type(t_symbol *list)
{
	uint8_t	n_type;
	uint8_t n_sect;

	n_type = list->s_info->n_type;
	n_sect = list->s_info->n_sect;
	if ((n_type & N_SECT) == N_SECT)
	{
		if (n_sect == NO_SECT)
			return ('X');
		return ('S');
	}
	else if ((n_type & N_UNDF) == N_UNDF)
		return ('U');
	else if ((n_type & N_ABS) == N_ABS)
		return ('A');
	else if ((n_type & N_PBUD) == N_PBUD)
		return ('U');
	else if ((n_type & N_INDR) == N_INDR)
		return ('I');
	return ('X');
}

/*
** Display letter corresponding to symbol type
*/

static int	display_type(t_symbol *list, t_data *data, char symbol_type)
{
	char	external;

	external = 0;
	if (symbol_type == 'S')
	{
		symbol_type = get_sector_type(data, list);
		if (symbol_type == _SCTR_NOT_FOUND)
			return (free_all(list, data, _SCTR_NOT_FOUND, list->name));
	}
	if ((list->s_info->n_type & N_EXT) == N_EXT)
		external = 1;
	if (!external)
		symbol_type += 32;
	ft_putchar(symbol_type);
	ft_putchar(' ');
	return (_DISPLAY_OK);
}

/*
** Go through symbol list and display details
*/

int			display(t_symbol *list, t_data *data)
{
	int		res;
	char	symbol_type;

	while (list)
	{
		symbol_type = get_symbol_type(list);
		if (symbol_type != 1)
		{
			display_value(list->s_info, data->filetype);
			res = display_type(list, data, symbol_type);
			if (res != _DISPLAY_OK)
				return (res);
			ft_putendl(list->name);
		}
		list = list->next;
	}
	return (_DISPLAY_OK);
}
