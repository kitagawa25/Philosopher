#include "../Philosophers.h"

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg; //initialiser struct la t_data
    t_philo *philosophers = data->philosophers; //initialiser la struct philo

    while (!data->finished) //tant qu'aucun philo nest mort ou n'a fini de manger
    {
        if (is_philosopher_alone(data, philosophers) == 1)
            return NULL;
        if (ate_all_verif_monit(data) == -1)
            return NULL;
        // Ensuite : vérifie s’il y a une mort
        if (death_verif_monit(philosophers, data) == -1)
            return NULL;
        usleep(1000); //on attends pour ne pas que ça boucle trop vite
    }
    return NULL;
}

void eat_routine(t_philo *philosophers, t_data *data)
{
    uint64_t        temp_time;
    
    pthread_mutex_lock(philosophers->r_chopstick); //lock la fourchette droite
    pthread_mutex_lock(&data->write_lock);
    if (eat_routine_fork(data, philosophers) == -1)
        return ;
    pthread_mutex_unlock(&data->write_lock);
    pthread_mutex_lock(philosophers->l_chopstick); //lock la fourchette gauche
    pthread_mutex_lock(&data->write_lock);
    if (eat_routine_fork(data, philosophers) == -1)
        return ;
    pthread_mutex_unlock(&data->write_lock);
    pthread_mutex_lock(&data->write_lock);
    temp_time = get_time() - philosophers->start;
    if (is_data_finished(data) == 1)
        return ;
    printf("%lu %d is eating\n", (temp_time), philosophers->id); //print que le philosopher X est en train de manger
    pthread_mutex_unlock(&data->write_lock);
    pthread_mutex_lock(philosophers->meal_lock); //on lock le meal_lock pour que la variable soit inchangeable
    philosophers->last_meal = get_time(); //on actualise le time du last_meal
    philosophers->meals_eaten = philosophers->meals_eaten + 1; //on ajoue un au meal total
    pthread_mutex_unlock(philosophers->meal_lock); //on unlock le meal_lock
    eat_routine_which_usleep(philosophers);
    pthread_mutex_unlock(philosophers->r_chopstick); //on unlock la fourchette droite
    pthread_mutex_unlock(philosophers->l_chopstick); //on unlock la fourchette gauche
}
void sleep_routine(t_philo *philosophers, t_data *data)
{
    uint64_t        temp_time;
    
    temp_time = get_time() - philosophers->start;
    pthread_mutex_lock(&data->write_lock);
    if (is_data_finished(data) == 1)
        return ;
    printf("%lu %d is sleeping\n", temp_time, philosophers->id); //print que le philosopher X est en train de dormir
    pthread_mutex_unlock(&data->write_lock);
    if (philosophers->time_to_sleep > 0)
        usleep(philosophers->time_to_sleep * 1000);
    else
        usleep(250); // mini-pause
}
void think_routine(t_philo *philosophers, t_data *data)
{
    uint64_t        temp_time;

    temp_time = get_time() - philosophers->start;
    pthread_mutex_lock(&data->write_lock);
    if (is_data_finished(data) == 1)
        return ;
    printf("%lu %d is thinking\n", temp_time, philosophers->id); //print que le philosopher X est en train de réfléchir
    pthread_mutex_unlock(&data->write_lock);
}

void    *routine(void *arg)
{  //routine individuelle
    t_philo *philosophers;
    t_data *data;
    t_regroup *regroup;

    regroup = (t_regroup *)arg;
    philosophers = regroup->philo;
    data = regroup->data;
    if (philosophers->id % 2) //on fait dormir un philosophe sur deux pour ne pas que les threads se  chevauchent
        usleep(philosophers->number_of_philosophers * 1000); //anciennement 15000
    if (philosophers->time_to_eat_number != -1)
    {
        while (philosophers->meals_eaten < philosophers->time_to_eat_number && data->finished != 1)
            if (intra_routine(philosophers, data) == 1)
                break;
    }
    else
    {
        while(data->finished != 1)
            if (intra_routine(philosophers, data) == 1)
                break;
    }
    return NULL;
}
