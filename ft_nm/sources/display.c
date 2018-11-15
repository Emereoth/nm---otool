/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 17:01:06 by acottier          #+#    #+#             */
/*   Updated: 2018/11/15 14:58:46 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include <stdio.h>

/*
** Display symbol value
*/

static void	display_value(uint64_t n_value)
{
	char	*value;
	int		padding;

	if (n_value > 0)
		ft_putstr("0000000");
	else
		ft_putstr("       ");
	value = ft_to_hex(n_value);
	ft_putstr(value);
	padding = 10 - ft_strlen(value);
	while (padding-- > 0)
		ft_putchar(' ');
}

/*
** Use appropriate function to filetype
*/

static char	get_sector_type(t_data *data, t_symbol *list)
{
	static char	(*f[1]) (t_data *data, t_symbol *list, struct load_command *lc);

	f[1] = &browse_sector_bin64;
	return (f[data->filetype](data, list, data->lc));
}

/*
** Fetch letter corresponding to symbol type
*/

char		get_symbol_type(uint8_t n_type, uint8_t n_sect)
{
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

static int	display_type(t_symbol *list, t_data *data)
{
	char	external;
	char	symbol_type;

	external = 0;
	symbol_type = get_symbol_type(list->s_info->n_type, list->s_info->n_sect);
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

	while (list)
	{
		display_value(list->s_info->n_value);
		res = display_type(list, data);
		if (res != _DISPLAY_OK)
			return (res);
		ft_putendl(list->name);
		list = list->next;
	}
	return (_DISPLAY_OK);
}
