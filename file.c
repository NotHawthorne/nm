/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:28:47 by alkozma           #+#    #+#             */
/*   Updated: 2020/02/22 00:29:44 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <mach-o/nlist.h>
#include <string.h>

void	*nm_load(char *file, struct stat *s)
{
	int			fd;
	void		*ret;

	if ((fd = open(file, O_RDONLY)) < 0)
	{
		printf("nm: open failed: %s\n", file);
		return (NULL);
	}
	if ((fstat(fd, s)) < 0)
	{
		printf("nm: stat failed: %s\n", file);
		return (NULL);
	}
	if ((ret = mmap(0, s->st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		printf("nm: mmap failed\n");
		return (NULL);
	}
	close(fd);
	return (ret);
}

char	nm_symbolize(struct nlist_64 *l, t_nm *info)
{
	int	tmp;
	int	desc;
	int	ext;

	tmp = l->n_type & N_TYPE;
	ext = l->n_type & N_EXT;
	desc = l->n_desc & ~REFERENCE_TYPE;
	if (tmp == N_UNDF)
	{
		if ((l->n_type & N_EXT) != 0 && l->n_value != 0)
			return ('C');
		return ('U');
	}
	else if (tmp == N_ABS)
		return (desc == REFERENCE_FLAG_DEFINED ? 'A' : 'a');
	else if (tmp == N_SECT)
	{
		if (!strcmp(info->stab[l->n_sect - 1], "__text"))
			return (ext ? 'T' : 't');
		else if (!strcmp(info->stab[l->n_sect - 1], "__data"))
			return (ext ? 'D' : 'd');
		else if (!strcmp(info->stab[l->n_sect - 1], "__bss"))
			return (ext ? 'B' : 'b');
	}
	return (ext ? 'S' : 's');
}

void	nm_insert_symbol(t_nm *info, t_nm_symbol *cur, t_nm_symbol *s)
{
	int	res;

	if (!cur)
		info->symbols = s;
	else
	{
		res = strcmp(cur->name, s->name);
		if (res < 0)
		{
			if (!cur->r)
				cur->r = s;
			else
				nm_insert_symbol(info, cur->r, s);
		}
		else
		{
			if (!cur->l)
				cur->l = s;
			else
				nm_insert_symbol(info, cur->l, s);
		}
	}
}

void	nm_print(t_nm_symbol *cur)
{
	if (cur->l)
		nm_print(cur->l);
	if (cur->undef)
		printf("                 ");
	else
		printf("%.16llx ", cur->val);
	printf("%c %s\n", cur->type, cur->name);
	if (cur->r)
		nm_print(cur->r);
	free(cur->name);
	free(cur);
	cur = NULL;
}

int		nm_symbols(void *start, struct mach_header_64 *h, t_nm *info)
{
	struct symtab_command	*c;
	struct nlist_64			*l;
	t_nm_symbol				*s;

	c = start;
	l = (void*)h + c->symoff;
	for (uint32_t i = 0; i != c->nsyms; i++)
	{
		s = malloc(sizeof(*s));
		s->name = strdup((void*)h + c->stroff + l->n_un.n_strx);
		s->l = 0;
		s->r = 0;
		s->val = l->n_value;
		s->type = nm_symbolize(l, info);
		s->undef = ((l->n_type & N_TYPE) == N_UNDF && !l->n_value);
		nm_insert_symbol(info, info->symbols, s);
		l = (void*)l + sizeof(struct nlist_64);
	}
	return (1);
}

void	nm_section_it(void *start, int nsects, t_nm *info)
{
	struct section_64 *s;

	s = start;
	while (nsects)
	{
		memcpy(info->stab[info->sections++], s->sectname, 16);
		s = ((void*)s + sizeof(struct section_64));
		nsects--;
	}
}

int		nm_dump(void *start, t_nm *info, int depth, int *os)
{
	struct segment_command_64	*c;
	struct load_command			*lc;

	lc = start;
	if (lc->cmd == LC_SYMTAB)
		info->symtab_start = start;
	c = start;
	if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
		if (c->nsects)
			nm_section_it((void*)(c + 1), c->nsects, info);
	if ((uint32_t)depth < info->header->ncmds)
	{
		*os += c->cmdsize;
		return (c->nsects + nm_dump(start + c->cmdsize, info, depth + 1, os));
	}
	return (0);
}

int		ft_nm(char *file)
{
	t_nm					info;
	void					*mem;
	int						os;

	memset(&info, 0, sizeof(struct s_nm));
	if (!(mem = nm_load(file, &(info.stat))))
		return (0);
	os = 0;
	info.header = mem;
	if (info.header->magic == MH_MAGIC_64 || info.header->magic == MH_CIGAM_64)
	{
		info.sections = nm_dump((void*)(info.header + 1), &info, 0, &os);
		nm_symbols(info.symtab_start, info.header, &info);
		munmap(info.header, info.stat.st_size);
		nm_print(info.symbols);
		if (errno)
			printf("%s\n",strerror(errno));	
		return (1);
	}
	printf("nm: invalid file: %s\n", file);
	return (0);
}
