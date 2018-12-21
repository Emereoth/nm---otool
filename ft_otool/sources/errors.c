/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acottier <acottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 11:13:32 by acottier          #+#    #+#             */
/*   Updated: 2018/12/21 10:07:17 by acottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_otool.h"

int	error(int errcode, char *file)
{
	static char	*msg[7] = {"Could not open file.", "Mmap call failure",
	"No symbol table found.", ": no such file or directory",
	"Unsupported format.", ": could not locate memory segment for symbol."};

	if (errno == 0)
	{
		ft_putendl_fd(msg[errcode - 1], 2);
		return (errcode);
	}
	ft_putstr_err(file);
	if (!file)
		return (errcode);
	if (errno == 2)
		ft_putendl_fd(": no such file or directory", 2);
	else if (errno == 13)
		ft_putendl_fd(": permission denied", 2);
	return (errcode);
}
