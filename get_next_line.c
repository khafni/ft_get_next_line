/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khafni <khafni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:51:26 by khafni            #+#    #+#             */
/*   Updated: 2019/11/18 13:13:35 by khafni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** just a normal boring implementation of the
** original libc strdup function :/
*/

char	*ft_strdup(const char *s1)
{
	int		len;
	char	*str;

	if (!(s1))
		return (NULL);
	len = ft_strlen(s1);
	str = (char *)malloc(len + 1);
	if (str == NULL)
		return (NULL);
	str[len] = '\0';
	return ((char *)ft_memcpy(str, s1, len));
}

/*
** read the whole content from a fd to a static buffer
** by reading BUFFER_SIZE content every call of
** a the function  and every call  it keeps reading
** untill i it reads the whole file content
** (we allocate an empty string (\0) at first to
** the static buffer because we work by re adding
** what we already allocated and fillied to what
** we will allocate and fill next time
** we didn't fill till (we find a \n) \ (we call
** get_next_line))
*/

char	*read_line(int fd, char *static_buffer)
{
	int		k;
	char	*tmp;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	k = 0;
	if (!static_buffer)
		static_buffer = ft_strdup("");
	while (ft_strchr(static_buffer, '\n') == NULL)
	{
		k = read(fd, buffer, BUFFER_SIZE);
		buffer[k] = '\0';
		if (k == 0)
			break ;
		tmp = static_buffer;
		if (!(static_buffer = ft_strjoin(static_buffer, buffer)))
		{
			free(tmp);
			free(buffer);
			return (NULL);
		}
		free(tmp);
	}
	free(buffer);
	return (static_buffer);
}

/*
**	here's where the magic happens we check for verious error cases
**	then we get the whole fd file content and store it in a static
**	buffer using the read_line function .
** if there's no new line character in the buffer we will fill
** the line buffer with the whole static buffer.
**	if there's a new line in the buffer we fill the line buffer
**	from the start of its pointer address to the new line pointer
**	position then advance the static buffer pointer's adress to one
**	step ahead than the new line address and remplace the previous
**	static buffer by itself advanced to the next line
*/

int		get_next_line(int fd, char **line)
{
	static char	*univ_buffer[1024];
	char		*tmp;
	int			p;

	if (fd < 0 || !line || read(fd, NULL, 0) != 0 || BUFFER_SIZE < 0)
		return (-1);
	univ_buffer[fd] = read_line(fd, univ_buffer[fd]);
	p = 0;
	if (!ft_strchr(univ_buffer[fd], '\n'))
	{
		if (!(*line = ft_strdup(univ_buffer[fd])))
			return (-1);
		free(univ_buffer[fd]);
		univ_buffer[fd] = NULL;
		free(univ_buffer[fd]);
		return (0);
	}
	p = ft_strchr(univ_buffer[fd], '\n') - univ_buffer[fd];
	if (!(*line = ft_substr(univ_buffer[fd], 0, p)))
		return (-1);
	tmp = univ_buffer[fd];
	if (!(univ_buffer[fd] = ft_strdup(univ_buffer[fd] + p + 1)))
		return (-1);
	free(tmp);
	return (1);
}
/*
**	FREEING TRICK  /!\ :
**	a temporary buffer is used to deal with freeing garbage
**	memory areas
*/
