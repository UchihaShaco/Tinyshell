#include "../minishell.h"
/* NOTES
	exit 5a 5
	exit a 
	exit 5 5a
	exit 50 (normal)
	exit 256
*/
//return last exit status
//exit out of shell
//if more than one argument, do not exit and send an error message
int	ft_exit(t_data *data);
//or void????
//exit with multiple number arguments