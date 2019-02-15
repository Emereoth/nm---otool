/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/02/15 16:08:08 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define TO_SYMTAB 88

/*
** Returns the size (in bytes) of the object's name in the header line
*/

static int	get_name_size(char *obj_ptr, t_meta *file, int *rvalue, int offset)
{
	int		length;
	char	*cursor;
	int		res;

	length = 0;
	res = 0;
	if ((*rvalue = check_bounds(file, offset + 3)))
		return (-1);
	cursor = obj_ptr + 3;
	while (*rvalue == _EXIT_SUCCESS && *cursor != 32)
	{
		length++;
		if ((*rvalue = check_bounds(file, offset + 3 + length)))
			return (-1);
		cursor++;
	}
	cursor -= length;
	while (length > 0)
	{
		res *= 10;
		res += (*cursor) - 48;
		length--;
		cursor++;
	}
	return (res);
}

/*
** Free object list from archive
*/

static void	free_archive_list(t_archive *list)
{
	t_archive	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

/*
** Calls magic_reader() on all nodes in archive object list
*/

static void	display_archive_list(t_archive *list, t_meta *f,
									int nb_args, int *rval)
{
	char		*obj_ptr;
	t_archive	*cursor;
	int			namesize;

	cursor = list;
	while (cursor)
	{
		ft_putchar('\n');
		if ((*rval = check_bounds(f, cursor->obj_off)) ||
			(*rval = check_bounds(f, cursor->obj_off + HEADER_SIZE)))
			break ;
		obj_ptr = cursor->start + cursor->obj_off;
		ft_putstr(f->name);
		ft_putchar('(');
		ft_putstr(obj_ptr + HEADER_SIZE);
		ft_putendl("):");
		namesize = get_name_size(obj_ptr, f, rval, cursor->obj_off);
		if (namesize == -1 || (*rval =
			check_object_bounds(f, cursor->obj_off + HEADER_SIZE, namesize)))
			break ;
		magic_reader(new_master(f->name, obj_ptr + HEADER_SIZE + namesize,
			ft_atoi((obj_ptr + 48))), nb_args, 1);
		cursor = cursor->next;
	}
	free_archive_list(list);
}

/*
** Checks for duplicates before adding new node
*/

int			check_duplicate_nodes(t_archive *list, int offset)
{
	while (list)
	{
		if (list->obj_off == offset)
			return (1);
		list = list->next;
	}
	return (0);
}

/*
** Creates and displays object list for archives
*/

int			static_lib(t_meta *file, int nb_args)
{
	struct ranlib	*symtab;
	int				symtab_size;
	char			*cursor;
	t_archive		*list;
	int				rvalue;

	rvalue = _EXIT_SUCCESS;
	if (check_bounds(file, TO_SYMTAB))
		return (_OUT_OF_BOUNDS);
	cursor = file->ptr + TO_SYMTAB;
	symtab_size = *(int *)cursor;
	if (check_bounds(file, TO_SYMTAB + 4))
		return (_OUT_OF_BOUNDS);
	cursor += 4;
	symtab = (struct ranlib *)cursor;
	list = mk_archive_list(symtab, symtab_size, file, &rvalue);
	if (rvalue)
	{
		free_archive_list(list);
		return (rvalue);
	}
	display_archive_list(list, file, nb_args, &rvalue);
	return (rvalue);
}
