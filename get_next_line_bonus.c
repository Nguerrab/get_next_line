/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguerrab <nguerrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 16:00:11 by nguerrab          #+#    #+#             */
/*   Updated: 2026/11/15 14:52:24 by nguerrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static	size_t	ft_len(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	return (i);
}

static	char	*ft_fill(int fd, char *buf, char *str)
{
	char	*tmp;
	int		n;

	n = 1;
	while (n > 0 && (buf == NULL || ft_strchr(buf, '\n') == NULL))
	{
		n = read(fd, str, BUFFER_SIZE);
		if (n < 0)
		{
			free(buf);
			return (NULL);
		}
		str[n] = '\0';
		tmp = ft_strjoin(buf, str);
		free(buf);
		if (tmp == NULL)
			return (NULL);
		buf = tmp;
	}
	return (buf);
}

static	char	*ft_read(int fd, char *buf)
{
	char	*str;

	str = malloc((size_t)BUFFER_SIZE + 1);
	if (!str)
	{
		free(buf);
		return (NULL);
	}
	buf = ft_fill(fd, buf, str);
	free(str);
	return (buf);
}

static	char	*ft_extract(char **buf)
{
	char	*tmp;
	char	*line;
	int		n;

	n = ft_len(*buf);
	line = ft_substr(*buf, 0, n + 1);
	tmp = ft_substr(*buf, n + 1, ft_strlen(*buf));
	free(*buf);
	*buf = tmp;
	if (line == NULL || tmp == NULL)
	{
		free(line);
		free(*buf);
		*buf = NULL;
		return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buf[MAX_FD];

	if (BUFFER_SIZE <= 0 || fd >= MAX_FD || fd < 0)
		return (NULL);
	buf[fd] = ft_read(fd, buf[fd]);
	if (buf[fd] == NULL || buf[fd][0] == '\0')
	{
		free(buf[fd]);
		buf[fd] = NULL;
		return (NULL);
	}
	return (ft_extract(&buf[fd]));
}
