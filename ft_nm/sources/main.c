/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2019/02/15 15:31:09 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Create array (one entry per arch in fat binary) telling if the arch is
** supposed to be displayed or not
*/

static int	*arch_selection(t_meta *file, int archnb, int i, int *rvalue)
{
	int				*res;
	int				highest;
	int				bin32;
	int				cputype;
	unsigned int	magicnb;

	res = (int *)malloc(sizeof(int) * (archnb + 1));
	highest = 0;
	bin32 = -1;
	magicnb = -1;
	while (++i <= archnb)
	{
		if ((*rvalue = arch_structures(file, &cputype, i, &magicnb)))
			break ;
		res[i] = (magicnb == MH_STATIC_LIB ? archive_priority() :
			determine_priority(magicnb, &res, &bin32, i));
		if (cputype == CPU_TYPE_POWERPC)
		{
			res[i] = _HIDE;
			bin32 = -1;
		}
		res[0] += res[i];
	}
	return (res);
}

/*
** Manages fat binary display
*/

static int	fat_boi(t_meta *f, int nb_args, int rvalue, uint32_t i)
{
	struct fat_header	*h;
	struct fat_arch		*arch;
	int					*display;

	h = (struct fat_header*)f->ptr;
	display = arch_selection(f, h->nfat_arch, 0, &rvalue);
	while (++i < h->nfat_arch && rvalue == _EXIT_SUCCESS)
	{
		if (i + 1 <= h->nfat_arch + 1 && display[i + 1])
		{
			if ((rvalue = check_bounds(f, sizeof(h) + sizeof(arch) * i)))
				break ;
			arch = (struct fat_arch*)(f->ptr + sizeof(h) + sizeof(arch) * i);
			show_arch(display[i], arch->cputype, f->name);
			if ((rvalue = check_bounds(f, arch->offset)))
				break ;
			rvalue = magic_reader(new_master(
				f->name, f->ptr + arch->offset, arch->size), nb_args, 1);
			if (i > 1 && display[i + 2])
				ft_putchar('\n');
		}
	}
	free(display);
	munmap(f->ptr, sizeof(f->ptr));
	return (rvalue != _EXIT_SUCCESS ? error(rvalue, NULL) : _EXIT_SUCCESS);
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
		return (_BAD_FMT);
	rvalue = -2;
	magicnb = *(unsigned int *)master->ptr;
	swap = ((magicnb == MH_CIGAM || magicnb == MH_CIGAM_64) ? 1 : 0);
	if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
		rvalue = bin32(master, nb_args, swap);
	else if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
		rvalue = bin64(master, nb_args, swap);
	else if (magicnb == FAT_MAGIC)
		rvalue = fat_boi(master, nb_args, _EXIT_SUCCESS, -1);
	else if (magicnb == MH_STATIC_LIB)
		rvalue = static_lib(master, nb_args);
	if (!fat)
		munmap(master->ptr, sizeof(master->ptr));
	free(master);
	if (rvalue == _EXIT_SUCCESS)
		return (_EXIT_SUCCESS);
	return (error(rvalue == -2 ? _BAD_FMT : rvalue, NULL));
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
