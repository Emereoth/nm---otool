/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2019/01/11 18:26:31 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/ft_otool.h"

/*
** Manages fat binary display
*/

static int	fat_boi(char *ptr, char *file)
{
	struct fat_header	*h;
	struct fat_arch		*arch;
	unsigned int		rvalue;
	unsigned int		i;

	i = 0;
	rvalue = -2;
	h = (struct fat_header*)ptr;
	while (i < h->nfat_arch)
	{
		arch = (struct fat_arch*)(ptr + sizeof(h) + sizeof(struct fat_arch) * i);
		if (arch->cputype != CPU_TYPE_POWERPC)
		{
			show_arch(arch->cputype, file);
			rvalue = magic_reader(ptr + arch->offset, file, 1);
			if (rvalue != _EXIT_SUCCESS)
				break;
		}
		i++;
	}
	munmap(ptr, sizeof(ptr));
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

/*
** Read magic_number et start appropriate function
*/

int			magic_reader(char *ptr, char *file, char fat)
{
	unsigned int	magicnb;
	int				rvalue;
	int				swap;

	if (!ptr)
		return (_EXIT_FAILURE);
	rvalue = -2;
	magicnb = *(unsigned int *)ptr;
	swap = ( (magicnb == MH_CIGAM || magicnb == MH_CIGAM_64) ? 1 : 0);
	if (magicnb == MH_MAGIC || magicnb == MH_CIGAM)
		rvalue = bin32(ptr, file, swap);
	else if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
		rvalue = bin64(ptr, file, swap);
	else if (magicnb == FAT_MAGIC)
		rvalue = fat_boi(ptr, file);
	if (!fat)
		munmap(ptr, sizeof(ptr));
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

/*
** Open, map and display one file's symbols
*/

static int	treat_file(char *file)
{
	int				fd;
	struct stat		buff;
	char			*ptr;
	unsigned int	magicnb;

	fd = 0;
	errno = 0;
	if ((fd = open(file, O_RDONLY)) < 0)
		return (error(_OPEN_FAILURE, file));
	if (fstat(fd, &buff) < 0)
		return (error(_FILE_NOT_FOUND, file));
	if ((ptr = mmap(NULL, buff.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
			fd, 0)) == MAP_FAILED)
		return (error(_MMAP_FAILURE, file));
	magicnb = *(unsigned int*)ptr;
	if (magicnb == FAT_CIGAM)
		ptr = fat_swap(ptr);
	return (magic_reader(ptr, file, 0));
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
	return (rvalue ? rvalue : _EXIT_SUCCESS);
}
