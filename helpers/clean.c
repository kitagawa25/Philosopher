#include "../Philosophers.h"

void	clean_mutex(t_data *data, pthread_mutex_t *chopsticks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->write_lock);
	while (i < data->philosophers[0].number_of_philosophers)
	{
		pthread_mutex_destroy(&chopsticks[i]);
		i++;
	}
	return ;
}
