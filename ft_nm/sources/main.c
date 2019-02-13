/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2019/02/13 16:29:05 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/ft_nm.h"

/*
** Create array (one entry per arch in fat binary) telling if the arch is
** supposed to be displayed or not
*/

static int	*arch_selection(char *ptr, int archnb, int i)
{
	int				*res;
	int				highest;
	int				bin32;
	struct fat_arch	*arch;
	unsigned int	magicnb;

	res = (int *)malloc(sizeof(int) * (archnb + 1));
	highest = 0;
	bin32 = -1;
	while (i <= archnb)
	{
		arch = (struct fat_arch *)(ptr + sizeof(struct fat_header)
				+ sizeof(struct fat_arch) * (i - 1));
		magicnb = *(unsigned int *)(ptr + arch->offset);
		res[i] = (magicnb == MH_STATIC_LIB ? archive_priority() :
					determine_priority(magicnb, &res, &bin32, i));
		if (arch->cputype == CPU_TYPE_POWERPC)
		{
			res[i] = _HIDE;
			bin32 = -1;
		}
		res[0] += res[i];
		i++;
	}
	return (res);
}

/*
** Manages fat binary display
*/

static int	fat_boi(char *ptr, char *file, int nb_args)
{
	struct fat_header	*h;
	struct fat_arch		*arch;
	unsigned int		rvalue;
	unsigned int		i;
	int					*display;

	i = -1;
	rvalue = EXIT_SUCCESS;
	h = (struct fat_header*)ptr;
	display = arch_selection(ptr, h->nfat_arch, 1);
	while (++i < h->nfat_arch && rvalue == EXIT_SUCCESS)
	{
		if (i + 1 <= h->nfat_arch + 1 && display[i + 1])
		{
			arch = (struct fat_arch*)(ptr + sizeof(h) + sizeof(&arch) * i);
			show_arch(display[i], arch->cputype, file);
			rvalue = magic_reader(
				new_master(file, ptr + arch->offset, arch->size), nb_args, 1);
			if (i > 1 && display[i + 2])
				ft_putchar('\n');
		}
	}
	free(display);
	munmap(ptr, sizeof(ptr));
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

/*
** Read magic_number et start appropriate function
*/

int			magic_reader(t_meta *master, int nb_args, char fat)
{
	unsigned int	magicnb;
	int				rvalue;
	int				swap;

	if (!(master->ptr))
		return (_EXIT_FAILURE);
	rvalue = -2;
	magicnb = *(unsigned int *)master->ptr;
	swap = ((magicnb == MH_CIGAM || magicnb == MH_CIGAM_64) ? 1 : 0);
	if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
		rvalue = bin32(master, nb_args, swap);
	else if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
		rvalue = bin64(master, nb_args, swap);
	else if (magicnb == FAT_MAGIC)
		rvalue = fat_boi(master->ptr, master->name, nb_args);
	else if (magicnb == MH_STATIC_LIB)
		rvalue = static_lib(master->ptr, master->name, nb_args);
	if (!fat)
		munmap(master->ptr, sizeof(master->ptr));
	free(master);
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

/*
** Open, map and display one file's symbols
*/

static int	treat_file(char *file, int nb_args)
{
	int				fd;
	struct stat		buff;
	char			*ptr;
	t_meta			*master_file;

	fd = 0;
	errno = 0;
	if ((fd = open(file, O_RDONLY)) < 0)
		return (error(_OPEN_FAILURE, file));
	if (fstat(fd, &buff) < 0)
		return (error(_FILE_NOT_FOUND, file));
	if ((ptr = mmap(NULL, buff.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
			fd, 0)) == MAP_FAILED)
		return (error(_MMAP_FAILURE, file));
	master_file = new_master(file, ptr, buff.st_size);
	if (*(unsigned int*)ptr == FAT_CIGAM)
		ptr = fat_swap(ptr);
	return (magic_reader(master_file, nb_args, 0));
}

int			main(int argc, char **argv)
{
	int i;
	int	rvalue;

	i = 0;
	rvalue = -1;
	while (argc - i > 1)
	{
		rvalue = treat_file(ft_strdup(argv[i + 1]), argc - 1);
		i++;
	}
	if (argc == 1)
		return (treat_file("a.out", 0));
	return (rvalue ? rvalue : _EXIT_SUCCESS);
}
