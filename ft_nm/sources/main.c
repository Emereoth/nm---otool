/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2018/11/29 16:08:48 by acottier         ###   ########.fr       */
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
	ft_putendl("FAAAAAAAAAT");
	while (i < h->nfat_arch)
	{
		arch = (struct fat_arch*)ptr + sizeof(h) + sizeof(struct fat_arch) * i;
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
	if (magicnb == FAT_CIGAM || magicnb == MH_CIGAM || magicnb == MH_CIGAM_64)
		swap = 1;
	if (magicnb == MH_MAGIC | magicnb == MH_CIGAM)
		rvalue = bin32(ptr, file, nb_args, swap);
	else if (magicnb == MH_MAGIC_64 || magicnb == MH_CIGAM_64)
		rvalue = bin64(ptr, file, nb_args, swap);
	else if (magicnb == FAT_MAGIC || magicnb == FAT_CIGAM)
		rvalue = fat_boi(ptr, file, nb_args, swap);
	if (!fat)
		munmap(ptr, sizeof(ptr));
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

void	endian_swap(char *ptr, size_t size)
{
	size_t		i;
	int32_t		*bin;

	i = 0;
	size = size / 4;
	bin = (void *)ptr;
	while (i < size)
	{
		bin[i] = ((bin[i] & 0xff0000000) >> 24) |
					((bin[i] & 0x00ff0000) >> 8) |
					((bin[i] & 0x0000ff00) << 8) |
					(bin[i] << 24);
		i++;
	}
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
	if ((ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
	{
		munmap(ptr, sizeof(ptr));
		return (error(_MMAP_FAILURE, file));
	}
	magicnb = *(unsigned int*)ptr;
	if (magicnb == FAT_CIGAM || magicnb == MH_CIGAM || magicnb == MH_CIGAM_64)
		endian_swap(ptr, buff.st_size);
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
