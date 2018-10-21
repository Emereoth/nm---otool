/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_browsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 15:26:55 by acottier          #+#    #+#             */
/*   Updated: 2018/10/21 15:49:17 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

char		browse_sector_bin64(t_data *data, t_symbol *list)
{
	struct segment_command_64	*seg;
	struct load_command			*lc_cursor;
	int							i;

	i = 0;
	lc_cursor = data->lc;
	while (i < data->ncmds)
	{
		if (i == list->s_info->n_sect)
		{
			seg = (struct segment_command_64 *)lc_cursor;
			return (seg->segname[2]);
		}
		lc_cursor = (void *)lc_cursor + lc_cursor->cmdsize;
		i++;
	}
	return (_SCTR_NOT_FOUND);
}