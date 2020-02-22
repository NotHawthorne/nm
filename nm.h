/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 15:23:41 by alkozma           #+#    #+#             */
/*   Updated: 2020/02/22 00:28:02 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <fcntl.h>
# include <mach-o/loader.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <errno.h>
# include <stdlib.h>

typedef struct	s_nm
{
	struct stat				stat;
	struct mach_header_64	*header;
	char					stab[24][16];
	size_t					sections;
	void					*symtab_start;
	struct s_nm_symbol		*symbols;
}				t_nm;

typedef struct	s_nm_symbol
{
	char				*name;
	uint64_t			val;
	char				type;
	struct s_nm_symbol	*l;
	struct s_nm_symbol	*r;
	int					undef;
}				t_nm_symbol;

void	*nm_load(char *file, struct stat *s);
int		ft_nm(char *file);

#endif
