/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2019/03/15 15:40:50 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Create array (one entry per arch in fat binary) telling if the arch is
** supposed to be displayed or not
*/

static int	*arch_selection(t_meta *file, int archnb, int *rvalue)
{
	int				*res;
	int				highest;
	int				cputype;
	unsigned int	magicnb;
	int				i;

	res = (int *)malloc(sizeof(int) * archnb);
	highest = 0;
	magicnb = -1;
	i = -1;
	while (++i < archnb)
	{
		if ((*rvalue = arch_structures(file, &cputype, i, &magicnb)))
			break ;
		res[i] = (magicnb == MH_STATIC_LIB ? archive_priority() :
					determine_priority(magicnb, &res, &highest, archnb));
		if (cputype == CPU_TYPE_POWERPC)
			res[i] = _HIDE;
	}
	return (res);
}

/*
** Manages fat binary display
*/

static int	fat_boi(t_meta *f, int rvalue, uint32_t i)
{
	struct fat_header	*h;
	struct fat_arch		*arch;
	int					*display;

	h = (struct fat_header*)f->ptr;
	arch = (struct fat_arch*)(f->ptr + sizeof(*h));
	display = arch_selection(f, h->nfat_arch, &rvalue);
	while (++i < h->nfat_arch && rvalue == _EXIT_SUCCESS)
	{
		if (display[i])
		{
			if ((rvalue = check_bounds(f, sizeof(*h) + sizeof(*arch) * i)))
				break ;
			arch = (struct fat_arch*)(f->ptr + sizeof(*h) + sizeof(*arch) * i);
			show_name(arch->cputype, f->name, display[i]);
			if ((rvalue = check_bounds(f, arch->offset)))
				break ;
			rvalue = magic_reader(
				new_master(f->name, f->ptr + arch->offset, arch->size), 1);
		}
	}
	free(display);
	munmap(f->ptr, sizeof(f->ptr));
	return (rvalue);
}

/*
** Read magic_number et start appropriate function
*/

int			magic_reader(t_meta *master, char fat)
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
		rvalue = bin32(master, swap, fat);
	else if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
		rvalue = bin64(master, swap, fat);
	else if (magicnb == FAT_MAGIC)
		rvalue = fat_boi(master, _EXIT_SUCCESS, -1);
	else if (magicnb == MH_STATIC_LIB)
		rvalue = static_lib(master);
	if (!fat)
		munmap(master->ptr, sizeof(master->ptr));
	if (rvalue == _EXIT_SUCCESS)
		return (_EXIT_SUCCESS);
	if (!fat)
		return (error(rvalue == -2 ? _BAD_FMT : rvalue, NULL));
	return (rvalue == -2 ? _BAD_FMT : rvalue);
}

/*
** Open, map and display one file's symbols
*/

static int	treat_file(char *file)
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
		master_file->ptr = fat_swap(ptr);
	return (magic_reader(master_file, 0));
}

int			main(int argc, char **argv)
{
	int i;
	int	rvalue;

	i = 0;
	rvalue = -1;
	while (argc - i > 1)
	{
		rvalue = treat_file(ft_strdup(argv[i + 1]));
		i++;
	}
	if (argc == 1)
		return (treat_file("a.out"));
	return (rvalue ? 1 : _EXIT_SUCCESS);
}
