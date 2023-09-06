/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihykim2 <jihykim2@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 21:57:23 by jihykim2          #+#    #+#             */
/*   Updated: 2023/09/06 18:08:25 by jihykim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libftprintf/libft/libft.h"
# include "../libftprintf/ft_printf/ft_printf.h"
# include "../libftprintf/get_next_line/get_next_line.h"

# define TRUE	1
# define FALSE	0

typedef enum e_token_type
{
	LEFT_1 = 1,		// <
	LEFT_2,			// <<
	RIGHT_1,		// >
	RIGHT_2			// >>
}	t_token_type;

/* [ parse -> execute ]: no alloc variable */
typedef struct s_shell_info
{
	int					pipe_cnt;	// pipe 개수
	struct s_env_info	*env;
	struct s_cmd_info	*cmd_node;	// cmd head -> execute 에서 다 free하고 넘길 것
}	t_shell_info;

/* struct for cmd_node */
typedef struct s_cmd_info
{
	int					cmd_cnt;		// cmd 개수
	char				**cmd_args;		// cmd 들만
	struct s_redir		*redir;			// redirection head
	struct s_cmd_info	*next;
}	t_cmd_info;

/* struct for redirections */
typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

/* struct for env */
typedef struct s_env_info
{
	char				*key;
	char				*value;
	struct s_env_info	*next;
}	t_env_info;

#endif
