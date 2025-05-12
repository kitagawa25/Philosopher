#include "../Philosophers.h"

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg; //initialiser struct la t_data
    t_philo *philosophers = data->philosophers; //initialiser la struct philo

    while (!data->finished) //tant qu'aucun philo nest mort ou n'a fini de manger
    {
        if (is_philosopher_alone(data, philosophers) == 1)
            return NULL;
        if (monitor_meals_verif(arg) == -1)
            return NULL;
        if (monitor_death_verif(arg) == -1)
            return NULL;
    }
    usleep(1000); //on attends pour ne pas que ça boucle trop vite
    return NULL;
}

void eat_routine(t_philo *philosophers, t_data *data)
{
    uint64_t        temp_time;
    
    pthread_mutex_lock(philosophers->r_chopstick); //lock la fourchette droite
    pthread_mutex_lock(&data->write_lock);
    temp_time = get_time() - philosophers->start;
    if (is_data_finished(data) == 1)
        return ;
    printf("%lu %d has taken a fork\n", (temp_time), philosophers->id); //print qu'on a prit la fourchette droite
    pthread_mutex_unlock(&data->write_lock);
    pthread_mutex_lock(philosophers->l_chopstick); //lock la fourchette gauche
    pthread_mutex_lock(&data->write_lock);
    temp_time = get_time() - philosophers->start;
    if (is_data_finished(data) == 1)
        return ;
    printf("%lu %d has taken a fork\n", (temp_time), philosophers->id); //print qu'on a prit la fouchette gauche
    pthread_mutex_unlock(&data->write_lock);
    eat_routine_2(philosophers, data);
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

void    *routine (void *arg)
{ //routine individuelle
    t_philo *philosophers;
    t_regroup *regroup;
    bool argv;

    regroup = (t_regroup *)arg;
    philosophers = regroup->philo;
    if (philosophers->id % 2) //on fait dormir un philosophe sur deux pour ne pas que les threads se  chevauchent
        usleep(philosophers->number_of_philosophers * 1000); //anciennement 15000
    argv = false;
    if (philosophers->time_to_eat_number != -1)
        argv = true;
    if (argv == true)
        routine_2(arg);
    else if (argv == false)
        routine_3(arg);
    return NULL;
}

