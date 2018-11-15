/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nique.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 17:46:46 by acottier          #+#    #+#             */
/*   Updated: 2018/11/15 17:13:45 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NIQUE_H
# define NIQUE_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdio.h>
# include <fcntl.h>

typedef unsigned long int	t_uli;

typedef struct	s_ctrl
{
	size_t			size;
	size_t			pos;
	unsigned char	zone;
	size_t			zone_size;
	struct s_ctrl	*prev;
	struct s_ctrl	*next;
	void			*zone_start;
}				t_ctrl;

typedef struct	s_data
{
	t_ctrl	*tiny;
	t_ctrl	*small;
	t_ctrl	*large;
}				t_data;

t_data		g_allocations;

enum
{
	CTRL = sizeof(struct s_ctrl),
	TINY = 271,
	SMALL = 2565,
	TINY_ZONE = (TINY + CTRL) * 104,
	SMALL_ZONE = (SMALL + CTRL) * 100 + 44,
};

/*
** MALLOC
*/

void			*ft_malloc(size_t size);
void			*find_alloc_point(size_t size, t_ctrl **alloc_list,
				int zone_type);
int				available_space(t_ctrl *cur, int size);
t_ctrl			*new_zone(t_ctrl *prev, size_t size, int zone_type);
void			*allocate(t_ctrl **alloc_point, size_t size,
				t_ctrl *next, t_ctrl *prev);

/*
** FREE
*/

void			ft_free(void *ptr);
t_ctrl			*find_memory(void *ptr, t_ctrl *alloc_list);

/*
** REALLOC
*/

void			*ft_realloc(void *ptr, size_t size);

/*
** SHOW_ALLOC_MEM.C
*/

void			show_alloc_mem();
void			to_hex(t_uli address);

/*
** ALLOCATION_DUMP.C
*/

void			allocation_dump();

/*
** UTILITIES.C
*/

void			ft_putchar(char c);
void			ft_putnbr(long long n);
void			ft_putstr(char *str);
void			ft_memcpy(void *dst, void *src, size_t n);
void			show_address(t_ctrl *address);

/*
** UTILITIES2.C
*/

t_ctrl			*align_target(t_ctrl *cursor);
void			show_readable(int fd, char *cursor, size_t limit);
int				zone_is_empty(t_ctrl *alloc);
int				only_zone(t_ctrl *alloc);
void			init();

/*
** UTILITIES3.C
*/

void			setup_standard_alloc(t_ctrl *alloc, t_ctrl *prev);
void			check_empty_zones(size_t zone_size);

#endif
