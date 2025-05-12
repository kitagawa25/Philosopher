#include "../Philosophers.h"

int	is_philosopher_alone(t_data *data, t_philo *philosophers)
{//routine spécial si nbr of philo = 1
	if (philosophers->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philosophers->r_chopstick); //lock la fourchette droite
		pthread_mutex_lock(&data->write_lock);
		printf("%lu %d has taken a fork\n",
			(get_time() - philosophers->start), philosophers->id); //print qu'on l'a prise
		pthread_mutex_unlock(&data->write_lock);
		pthread_mutex_unlock(philosophers->r_chopstick); //on l'unlock
		usleep(philosophers->time_to_die * 1000); //on attends le time to die
		pthread_mutex_lock(&data->write_lock); //on lock le write de data
		data->finished = 1; //on change le finished de data à un pour que le programme s'arrête
		printf("%lu %d died\n", 
			(get_time() - philosophers->start), philosophers->id); //print qu'on l'a prise
		pthread_mutex_unlock(&data->write_lock); //on unlock le write de data
		return (1);
	}
	return (0);
}

void	is_time_to_die_inferior_2(t_philo *philosophers, t_data *data)
{
	bool	tgtb;

	tgtb = false;
	if (philosophers->time_to_die < philosophers->time_to_sleep)
	{
		tgtb = true;
		usleep(philosophers->time_to_eat * 1000);
		pthread_mutex_lock(&data->write_lock);
		printf("%lu 1 is sleeping\n", (get_time() - philosophers->start));
		pthread_mutex_unlock(&data->write_lock);
	}
	if (tgtb == true && philosophers->time_to_die < philosophers->time_to_sleep)
		usleep((philosophers->time_to_die - philosophers->time_to_eat) * 1000);
	else if (tgtb == true
		&& philosophers->time_to_die < philosophers->time_to_eat)
		usleep((philosophers->time_to_eat - philosophers->time_to_die) * 1000);
	else if (tgtb == false)
		usleep(philosophers->time_to_die * 1000);
}

int	is_time_to_die_inferior(t_philo *philosophers, t_data *data)
{
	if (philosophers->time_to_die < philosophers->time_to_eat
		|| philosophers->time_to_die < philosophers->time_to_sleep)
	{
		pthread_mutex_lock(&data->write_lock);
		printf("%lu 1 has taken a fork\n",
			(get_time() - philosophers->start));
		printf("%lu 1 has taken a fork\n",
			(get_time() - philosophers->start));
		printf("%lu 1 is eating\n", (get_time() - philosophers->start));
		pthread_mutex_unlock(&data->write_lock);
		is_time_to_die_inferior_2(philosophers, data);
		pthread_mutex_lock(&data->write_lock); //on lock le write lock pour qu'il reste inmodifiable
		data->finished = 1; //on set le finished à 1
		printf("%lu 1 died\n", (get_time() - philosophers->start));
		pthread_mutex_unlock(&data->write_lock); //on délock le write
		return (1);
	}
	return (0);
}

int	ate_all_verif_monit(t_data *data)
{
	// t_data *data = (t_data *)arg; //initialiser struct la t_data
	t_philo	*philosophers; //initialiser la struct philo
	int		i;
	int		fin;

	philosophers = data->philosophers;
	i = 0;
	fin = 0;
	// D'abord : vérifie si tous ont mangé
	while (i < philosophers->number_of_philosophers) //tant qu'on est pas arrivé au nombre tot de philo
	{
		pthread_mutex_lock(philosophers[i].meal_lock); //lock le mutex pour le meal ( car on a besoin de sa valeur intacte et non modifié par d'autres threads)
		if (philosophers[i].done) // si il y a argv 5 et  que ce que le philo a mangé est supérieur à l'argv 5
			fin++; //on incrémente fin
		pthread_mutex_unlock(philosophers[i].meal_lock); //on unlock le mutex
		i++; //on passe au philo suivant
	}
	if (fin == philosophers->number_of_philosophers) //si fin est égal au numéro tot de philo
	{
		pthread_mutex_lock(&data->write_lock); //on lock le write lock pour qu'il reste inmodifiable
		data->finished = 1; //on set le finished à 1
		pthread_mutex_unlock(&data->write_lock); //on délock le write
		return (-1);
	}
	return (0);
}

int	death_verif_monit(t_philo *philosophers, t_data *data)
{
	int					i;
	long unsigned int	time;

	i = 0; //on le re set a zéro pour re vérifier tous les philo
	while (i < philosophers->number_of_philosophers) //tant que i est inférieur au tot philo
	{
		pthread_mutex_lock(philosophers[i].meal_lock); //on lock le meal lock pour que la var reste inchangeable
		time = get_time() - philosophers[i].last_meal; //on dit que le time est égal au temps écoulé depuis le last meal du philo i
		if (time > philosophers[i].time_to_die) //si le temps est supérieur au time to die 
		{
			pthread_mutex_lock(&data->write_lock); //on lock le write lock pour que la variable reste inchangeable
			data->finished = 1; //on set le data finished à 1
			printf("%lu %d died\n",
				(get_time() - philosophers->start), philosophers[i].id);
			pthread_mutex_unlock(&data->write_lock); //on unlock write_lock
			pthread_mutex_unlock(philosophers[i].meal_lock); //on unlock le meal_lock du philo i
			return (-1);
		}
		pthread_mutex_unlock(philosophers[i].meal_lock); //on unlock le meal_lock de si on est pas entré dans la boucle
		i++; //on avance au philo suivant
	}
	return (0);
}
