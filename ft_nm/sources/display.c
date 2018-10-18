/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 17:01:06 by acottier          #+#    #+#             */
/*   Updated: 2018/10/18 16:56:43 by acottier         ###   ########.fr       */
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

static void	display_type(t_symbol *list)
{
	char	external;
	char	symbol_type;

	external = 0;
	symbol_type = get_symbol_type(list->data->n_type, list->data->n_sect);
	if ((list->data->n_type & N_EXT) == N_EXT)
		external = 1;
	if (!external)
		symbol_type += 32;
	ft_putchar(symbol_type);
	ft_putchar(' ');
}

void		display(t_symbol *list)
{
	while (list)
	{
		display_value(list->data->n_value);
		display_type(list);
		ft_putendl(list->name);
		list = list->next;
	}
}