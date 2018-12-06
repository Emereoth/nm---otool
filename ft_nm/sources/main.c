/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2018/12/06 17:11:24 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/ft_nm.h"

static int	fat_boi(char *ptr, char *file, int nb_args, int swap)
{
	struct fat_header	*h;
	struct fat_arch		*arch;
	unsigned int		rvalue;
	unsigned int		i;

	(void)swap;
	i = 0;
	rvalue = -2;
	h = (struct fat_header*)ptr;
	printf("fat boi address: %p\n", ptr);
	while (i < h->nfat_arch)
	{
		arch = (struct fat_arch*)(ptr + sizeof(h) + sizeof(struct fat_arch) * i);
		printf("arch struct address: %p\n", arch);
		printf("magic_reader pointer address: %p (%d)\n", ptr + arch->offset, arch->offset);
		rvalue = magic_reader(ptr + arch->offset, file, nb_args, 1);
		i++;
		if (rvalue != _EXIT_SUCCESS)
			break;
	}
	munmap(ptr, sizeof(ptr));
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

/*
** Read magic_number et start appropriate function
*/

int			magic_reader(char *ptr, char *file, int nb_args, char fat)
{
	unsigned int	magicnb;
	unsigned int	rvalue;
	int				swap;

	if (!ptr)
		return (_EXIT_FAILURE);
	rvalue = -2;
	magicnb = *(unsigned int *)ptr;
	swap = 0;
	printf("magicnb = %#x (%p)\n", magicnb, ptr);
	if (magicnb == FAT_CIGAM || magicnb == MH_CIGAM || magicnb == MH_CIGAM_64)
	{
		printf("C I G A M\n");
		swap = 1;
	}
	if (magicnb == MH_MAGIC | magicnb == MH_CIGAM)
		rvalue = bin32(ptr, file, nb_args, swap);
	else if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
		rvalue = bin64(ptr, file, nb_args, swap);
	else if (magicnb == FAT_MAGIC)
		rvalue = fat_boi(ptr, file, nb_args, swap);
	if (!fat)
		munmap(ptr, sizeof(ptr));
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
	return (magic_reader(ptr, file, nb_args, 0));
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
