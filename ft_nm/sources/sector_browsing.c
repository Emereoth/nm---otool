/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_browsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 15:26:55 by acottier          #+#    #+#             */
/*   Updated: 2019/04/06 15:30:34 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Go through segments & sectors for symbol type
*/

char		browse_sector_bin64(t_data *data, uint8_t n_sect,
								struct load_command *lc, int swap)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	unsigned int				i;
	unsigned int				cur_sec;

	i = 0;
	cur_sec = 0;
	(void)swap; // TODO
	while (i++ < data->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			sect = (struct section_64 *)(seg + 1);
			if (n_sect - cur_sec > seg->nsects)
				cur_sec += seg->nsects;
			else
			{
				while (++cur_sec < n_sect)
					sect++;
				return (sect->sectname[2] - 32);
			}
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (_SCTR_NOT_FOUND);
}

static void	swap_segments_32(struct load_command *lc, unsigned int n)
{
	unsigned int			i;
	struct segment_command	*seg;

	i = 0;
	while (i++ < n && lc->cmd == LC_SEGMENT)
	{
		seg = (struct segment_command*)lc;
		endian_swap((void*)seg + 24, sizeof(struct segment_command) - 24);
		lc = (void*)lc + lc->cmdsize;
	}
}

char		browse_sector_bin32(t_data *data, uint8_t n_sect,
								struct load_command *lc, int swap)
{
	struct segment_command		*seg;
	struct section				*sect;
	unsigned int				i;
	unsigned int				cur_sec;

	i = 0;
	cur_sec = 0;
	if (swap)
		swap_segments_32(lc, data->ncmds);
	while (i++ < data->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (struct segment_command *)lc;
			sect = (struct section *)(seg + 1);
			if (n_sect - cur_sec > seg->nsects)
				cur_sec += seg->nsects;
			else
			{
				while (++cur_sec < n_sect)
					sect++;
				return (sect->sectname[2] - 32);
			}
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (_SCTR_NOT_FOUND);
}
