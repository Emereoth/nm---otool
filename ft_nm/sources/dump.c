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

/*static void						show_bin(char *cursor)
{
	int				sign;
	static char		table[2] = "01";
	unsigned char	nb;

	sign = *(cursor) >= 0 ? 1 : -1;
	nb = *cursor;
	ft_putnbr(nb);
	ft_putchar('\n');
	ft_putchar(table[nb / 2^7]);
	ft_putchar('.');
	ft_putnbr(nb / 2^7);
	nb = nb & 0x80;
	ft_putchar(table[nb / 2^6]);
	ft_putchar('.');
	ft_putnbr(nb / 2^6);
	nb = nb & 0x40;
	ft_putchar(table[nb / 2^5]);
	ft_putchar('.');
	ft_putnbr(nb / 2^5);
	nb = nb & 0x20;
	ft_putchar(table[nb / 2^4]);
	ft_putchar('.');
	ft_putnbr(nb / 2^4);
	nb = nb & 0x10;
	ft_putchar(table[nb / 2^3]);
	ft_putchar('.');
	ft_putnbr(nb / 2^3);
	nb = nb & 0x08;
	ft_putchar(table[nb / 2^2]);
	ft_putchar('.');
	ft_putnbr(nb / 2^2);
	nb = nb & 0x04;
	ft_putchar(table[nb / 2]);
	ft_putchar('.');
	ft_putnbr(nb / 2);
	nb = nb & 0x02;
	ft_putchar(table[nb]);
	ft_putchar('.');
	ft_putnbr(nb);
	nb = nb & 0x01;
}*/

static void						show_bin(char *cursor)
{
	char	c;
	char	todisplay[8];

	c = *cursor;
	todisplay[0] = (c & 0x80) ? '1' : '0';
	todisplay[1] = (c & 0x40) ? '1' : '0';
	todisplay[2] = (c & 0x20) ? '1' : '0';
	todisplay[3] = (c & 0x10) ? '1' : '0';
	todisplay[4] = (c & 0x08) ? '1' : '0';
	todisplay[5] = (c & 0x04) ? '1' : '0';
	todisplay[6] = (c & 0x02) ? '1' : '0';
	todisplay[7] = (c & 0x01) ? '1' : '0';
	write(1, todisplay, 8);
}

static char		*show_address(char *address)
{
	return (ft_to_hex((uint64_t)address));
}

void			dump_bin(char *ptr, u_long length, u_long offset)
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
			ft_putchar('\n');
			ft_putnbr(size);
			if (offset != 0)
			{
				ft_putstr("\t(");
				ft_putnbr(size + offset);
				ft_putstr(")");
			}
			ft_putstr("\t");
			ft_putstr(show_address(ptr + size));
			ft_putchar('\t');
		}
		show_bin(ptr + size);
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

void			dump(char *ptr, u_long length, u_long offset)
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
			ft_putchar('\n');
			ft_putnbr(size);
			if (offset != 0)
			{
				ft_putstr("\t(");
				ft_putnbr(size + offset);
				ft_putstr(")");
			}
			ft_putstr("\t");
			ft_putstr(show_address(ptr + size));
			ft_putchar('\t');
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