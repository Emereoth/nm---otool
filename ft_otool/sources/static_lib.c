/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 13:58:52 by acottier          #+#    #+#             */
/*   Updated: 2019/01/24 18:00:14 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

#define TO_SYMTAB 88
#define TO_FILENAME 60
#define TO_BINARY 20

int		static_lib(char *ptr, char *file)
{
	char			*cursor;
	struct ranlib	*ranlib;
	int				symtab_s;

	ft_putstr("Archive : ");
	ft_putendl(file);
	cursor = ptr + TO_SYMTAB;
	symtab_s = *(int *)cursor;
	cursor += 4;
	ranlib = (struct ranlib *)ptr + TO_SYMTAB + 4; // symtab first entry
	cursor = ptr + ranlib->ran_off + TO_FILENAME;
	ft_putnbr(ranlib->ran_off);
	ft_putendl("toast");
	ft_putendl(cursor);
	ft_putendl("toast");
	ranlib = (struct ranlib *)ptr + TO_SYMTAB + 8;
	cursor = ptr + ranlib->ran_off + TO_FILENAME;
	ft_putnbr(ranlib->ran_off);
	ft_putendl(cursor);
	
	// cursor += 20;
	// magic_reader(cursor + TO_BINARY, cursor, 1);
	return (0);
}