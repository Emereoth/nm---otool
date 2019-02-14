/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/26 14:26:06 by acottier          #+#    #+#             */
/*   Updated: 2019/02/14 15:14:54 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define TO_SYMTAB 88

/*
** Returns pointer to last element of archive list
*/

static t_archive	*last_node(t_archive *list)
{
	while (list && list->next)
		list = list->next;
	return (list);
}

/*
** Links prev and next pointers between two given nodes
*/

static void			link_nodes(t_archive *new, t_archive *prev, t_archive *next)
{
	new->next = next;
	new->prev = prev;
	if (prev)
		prev->next = new;
	if (next)
		next->prev = new;
}

/*
** Adds new node to archive list
*/

static void			add_to_list(t_archive **start, t_archive *new)
{
	t_archive	*cursor;

	if (!(*start))
		*start = new;
	else
	{
		cursor = *start;
		while (cursor)
		{
			if (cursor->obj_off > new->obj_off)
			{
				link_nodes(new, cursor->prev, cursor);
				if (cursor == *start)
					*start = new;
				return ;
			}
			cursor = cursor->next;
		}
		link_nodes(new, last_node(*start), NULL);
	}
}

/*
** Creates new archive list node
*/

static t_archive	*new_node(int obj_off, int str_off, char *ptr)
{
	t_archive	*res;

	res = (t_archive *)malloc(sizeof(t_archive));
	res->obj_off = obj_off;
	res->str_off = str_off;
	res->start = ptr;
	res->prev = NULL;
	res->next = NULL;
	return (res);
}

/*
** Creates list containing all objects in archive
*/

t_archive			*mk_archive_list(struct ranlib *symtab, int symtab_size,
										t_meta *file, int *rval)
{
	char		*stringtab;
	t_archive	*res;
	u_long		pos;

	pos = TO_SYMTAB + 4 + 4 + symtab_size + 4;
	if ((*rval = check_bounds(file, pos)))
		return (NULL);
	stringtab = (void *)symtab + 4 + symtab_size + 4;
	res = NULL;
	while (symtab_size >= 8)
	{
		if (!(check_duplicate_nodes(res, symtab->ran_off)))
			add_to_list(&res, new_node(symtab->ran_off,
			symtab->ran_un.ran_strx, file->ptr));
		symtab_size -= 8;
		pos += sizeof(struct ranlib);
		if ((*rval = check_bounds(file, pos)))
			break ;
		symtab++;
	}
	return (res);
}
