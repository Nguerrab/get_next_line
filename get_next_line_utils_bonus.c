/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguerrab <nguerrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 10:12:43 by nguerrab          #+#    #+#             */
/*   Updated: 2026/11/11 10:33:14 by nguerrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static	char	*help_sub(size_t st, char const *s, size_t len)
{
	size_t	i;
	size_t	over;
	char	*result;

	i = 0;
	over = ft_strlen(s) - st;
	if (len < over)
		over = len;
	result = malloc(over + 1);
	if (result == NULL)
		return (NULL);
	while (i < over)
	{
		result[i] = s[st + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	st;
	char	*result;

	st = (size_t)start;
	if (st > ft_strlen(s))
	{
		result = malloc(1);
		if (result != NULL)
			result[0] = '\0';
		return (result);
	}
	result = help_sub(st, s, len);
	return (result);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*result;

	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = malloc(len);
	if (result == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		result[i++] = s1[j++];
	j = 0;
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}

char	*ft_strchr(const char *s, int c)
{
	char	n;
	char	*p;
	int		i;

	i = 0;
	n = (char)c;
	p = (char *)s;
	while (s[i])
	{
		if (p[i] == n)
			return (&p[i]);
		i++;
	}
	if (n == '\0')
		return (&p[i]);
	return (NULL);
}
