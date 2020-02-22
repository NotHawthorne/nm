/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:28:07 by alkozma           #+#    #+#             */
/*   Updated: 2020/02/22 00:29:39 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int		main(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc == 2)
		ft_nm(argv[i]);
	else if (argc >= 2)
	{
		printf("\n");
		while (i < argc)
		{
			printf("%s:\n", argv[i]);
			ft_nm(argv[i++]);
			if (i < argc)
				printf("\n");
		}
	}
	else
		printf("usage: nm [file]\n");
}
