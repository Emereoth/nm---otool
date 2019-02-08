/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/02/08 11:48:53 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

#define TO_SYMTAB 88
#define TO_BINARY 20

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
		magic_reader(obj_ptr + 60 + get_name_size(obj_ptr),
			stringtab + list->str_off + 1, 1);
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
	display_archive_list(list, (void *)symtab + symtab_size + 4, file, ptr);
	return (0);
}