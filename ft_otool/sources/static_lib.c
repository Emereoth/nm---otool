/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/02/07 14:28:45 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

#define TO_SYMTAB 88
#define TO_BINARY 20

static void	display_archive_list(t_archive *list, char *stringtab, char *file,
									char *ptr)
{
	while (list)
	{
		ft_putstr(file);
		ft_putchar('(');
		ft_putstr(ptr + list->obj_off + HEADER_SIZE);
		ft_putendl("):");
		ft_putstr("Total offset on target: ");
		ft_putnbr(list->obj_off + 80);
		ft_putchar('\n');
		magic_reader((void*)ptr + list->obj_off + 80, stringtab + list->str_off + 1, 1);
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