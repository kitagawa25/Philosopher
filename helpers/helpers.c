#include "../Philosophers.h"

int		ft_atoi(const char *str)
{
	int sign;
	int i;
	int res;

	i = 0;
	sign = 1;
    res = 0;
	
    while((str[i] == ' ') || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if(str[i] == '-')
            sign *= (-1);
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = (res * 10) + (str[i] - 48);
        i++;
    }
    return (res * sign);
}

long int	get_time(void)
{
	//!gets the actual time
	struct timeval		actual_time; //structure qui contient tv_sec et tv_usec (temps en seconde et millisecondes)
	long int			time; //variable qui stock le temps

	time = 0; //on set à 0 au début
	if (gettimeofday(&actual_time, NULL) == -1)
		ft_error(); //securiter 
	time = (actual_time.tv_sec * 1000) + (actual_time.tv_usec / 1000); //obtenir le temps en millisecondes
	return (time);
}

void    ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	if (str == NULL)
		return ;
	while (str[i])
	{
		ft_putchar_fd(str[i], fd);
		i++;
	}
    return ;
}
int is_data_finished(t_data *data)
{
    if (data->finished == 1)
    {
        pthread_mutex_unlock(&data->write_lock);
        return (1);
    }
    return (0);
}
