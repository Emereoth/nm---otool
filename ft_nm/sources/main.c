/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 10:44:04 by acottier          #+#    #+#             */
/*   Updated: 2018/11/15 11:50:47 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

/*
** Read magic_number et start appropriate function
*/

static int	magic_reader(char *ptr, char *file, int nb_args)
{
	unsigned int	magicnb;
	unsigned int	rvalue;

	if (!ptr)
		return (_EXIT_FAILURE);
	magicnb = *(int *)ptr;
	if (magicnb == MH_MAGIC_64)
		rvalue = bin64(ptr, file, nb_args);
	else
		rvalue = _EXIT_SUCCESS;
	munmap(ptr, sizeof(ptr));
	return (rvalue != _EXIT_SUCCESS ? error(_BAD_FMT, NULL) : _EXIT_SUCCESS);
}

/*
** Open, map and display one file's symbols
*/

static int	treat_file(char *file, int nb_args)
{
	int			fd;
	struct stat	buff;
	char		*ptr;

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
	return (magic_reader(ptr, file, nb_args));
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
