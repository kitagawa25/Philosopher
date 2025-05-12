#include "../Philosophers.h"

int	is_data_finished(t_data *data)
{
	if (data->finished == 1)
	{
		pthread_mutex_unlock(&data->write_lock);
		return (1);
	}
	return (0);
}

void	eat_routine_which_usleep(t_philo *philosophers)
{
	if (philosophers->time_to_eat > 0)
		usleep(philosophers->time_to_eat * 1000);
	else
		usleep(250); // mini-pause
	if (philosophers->meals_eaten == philosophers->time_to_eat_number)
		philosophers->done = true;
}

int	eat_routine_fork(t_data *data, t_philo *philosophers)
{
	uint64_t	temp_time;

	temp_time = get_time() - philosophers->start;
	if (is_data_finished(data) == 1)
		return (-1);
	printf("%lu %d has taken a fork\n",
		(temp_time), philosophers->id); //print qu'on a prit la fourchette
	return (0);
}

int	intra_routine(t_philo *philosophers, t_data *data)
{
	if (philosophers->number_of_philosophers == 1)
		return (1);
	eat_routine(philosophers, data);
	sleep_routine(philosophers, data);
	think_routine(philosophers, data);
	return (0);
}

void	regroup_setup(t_regroup *regroup, t_data *data, int i)
{
	regroup[i].philo = &data->philosophers[i];
	regroup[i].data = data;
}
