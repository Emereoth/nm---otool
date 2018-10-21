/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 17:01:06 by acottier          #+#    #+#             */
/*   Updated: 2018/10/21 15:51:45 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void	display_value(int n_value)
{
	char	*value;
	int		padding;

	value = ft_to_hex(n_value);
	ft_putstr(value);
	padding = 17 - ft_strlen(value);
	while (padding-- > 0)
		ft_putchar(' ');
}

static char	get_sector_type(t_data *data, t_symbol *list)
{
	char	(*f[1]) (t_data *data, t_symbol *list) = 
	{
		&browse_sector_bin64
	};

	return (f[data->filetype](data, list));
}

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