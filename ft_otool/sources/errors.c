/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 11:13:32 by acottier          #+#    #+#             */
/*   Updated: 2019/03/08 13:41:20 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

/*
** Shows error message
*/

int	error(int errcode, char *file)
{
	static char	*msg[10] = {"Could not open file.", "Mmap call failure",
	"No symbol table found.", ": no such file or directory",
	"Unsupported format.", ": could not locate memory segment for symbol.",
	"Corrupted string table.", "File size overflow.",
	"Couldn't find memory section"};

	ft_putendl_fd(msg[errcode - 1], 2);
	return (errcode);
	ft_putstr_err(file);
	if (!file)
		return (errcode);
	if (errno == 2)
		ft_putendl_fd(": no such file or directory", 2);
	else if (errno == 13)
		ft_putendl_fd(": permission denied", 2);
	return (errcode);
}
