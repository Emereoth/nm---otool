/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:46:16 by acottier          #+#    #+#             */
/*   Updated: 2019/04/03 16:12:31 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Add link to list
*/

static t_symbol	*add_to_list(t_symbol *list, t_symbol *new)
{
	t_symbol	*start;

	start = list;
	while (list)
	{
		if (ft_strcmp(list->name, new->name) > 0)
		{
			new->next = list;
			new->prev = list->prev;
			if (list->prev)
				list->prev->next = new;
			list->prev = new;
			if (list == start)
				return (new);
			return (start);
		}
		if (!(list->next))
		{
			list->next = new;
			new->prev = list;
			return (start);
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
		free(list->s_info);
		free(list);
		list = tmp;
	}
}

/*
** Create t_info struct from nlist/nlist_64
*/

static t_info	*create_info_struct(t_nlist *el, char type, int i)
{
	t_info		*res;

	res = (t_info *)malloc(sizeof(t_info));
	if (type == _BIN32)
	{
		res->n_un = (el->list32)[i].n_un.n_strx;
		res->n_type = (el->list32)[i].n_type;
		res->n_sect = (el->list32)[i].n_sect;
		res->n_desc = (el->list32)[i].n_desc;
		res->n_value.val_32 = (el->list32)[i].n_value;
	}
	else if (type == _BIN64)
	{
		res->n_un = (el->list64)[i].n_un.n_strx;
		res->n_type = (el->list64)[i].n_type;
		res->n_sect = (el->list64)[i].n_sect;
		res->n_desc = (el->list64)[i].n_desc;
		res->n_value.val_64 = (el->list64)[i].n_value;
	}
	return (res);
}

/*
** Create new link to add to list
*/

static t_symbol	*create_link(char *stringtab, t_nlist *el, char type, int i)
{
	t_symbol		*res;

	if ((res = (t_symbol *)malloc(sizeof(t_symbol))) == NULL)
		return (NULL);
	ft_bzero(res, sizeof(t_symbol));
	res->s_info = create_info_struct(el, type, i);
	if (type == _BIN32)
		res->name = ft_strdup(stringtab + el->list32[i].n_un.n_strx);
	else if (type == _BIN64)
		res->name = ft_strdup(stringtab + el->list64[i].n_un.n_strx);
	res->type = type;
	return (res);
}

/*
** Create sym_list from symbol table contents
*/

t_symbol		*make_sym_list(char *stringtab, t_nlist *el, int nsyms,
								char type)
{
	t_symbol	*res;
	t_symbol	*new;
	int			i;
	uint8_t		n_type;

	i = 0;
	res = NULL;
	new = NULL;
	while (i < nsyms)
	{
		n_type = (type == _BIN32 ? el->list32[i].n_type : el->list64[i].n_type);
		if (!(n_type & N_STAB))
		{
			new = create_link(stringtab, el, type, i);
			if (!res)
				res = new;
			else
				res = add_to_list(res, new);
		}
		i++;
	}
	return (res);
}
