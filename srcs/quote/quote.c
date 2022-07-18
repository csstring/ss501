/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soo <soo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 18:13:38 by soo               #+#    #+#             */
/*   Updated: 2022/07/18 20:22:08 by soo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//!!널가드 추가

int	del_quote_cnt(char *origin, char c)
{
	int		o_idx;
	int		cnt;
	
	o_idx = 1;
	cnt = 0;
	
	if (origin[0] == '\0')
		return (0);
	while (origin[o_idx + 2])
	{
		if (origin[o_idx] == c && origin[o_idx + 1] == c)
		{
			o_idx += 2;
			cnt += 2;
		}
		else
			++o_idx;
	}
	return (cnt);
}

char	*del_quote(char *origin, char c)
{
	int		i;
	int		o_idx;
	int		cnt;
	char	*ret;

	i = 0;
	cnt = del_quote_cnt(origin, c);
	if (!cnt)
		return (NULL);
	ret = (char *)malloc(ft_strlen(origin) - cnt + 2);
	o_idx = 0;
	while (origin[o_idx + 2])
	{
		if (origin[o_idx] == c && origin[o_idx + 1] == c)
			o_idx += 2;
		else
			ret[i++] = origin[o_idx++];
	}
	if (origin[o_idx] != '\0')
		ret[i++] = origin[o_idx]; 
	ret[i++] = origin[ft_strlen(origin) - 1];
	ret[i] = '\0';
	return (ret);
}

char	*s_line_handler(char **line, char **origin)
{
	char *ret;

	ret = del_quote(*origin,'\'');
	// free(*line);
	if (!ret)
		return (*origin);
	free(*origin);
	*origin = ft_strdup(ret);
	free(ret);
	return (*origin);
}

char	*d_line_handler(t_env *env, char **line, char **origin, int exit_code)
{
	char	*ret;
	int		dollar;
	int		idx;

	dollar = cnt_c(*origin, '$');
	if (!dollar)
	{
		// free(*line);
		return (NULL);
	}
	idx = 0;
	while (dollar--)
	{
		substitution_env(env, origin, exit_code, &idx);
		// free(*origin);
		// *origin = ft_strdup(*line);
	}
	ret = del_quote(*origin, '\"');
	// free(*line);
	if (!ret)
		return (*origin);
	free(*origin);
	*origin = ft_strdup(ret);
	free(ret);
	return (*origin);
}

char	*no_quote_line(char **origin)
{
	char	*cmd_tmp;
	char	*newline;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (origin[0][i] && origin[0][i] != ' ')
		++i;
	if (origin[0][i] == '\0')
		return (NULL);
	++i;
	cmd_tmp = ft_strndup(*origin, i);
	newline = (char *)malloc(ft_strlen(&origin[0][i]) + 3);
	newline[0] = '\"';
	while (origin[0][i])
		newline[++j] = origin[0][i++];
	newline[++j] = '\"';
	newline[++j] = '\0';
	free(*origin);
	*origin = ft_strjoin(cmd_tmp, newline);
	free(cmd_tmp);
	free(newline);
	return (*origin);
}

char	*no_quote_line_handler(char **origin, int exit_code, t_env *env)
{
	// char	*ret;
	int		dollar;
	int		idx;

	dollar = cnt_c(*origin, '$');
	if (!dollar)
	{
		// free(*line);
		return (NULL);
	}
	idx = 0;
	while (dollar--)
	{
		substitution_env(env, origin, exit_code, &idx);
		// free(*origin);
		// *origin = ft_strdup(*line);
	}
	// ret = del_quote(*origin, '\"');
	// // free(*line);
	// if (!ret)
	// 	return (*origin);
	// free(*origin);
	// *origin = ft_strdup(ret);
	// free(ret);
	return (*origin);
}

char	*quote_line(char **origin, int exit_code, t_env *env)
{
	char	*ret;
	char	*s_line;
	char	*d_line;
	//char	**split_origin;
	
	// split_origin = (char **)malloc(sizeof(char *) * 3);
	// split_origin[2] = NULL;
	if ((find_first_c(*origin, '\"') == -1) && (find_first_c(*origin, '\'') == -1))
		no_quote_line(origin);
	// if (find_first_c(*origin, '\"') != -1)
	// {
		// d_line = split_dup_quote(*origin , split_origin, '\"');
		if (!d_line_handler(env, &d_line, origin, exit_code))
			return (*origin);
		// ret = (char *)malloc(ft_strlen(split_origin[0]) + ft_strlen(*origin) + ft_strlen(split_origin[1]) + 1);
		// arrange_str_cpy(ret,split_origin[0], *origin, split_origin[1]);
		// free(*origin);
		// free(split_origin[0]);
		// free(split_origin[1]);
		// *origin = ft_strdup(ret);
		// free(ret);
	// }
	//no_quote_line_handler(origin, exit_code, env);
	// if (find_first_c(*origin, '\'') != -1) // arrange안함
	// {
		// s_line = split_dup_quote(*origin , split_origin, '\'');
		s_line_handler(&s_line, origin);
		// free(split_origin[0]);
		// free(split_origin[1]);
	// }
	// free(split_origin);
	return (*origin);
}