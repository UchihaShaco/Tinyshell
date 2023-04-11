/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 13:43:33 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/26 13:45:30 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*t;

	t = malloc(sizeof(t_list));
	if (t)
	{
		t->content = content;
		t->next = NULL;
		return (t);
	}
	return (NULL);
}
