/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_init_shell_info.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiyunlee <jiyunlee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 03:07:19 by jihykim2          #+#    #+#             */
/*   Updated: 2023/09/25 22:07:14 by jiyunlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	_check_here_doc(t_shell_info *parse);
static int	_change_shell_info(t_shell_info *parse);
static void	_change_here_doc_to_infile(t_redir *redir, t_env_info *env, int *filenum, int mode);
static void	_get_here_doc_file(t_env_info *env, char *filename, char *limiter);

int	re_init_shell_info(t_shell_info *parse)
{
	pid_t		pid;
	int			status;
	int			exit_code_origin;

	set_signal(IGNORE, IGNORE);
	exit_code_origin = g_exit_code;		// exit이 들어온 경우에 대한 처리...
	g_exit_code = 0;
	if (parse->heredoc_cnt != 0)
	{
		pid = fork();
		if (pid < 0)
			exit (EXIT_FAILURE);
		else if (pid == 0)
			_check_here_doc(parse);
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) == EXIT_FAILURE)
			g_exit_code = EXIT_FAILURE;
	}
	_change_shell_info(parse);
	if (g_exit_code == EXIT_FAILURE)
		return (EXIT_FAILURE);
	g_exit_code = exit_code_origin;		// exit이 들어온 경우에 대한 처리...
	return (EXIT_SUCCESS);
}

static void	_check_here_doc(t_shell_info *parse)
{
	t_cmd_info	*node;
	int			filenum;

	set_signal(HRD_CHILD, HRD_CHILD);
	filenum = 0;
	node = parse->cmd;
	while (node)
	{
		if (node->redir != NULL)
			_change_here_doc_to_infile(node->redir, parse->env, &filenum, TRUE);
		node = node->next;
	}
	exit (EXIT_SUCCESS);
}

static int	_change_shell_info(t_shell_info *parse)
{
	t_cmd_info	*node;
	int			idx;
	int			len;
	int			filenum;

	node = parse->cmd;
	filenum = 0;
	while (node)
	{
		_change_here_doc_to_infile(node->redir, parse->env, &filenum, FALSE);
		idx = 0;
		while (node->cmd_args[idx])
		{
			len = ft_strlen(node->cmd_args[idx]);
			node->cmd_args[idx] = remove_quotation(node->cmd_args[idx], len);
			idx++;
		}
		node = node->next;
	}
	return (EXIT_SUCCESS);
}

static void	_change_here_doc_to_infile(t_redir *redir, t_env_info *env, int *filenum, int mode)
{
	char	*file;
	char	*_num;

	while (redir)
	{
		if (redir->type == LEFT_2)
		{
			_num = ft_itoa((*filenum)++);
			file = ft_strjoin(HERE_DOC, _num);
			if (file == NULL)
				exit (EXIT_FAILURE);		// 이전까지 생성한 file unlink해줘야 할수도..?(시그널인가)
			if (mode == TRUE)
				_get_here_doc_file(env, file, redir->filename);
			free (redir->filename);
			free (_num);
			redir->filename = file;
		}
		redir = redir->next;
	}
}

static void	_get_here_doc_file(t_env_info *env, char *filename, char *limiter)
{
	char	*line;
	int		fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_file_open("heredoc");
	while (TRUE)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, limiter) == 0)
			break ;
		// line = line_replace_env(env, line);		// char *line_replace_enc(char * str);
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	if (line != NULL)
		free(line);
	close(fd);
}
