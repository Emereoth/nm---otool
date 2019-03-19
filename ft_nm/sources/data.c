/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 12:33:17 by acottier          #+#    #+#             */
/*   Updated: 2019/03/19 18:29:51 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Destroy and free data struct
*/

int			free_all(t_symbol *list, t_data *data, int errcode)
{
	free_sym_list(list);
	if (data)
		free(data);
	return (errcode);
}

/*
** Fills t_data struct with generic fields
*/

static void	prefill(t_data **data, char *ptr, unsigned int ncmds,
					struct load_command *lc)
{
	(*data)->ptr = ptr;
	(*data)->ncmds = ncmds;
	(*data)->lc = lc;
	(*data)->symtab = NULL;
}

/*
** Create and fill data structure for current file
*/

int			fill_data(char *ptr, t_data **data, t_meta *file)
{
	struct load_command		*lc_cursor;
	unsigned int			i;
	unsigned int			pos;

	i = 0;
	if (check_bounds(file, sizeof(struct mach_header_64)))
		return (_OUT_OF_BOUNDS);
	lc_cursor = (void *)ptr + sizeof(struct mach_header_64);
	prefill(data, ptr, ((struct mach_header_64 *)ptr)->ncmds,
		(void *)ptr + sizeof(struct mach_header_64));
	(*data)->filetype = _BIN64;
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		pos = (sizeof(struct mach_header_64) + i * sizeof(lc_cursor)->cmdsize);
		if (lc_cursor->cmd == LC_SYMTAB)
			(*data)->symtab = (struct symtab_command *)lc_cursor;
		i++;
		if (check_bounds(file, pos))
			return (_OUT_OF_BOUNDS);
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!(*data)->symtab)
		return (_NO_SYMTAB_FAILURE);
	return (_DATA_OK);
}

/*
** Create and fill data structure for 32-bit arch
*/

static void						show_hex(char *cursor)
{
	int				sign;
	static char		table[16] = "0123456789abcdef";
	unsigned char	nb;

	sign = *(cursor) >= 0 ? 1 : -1;
	nb = *cursor;
	ft_putchar(table[nb / 16]);
	ft_putchar(table[nb % 16]);
}

static char		*show_address(char *address)
{
	return (ft_to_hex((uint64_t)address));
}

static void	dump(char *ptr, u_long length)
{
	char	c;
	u_long	size = 0;

	while (size < length)
	{
		if (size % 16 == 0)
		{
			if (size != 0)
			{
				for (int i = -16 ; i < 0 ; i++)
				{
					c = *(ptr + size + i);
					if (c > 31 && c < 127)
						ft_putchar(c);
					else
						ft_putchar('.');
				}
			}
			ft_putstr("\n");
			ft_putstr(show_address(ptr + size));
			ft_putstr("\t");
		}
		show_hex(ptr + size);
		ft_putchar(' ');
		size++;
	}
	if (size % 16 != 0)
	{
		for (int i = -(size % 16) ; i < 0 ; i++)
			ft_putstr(" ");
		for (int i = -(size % 16) ; i < 0 ; i++)
			{
				c = *(ptr + size + i);
				if (c > 31 && c < 127)
					ft_putchar(c);
				else
					ft_putchar('.');
			}
	}
	ft_putstr("\n\n");
}

int			fill_data_32(char *ptr, t_data **data, t_meta *file)
{
	struct load_command		*lc_cursor;
	unsigned int			i;
	unsigned int			pos;

	i = 0;
	ft_putendl("ayyy lmao");
	if (check_bounds(file, sizeof(struct mach_header)))
		return (_OUT_OF_BOUNDS);
	ft_putendl("bounds ok");
	lc_cursor = (void *)ptr + sizeof(struct mach_header);
	dump(file->ptr, sizeof(struct mach_header));
	/*file->ptr = */endian_swap(ptr, sizeof(struct mach_header));
	dump(file->ptr, sizeof(struct mach_header));
	prefill(data, ptr, ((struct mach_header*)ptr)->ncmds,
		(void *)ptr + sizeof(struct mach_header));
	ft_putendl("prefill ok");
	(*data)->filetype = _BIN32;
	ft_putstr("ok boi we've got ");
	ft_putnbr((*data)->ncmds);
	ft_putendl(" ncmds");
	while (i < (*data)->ncmds && !(*data)->symtab)
	{
		pos = (sizeof(struct mach_header) + i * sizeof(lc_cursor)->cmdsize);
		if (lc_cursor->cmd == LC_SYMTAB)
			(*data)->symtab = (struct symtab_command *)lc_cursor;
		i++;
		if (check_bounds(file, pos))
			return (_OUT_OF_BOUNDS);
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	}
	if (!(*data)->symtab)
		return (_NO_SYMTAB_FAILURE);
	return (_DATA_OK);
}
