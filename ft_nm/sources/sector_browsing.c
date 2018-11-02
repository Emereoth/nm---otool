/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_browsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 15:26:55 by acottier          #+#    #+#             */
/*   Updated: 2018/11/02 13:59:45 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

char		browse_sector_bin64(t_data *data, t_symbol *list, struct load_command *lc)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	unsigned int				i;
	unsigned int				cur_sec;
	struct load_command			*lc_start;
	unsigned int				j;

	i = 0;
	lc_start = lc;
	ft_putchar('\n');
	while (i < data->ncmds)
	{
		j = 0;
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			sect = (struct section_64 *)(seg + 1);
			ft_putstr("SEGMENT : ");
			ft_putstr(seg->segname);
			ft_putstr(" (");
			ft_putnbr(seg->nsects);
			ft_putendl(" sections)");
			while (j++ < seg->nsects)
			{
				ft_putstr("\tSECTION ");
				ft_putnbr(j);
				ft_putstr(" (segment name confirmation: ");
				ft_putstr(sect->segname);
				ft_putstr(") : ");
				ft_putendl(sect->sectname);
				sect = (struct section_64 *)(seg + j);
			}
		}
		lc = (void *)lc + lc->cmdsize;
		// i++;
	}
	i = 0;
	cur_sec = 1;
	lc = lc_start;
	while (i < data->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			if (seg->nsects >= cur_sec + list->s_info->n_sect)
			{
				sect = (struct section_64 *)
						(seg + 1 +list->s_info->n_sect - cur_sec);
				return (sect->sectname[2]);
			}
			cur_sec += seg->nsects;
		}
		i++;
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