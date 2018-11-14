/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_browsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 15:26:55 by acottier          #+#    #+#             */
/*   Updated: 2018/11/14 14:36:53 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

char		browse_sector_bin64(t_data *data, t_symbol *list, struct load_command *lc)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	unsigned int				i;
	unsigned int				cur_sec;

	i = 0;
	cur_sec = 0;
	lc = (void *)lc + lc->cmdsize;
	while (i < data->ncmds)
	{
		ft_putstr("index @ start: ");
		ft_putnbr(i);
		ft_putchar('\n');
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			sect = (struct section_64 *)(seg + 1);
			ft_putstr("Segment: ");
			ft_putendl(seg->segname);
			ft_putstr("Section: ");
			ft_putstr(sect->sectname);
			ft_putstr(" (");
			ft_putstr(seg->segname);
			ft_putendl(")");
			ft_putstr("\ttarget (");
			ft_putnbr(list->s_info->n_sect);
			ft_putstr("), current pos (");
			ft_putnbr(cur_sec);
			ft_putstr("), sect in cur seg (");
			ft_putnbr(seg->nsects);
			ft_putendl(")");
			if (list->s_info->n_sect - cur_sec < seg->nsects)
				cur_sec += seg->nsects;
			else
			{
				sect += (list->s_info->n_sect - cur_sec);
				ft_putendl(sect->segname);
				return (sect->segname[2]);
			}
			ft_putendl("not in current seg");
		}
		ft_putstr("index @ end: ");
		ft_putnbr(i);
		ft_putchar('\n');
		i++;
		lc = (void *)lc + lc->cmdsize;
	}
	return (_SCTR_NOT_FOUND);
}