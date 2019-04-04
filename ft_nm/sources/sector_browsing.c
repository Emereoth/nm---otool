/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_browsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 15:26:55 by acottier          #+#    #+#             */
/*   Updated: 2019/04/04 16:53:26 by acottier         ###   ########.fr       */
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

static void	swap_seg_32(struct segment_command* seg)
{
	uint32_t		nsects;
	uint32_t		i;
	struct section	*sect;

	i = 0;
	ft_putendl("\n\n\nH E L L O\n\n\nT H E R E");
	dump((void*)seg, sizeof(struct segment_command), 0);
	endian_swap((void*)seg, 8);
	endian_swap((void*)seg + 32, sizeof(struct segment_command) - 32);
	dump((void*)seg, sizeof(struct segment_command), 0);
	nsects = seg->nsects;
	sect = (struct section*)(seg + 1);
	ft_putstr("Ayy we got ");
	ft_putnbr(nsects);
	ft_putendl(" nsects here");
	while (i < nsects)
	{
		ft_putstr("Section ");
		ft_putnbr(i + 1);
		ft_putstr(" of ");
		ft_putnbr(nsects);
		ft_putchar('\n');
		ft_putendl("Pre-swap:");
		dump((void*)sect, sizeof(struct section), 0);
		endian_swap((void*)sect + 32, sizeof(struct section) - 32);
		ft_putendl("Post-swap:");
		dump((void*)sect, sizeof(struct section), 0);
		sect++;
		i++;
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
	ft_putnbr(n_sect);
	ft_putendl(": n_sect");
	while (i++ < data->ncmds)
	{
		ft_putendl(ft_to_hex((uint64_t)data->ptr));
		if (lc->cmd == LC_SEGMENT)
		{
			ft_putendl("cmd is LC_SEGMENT");
			seg = (struct segment_command *)lc;
			ft_putendl(ft_to_hex((uint64_t)lc));
			dump((void*)lc, sizeof(struct load_command), 0);
			ft_putendl(ft_to_hex((uint64_t)seg));
			dump((void*)seg, sizeof(struct segment_command), 0);
			if (swap)
				swap_seg_32(seg);
			sect = (struct section *)(seg + 1);
			if (n_sect - cur_sec > seg->nsects)
			{
				cur_sec += seg->nsects;
				ft_putstr("not enough sections in segment (");
				ft_putnbr(seg->nsects);
				ft_putstr("), incrementing cur_sec (");
				ft_putnbr(cur_sec);
				ft_putendl(") and passing to next segment");
			}
			else
			{
				ft_putstr("Current segment contains ");
				ft_putnbr(seg->nsects);
				ft_putstr(" sections: ");
				ft_putendl(seg->segname);
				ft_putstr("incrementing cur_sec (");
				ft_putnbr(cur_sec);
				ft_putstr(") of ");
				ft_putnbr(n_sect - cur_sec);
				ft_putendl(" section sizes");
				while (++cur_sec < n_sect)
					sect++;
				// ft_putstr("file start ptr: ");
				// ft_putendl(ft_to_hex((uint64_t)data->ptr));
				// dump((void*)sect, sizeof(struct section), 0);
				// dump_bin((void*)sect, sizeof(struct section), 0);
				return (sect->sectname[2] - 32);
			}
		}
		ft_putstr("incrementing lc of ");
		ft_putnbr(lc->cmdsize);
		ft_putendl(" bytes");
		lc = (void *)lc + lc->cmdsize;
	}
	return (_SCTR_NOT_FOUND);
}
