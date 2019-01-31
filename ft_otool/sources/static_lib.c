/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/01/31 13:27:03 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

#define TO_SYMTAB 88
#define TO_FILENAME 60
#define TO_BINARY 20

static void	display_archive_list(t_archive *list, char *stringtab, char *file,
									char *ptr)
{
	while (list)
	{
		ft_putstr(file);
		ft_putchar('(');
		ft_putstr(stringtab + list->str_off + 1);
		ft_putendl(".o):");
		ft_putstr("reading object at offset ");
		ft_putnbr(list->obj_off + 80);
		ft_putchar('\n');
		magic_reader((void*)ptr + list->obj_off + 80, file, 1);
		list = list->next;
	}
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

	// ft_putendl("||");
	// ft_putnbr(ranlib->ran_un.ran_strx);
	// ft_putchar('\n');
	// ft_putnbr(ranlib->ran_off);
	// ft_putchar('\n');

	// stringtab = ptr + TO_SYMTAB + 4 + symtab_size + 4 + ranlib->ran_un.ran_strx;
	// ft_putendl(stringtab);
	
	// cursor += 20;
	// magic_reader(cursor + TO_BINARY, cursor, 1);
	return (0);
}