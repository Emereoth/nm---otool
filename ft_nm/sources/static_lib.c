/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/02/09 15:27:12 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define TO_SYMTAB 88

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

static void	display_archive_list(t_archive *list, char *stringtab, char *file,
									int nb_args)
{
	char	*obj_ptr;

	while (list)
	{
		ft_putchar('\n');
		obj_ptr = list->start + list->obj_off;
		ft_putstr(file);
		ft_putchar('(');
		ft_putstr(obj_ptr + HEADER_SIZE);
		ft_putendl("):");
		magic_reader(obj_ptr + 60 + get_name_size(obj_ptr),
			stringtab + list->str_off + 1, nb_args, 1);
		list = list->next;
	}
}

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

int			static_lib(char *ptr, char *file, int nb_args)
{
	struct ranlib	*symtab;
	int				symtab_size;
	char			*cursor;
	t_archive		*list;

	cursor = ptr + TO_SYMTAB;
	symtab_size = *(int *)cursor;
	cursor += 4;
	symtab = (struct ranlib *)cursor;
	list = mk_archive_list(symtab, symtab_size, ptr);
	display_archive_list(list,
		(void *)symtab + symtab_size + 4, file, nb_args);
	return (0);
}
