/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ko.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 13:38:34 by jalwahei          #+#    #+#             */
/*   Updated: 2023/02/28 11:34:48 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char *args[] = { "/bin/ls", "-l", NULL };
    char *envp[] = { NULL };
    if (execve(args[0], args, envp) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }
    return 0;
}
