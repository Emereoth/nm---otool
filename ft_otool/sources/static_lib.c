/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/02/13 17:23:30 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

#define TO_SYMTAB 88

/*
** Returns the size (in bytes) of the object's name in the header line
*/

static int	get_name_size(char *ptr)
{
	int		length;
	char	*cursor;
	int		res;

	length = 0;
	res = 0;
	cursor = ptr + 3;
	while (*cursor != 32)
	{
		length++;
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
** Calls magic_reader() on all nodes in archive object list
*/

static void	display_archive_list(t_archive *list, char *file,
									char *ptr)
{
	char	*obj_ptr;

	while (list)
	{
		obj_ptr = ptr + list->obj_off;
		ft_putstr(file);
		ft_putchar('(');
		ft_putstr(obj_ptr + HEADER_SIZE);
		ft_putendl("):");
		magic_reader(new_master(file, obj_ptr + 60 + get_name_size(obj_ptr),
			(u_long)obj_ptr + 48), 1);
		list = list->next;
	}
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

int			static_lib(char *ptr, char *file)
{
	struct ranlib	*symtab;
	int				symtab_size;
	char			*cursor;
	t_archive		*list;

	ft_putstr("Archive : ");
	ft_putendl(file);
	cursor = ptr + TO_SYMTAB;
	symtab_size = *(int *)cursor;
	cursor += 4;
	symtab = (struct ranlib *)cursor;
	list = mk_archive_list(symtab, symtab_size);
	display_archive_list(list, file, ptr);
	return (0);
}
