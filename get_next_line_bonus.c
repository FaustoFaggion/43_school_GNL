/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fagiusep <fagiusep@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 10:17:40 by fagiusep          #+#    #+#             */
/*   Updated: 2021/09/08 19:05:33 by fagiusep         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*gnl_prep_backup(char **backup)
{
	char		*temp;
	char		*line;
	size_t		i;

	i = 0;
	while ((*backup)[i] != '\n' && (*backup)[i] != '\0')
		i++;
	if ((*backup)[i] == '\n')
		i++;
	temp = *backup;
	line = ft_substr(temp, 0, i);
	*backup = ft_strdup(&(*backup)[i]);
	free(temp);
	return (line);
}

int	gnl_read(int fd, char **backup, char **buff)
{
	int		r;
	char	*swap;

	r = 1;
	while (!ft_strchr(*backup, '\n') && r)
	{
		r = read(fd, *buff, BUFFER_SIZE);
		if (r < 0)
			return (r);
		(*buff)[r] = '\0';
		swap = *backup;
		*backup = ft_strjoin(swap, *buff);
		free(swap);
	}
	return (r);
}

static char	*gnl_prep_line(int fd, char **backup, char **buff)
{
	char	*temp;
	int		r;

	if (ft_strchr(*backup, '\n'))
		return (gnl_prep_backup(backup));
	r = gnl_read(fd, backup, buff);
	if (r <= 0 && !**backup)
	{
		free(*backup);
		*backup = NULL;
		return (NULL);
	}
	if (ft_strchr(*backup, '\n'))
		return (gnl_prep_backup(backup));
	if (!ft_strchr(*backup, '\n') && **backup)
	{
		temp = ft_strdup(*backup);
		free(*backup);
		*backup = NULL;
		return (temp);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*backup[OPEN_MAX];
	char		*line;
	char		*buff;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX)
		return (NULL);
	if (!backup[fd])
	{
		backup[fd] = ft_strdup("");
		if (!backup[fd])
			return (NULL);
	}
	buff = (char *)malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	line = gnl_prep_line(fd, &backup[fd], &buff);
	free(buff);
	buff = NULL;
	return (line);
}
