#include "../includes/ft_nm.h"

static void						show_hex(char *cursor)
{
	int				sign;
	static char		table[16] = "0123456789abcdef";
	unsigned char	nb;

	sign = *(cursor) >= 0 ? 1 : -1;
	nb = *cursor;
	ft_putchar(table[nb / 16]);
	ft_putchar(table[nb % 16]);
}

static char		*show_address(char *address)
{
	return (ft_to_hex((uint64_t)address));
}

void			dump(char *ptr, u_long length)
{
	char	c;
	u_long	size = 0;

	if (!ptr)
		return ;
	while (size < length)
	{
		if (size % 16 == 0)
		{
			if (size != 0)
			{
				for (int i = -16 ; i < 0 ; i++)
				{
					c = *(ptr + size + i);
					if (c > 31 && c < 127)
						ft_putchar(c);
					else
						ft_putchar('.');
				}
			}
			ft_putstr("\n");
			ft_putstr(show_address(ptr + size));
			ft_putstr("\t");
		}
		show_hex(ptr + size);
		ft_putchar(' ');
		size++;
	}
	if (size % 16 != 0)
	{
		for (int i = -(size % 16) ; i < 0 ; i++)
			ft_putstr(" ");
		for (int i = -(size % 16) ; i < 0 ; i++)
			{
				c = *(ptr + size + i);
				if (c > 31 && c < 127)
					ft_putchar(c);
				else
					ft_putchar('.');
			}
	}
	ft_putstr("\n\n");
}