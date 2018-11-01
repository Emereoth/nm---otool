/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_browsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 15:26:55 by acottier          #+#    #+#             */
/*   Updated: 2018/11/01 15:11:38 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

char		browse_sector_bin64(t_data *data, t_symbol *list, struct load_command *lc)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	unsigned int				i;

	i = 0;
	while (i < data->ncmds)
	{
		ft_putstr("\n SECTION: ");
		ft_putnbr(i);
		ft_putchar('\n');
		if (lc->cmd == LC_SEGMENT_64)
		{
			// ft_putstr("SEG64\n");
			seg = (struct segment_command_64 *)lc;
			// ft_putstr("target section: ");
			// ft_putnbr(list->s_info->n_sect);
			// ft_putstr("\nnumber of sections in current segment: ");
			// ft_putnbr(seg->nsects);
			if (seg->nsects >= i + list->s_info->n_sect)
			{
				// ft_putstr("\nYES");
				sect = (struct section_64 *)(seg + list->s_info->n_sect - i);
				// ft_putchar('\'');
				// ft_putnbr(list->s_info->n_sect - i);
				// ft_putchar('\'');
				return (sect->sectname[2]);
			}
			// else
				// ft_putstr("\nNO");
			i += seg->nsects;
			// ft_putnbr(i);
		}
		else
			i++;
		ft_putchar('\n');
		lc = (void *)lc + lc->cmdsize;
	}
	return (_SCTR_NOT_FOUND);
	// lc_cursor = data->lc;
	// // ft_putnbr(data->ncmds);
	// 	// ft_putendl("START");
	// while (i != list->s_info->n_sect)
	// {
	// 	seg = (struct segment_command_64 *)lc_cursor;
	// 	sect = (struct section *)(seg + 1);
	// 	// ft_putnbr(i);
	// 	// ft_putstr(" section: ");
	// 	// ft_putendl(sect->sectname);
	// 	sect++;
	// 	// lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	// 	i++;
	// }
	// 	// ft_putendl("END");
	// i = 0;
	// while (i < data->ncmds)
	// {
	// 	if (i == list->s_info->n_sect)
	// 	{
	// 		seg = (struct segment_command_64 *)lc_cursor;
	// 		sect = (struct section *)(seg + 1);
	// 		ft_putnbr(i);
	// 		// ft_putstr("memory section found: '");
	// 		// ft_putstr(sect->sectname);
	// 		// ft_putchar('\'');
	// 		return (sect->sectname[2]);
	// 	}
	// 	// lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
	// 	i++;
	// }
}