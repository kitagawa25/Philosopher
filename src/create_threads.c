#include "../Philosophers.h"

int create_threads_2(t_data *data)
{
    int         i;
    t_regroup regroup[PHIL_MAX];

    i = 0;
    while(i < data->philosophers[0].number_of_philosophers) //boucle qui créer les threads des philosophes 
    {
        regroup_setup(regroup, data, i);
        if(pthread_create(&data->philosophers[i].t1, NULL, &routine, &regroup[i]) != 0 )
        {
            ft_error();
            return (-1);
        }
        i++;
    }
    i = 0;
    while(i < data->philosophers[0].number_of_philosophers)
    {
        if(pthread_join(data->philosophers[i].t1, NULL) != 0) //on separe le create et join car sinon il n'y aura qu'un thread a la fois qui sera executer
        {
            ft_error();
            return (-1);
        }
        i++;
    }
    return (0);
}

void create_threads(t_data *data)
{  //création des threads des philosophes et du moniteur et lancement
    // int         i;
    pthread_t   monitoring;
    // t_regroup regroup[PHIL_MAX];

    if(pthread_create(&monitoring, NULL, &monitor, data) != 0) //thread qui sert a savoir si un philosophe meurt ou tous les philo ont mangés
    {
        ft_error(); //sécurité
        return ;
    }
    if (create_threads_2(data) == -1)
        return ;
    if(pthread_join(monitoring, NULL) != 0) //executer le thread monitoring
    {
        ft_error();
        return ;
    }
    return ;
}
