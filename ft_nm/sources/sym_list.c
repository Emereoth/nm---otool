/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:46:16 by acottier          #+#    #+#             */
/*   Updated: 2018/10/18 16:00:11 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static t_symbol	*list_start(t_symbol *list)
{
	while (list && list->prev)
		list = list->prev;
	return (list);
}

static t_symbol	*add_to_list(t_symbol *list, t_symbol *new)
{
	if (!list)
		return (new);
	while (list)
	{
		if (ft_strcmp(list->name, new->name) > 0)
		{
			new->next = list;
			new->prev = list->prev;
			if (list->prev)
				list->prev->next = new;
			list->prev = new;
			return (list_start(new));
		}
		if (!(list->next))
		{
			list->next = new;
			new->prev = list;
			return (list_start(new));
		}
		list = list->next;
	}
	return (NULL);
}

void		free_sym_list(t_symbol *list)
{
	t_symbol	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->name);
		free(list);
		list = tmp;
	}
}

t_symbol	*make_sym_list(char *stringtable, struct nlist_64 *el, int nsyms)
{
	t_symbol	*res;
	t_symbol	*new;
	int			i;

	i = 0;
	res = NULL;
	new = NULL;
	while (i < nsyms)
	{
		new = (t_symbol *)malloc(sizeof(t_symbol));
		new->name = ft_strdup(stringtable + el[i].n_un.n_strx);
		new->data = &el[i];
		new->prev = NULL;
		new->next = NULL;
		res = add_to_list(res, new);
		i++;
	}
	return (res);
}