/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:06:39 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/27 17:36:07 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_head;
	t_list	*node;
	t_list	*lst_ptr;

	if (!lst || !f || !del)
		return (NULL);
	new_head = NULL;
	lst_ptr = lst;
	while (lst_ptr)
	{
		node = ft_lstnew(f(lst_ptr->content));
		if (!node)
		{
			ft_lstclear(&new_head, del);
			return (NULL);
		}
		ft_lstadd_back(&new_head, node);
		lst_ptr = lst_ptr->next;
	}
	return (new_head);
}
