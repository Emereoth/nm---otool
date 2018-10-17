/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 17:01:06 by acottier          #+#    #+#             */
/*   Updated: 2018/10/17 17:07:58 by acottier         ###   ########.fr       */
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
	(void)list;
	ft_putstr("X ");
}

void		display(t_symbol *list)
{
	while (list)
	{
		display_value(list->value);
		display_type(list);
		ft_putendl(list->name);
		list = list->next;
	}
}