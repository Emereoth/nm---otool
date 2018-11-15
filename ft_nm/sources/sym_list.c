/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:46:16 by acottier          #+#    #+#             */
/*   Updated: 2018/11/15 11:49:18 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Return pointer to start of list
*/

static t_symbol	*list_start(t_symbol *list)
{
	while (list && list->prev)
		list = list->prev;
	return (list);
}

/*
** Add link to list
*/

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

/*
** Free and destroy list
*/

void			free_sym_list(t_symbol *list)
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

/*
** Create sym_list from symbol table contents
*/

t_symbol		*make_sym_list(char *stringtab, struct nlist_64 *el, int nsyms)
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
		new->name = ft_strdup(stringtab + el[i].n_un.n_strx);
		new->s_info = &el[i];
		new->prev = NULL;
		new->next = NULL;
		res = add_to_list(res, new);
		i++;
	}
	return (res);
}
