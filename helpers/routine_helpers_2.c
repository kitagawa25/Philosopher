#include "../Philosophers.h"

void eat_routine_2(t_philo *philosophers, t_data *data)
{
    uint64_t        temp_time;

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
    if (philosophers->time_to_eat > 0)
        usleep(philosophers->time_to_eat * 1000);
    else
        usleep(250); // mini-pause
    if (philosophers->meals_eaten == philosophers->time_to_eat_number)
        philosophers->done = true;
}

void    routine_2(void *arg)
{
    t_philo *philosophers;
    t_data *data;
    t_regroup *regroup;

    regroup = (t_regroup *)arg;
    philosophers = regroup->philo;
    data = regroup->data;
    while (philosophers->meals_eaten < philosophers->time_to_eat_number && data->finished != 1)
    {
        if(philosophers->number_of_philosophers == 1)
            break;
        eat_routine(philosophers, data);
        sleep_routine(philosophers, data);
        think_routine(philosophers, data);
    }
}

void    routine_3(void *arg)
{
    t_philo *philosophers;
    t_data *data;
    t_regroup *regroup;
    long unsigned int i;

    i = 0;
    regroup = (t_regroup *)arg;
    philosophers = regroup->philo;
    data = regroup->data;
    while(data->finished != 1)
    {
        if(philosophers->number_of_philosophers == 1)
            break;
        eat_routine(philosophers, data);
        i++;
        sleep_routine(philosophers, data);
        think_routine(philosophers, data);
    }
}
pthread_t creating_threads(t_data *data)
{
    int         i;
    pthread_t   monitoring;
    t_regroup regroup[PHIL_MAX];

    if(pthread_create(&monitoring, NULL, &monitor, data) != 0) //thread qui sert a savoir si un philosophe meurt ou tous les philo ont mangés
    {
        ft_error(); //sécurité
        return (0);
    }
    i = 0;
    while(i < data->philosophers[0].number_of_philosophers) //boucle qui créer les threads des philosophes 
    {
        regroup[i].philo = &data->philosophers[i];
        regroup[i].data = data;
        if(pthread_create(&data->philosophers[i].t1, NULL, &routine, &regroup[i]) != 0 )
        {
            ft_error();
            return (0);
        }
        i++;
    }
    return (monitoring);
}

void joining_threads(t_data *data,  pthread_t  monitoring)
{
    int         i;

    i = 0;
    while(i < data->philosophers[0].number_of_philosophers)
    {
        if(pthread_join(data->philosophers[i].t1, NULL) != 0) //on separe le create et join car sinon il n'y aura qu'un thread a la fois qui sera executer
        {
            ft_error();
            return ;
        }
        i++;
    }
    if(pthread_join(monitoring, NULL) != 0) //executer le thread monitoring
    {
        ft_error();
        return ;
    }
}
