#include "Philosophers.h"

int is_philosopher_alone(t_data *data, t_philo *philosophers) //routine spécial si nbr of philo = 1
{
    if (philosophers->number_of_philosophers == 1)
    {
        pthread_mutex_lock(philosophers->r_chopstick); //lock la fourchette droite
        pthread_mutex_lock(&data->write_lock);
        printf("%lu %d has taken a fork\n", (get_time() - philosophers->start), philosophers->id); //print qu'on l'a prise
        pthread_mutex_unlock(&data->write_lock);
        pthread_mutex_unlock(philosophers->r_chopstick); //on l'unlock
        usleep(philosophers->time_to_die * 1000); //on attends le time to die
        pthread_mutex_lock(&data->write_lock); //on lock le write de data
        data->finished = 1; //on change le finished de data à un pour que le programme s'arrête
        printf("%lu %d died\n", (get_time() - philosophers->start), philosophers->id); //print qu'on l'a prise
        pthread_mutex_unlock(&data->write_lock); //on unlock le write de data
        return (1);
    }
    return (0);
}

int is_time_to_die_inferior(t_philo *philosophers, t_data *data)
{
    bool tgtb;

    tgtb = false;
    if (philosophers->time_to_die < philosophers->time_to_eat || philosophers->time_to_die < philosophers->time_to_sleep)
    {
        pthread_mutex_lock(&data->write_lock);
        printf("%lu 1 has taken a fork\n",  (get_time() - philosophers->start)); 
        printf("%lu 1 has taken a fork\n",  (get_time() - philosophers->start));
        printf("%lu 1 is eating\n",  (get_time() - philosophers->start));
        pthread_mutex_unlock(&data->write_lock);
        if(philosophers->time_to_die < philosophers->time_to_sleep)
        {
            tgtb = true;
            usleep(philosophers->time_to_eat * 1000);
            pthread_mutex_lock(&data->write_lock);
            printf("%lu 1 is sleeping\n", (get_time() - philosophers->start));
            pthread_mutex_unlock(&data->write_lock);
        }
        if (tgtb == true && philosophers->time_to_die < philosophers->time_to_sleep)
            usleep((philosophers->time_to_die - philosophers->time_to_eat)* 1000);
        else if (tgtb == true && philosophers->time_to_die < philosophers->time_to_eat)
            usleep((philosophers->time_to_eat - philosophers->time_to_die)* 1000);
        else if (tgtb == false)
            usleep(philosophers->time_to_die * 1000);
        pthread_mutex_lock(&data->write_lock); //on lock le write lock pour qu'il reste inmodifiable
        data->finished = 1; //on set le finished à 1
        printf("%lu 1 died\n", (get_time() - philosophers->start));
        pthread_mutex_unlock(&data->write_lock); //on délock le write
        return (1);
    }
    return (0);
}

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg; //initialiser struct la t_data
    t_philo *philosophers = data->philosophers; //initialiser la struct philo
    long unsigned int time;
    int i;
    int fin;

    i = 0;
    while (!data->finished) //tant qu'aucun philo nest mort ou n'a fini de manger
    {
        i = 0;
        fin = 0;
        if (is_philosopher_alone(data, philosophers) == 1)
            return NULL;
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
            return NULL;
        }
        // Ensuite : vérifie s’il y a une mort
        i = 0; //on le re set a zéro pour re vérifier tous les philo
        while (i < philosophers->number_of_philosophers) //tant que i est inférieur au tot philo
        {
            
            pthread_mutex_lock(philosophers[i].meal_lock); //on lock le meal lock pour que la var reste inchangeable
            time = get_time() - philosophers[i].last_meal; //on dit que le time est égal au temps écoulé depuis le last meal du philo i
            // printf("%lu\n", time);
            if (time > philosophers[i].time_to_die) //si le temps est supérieur au time to die 
            {
                pthread_mutex_lock(&data->write_lock); //on lock le write lock pour que la variable reste inchangeable
                data->finished = 1; //on set le data finished à 1
                printf("%lu %d died\n", (get_time() - philosophers->start), philosophers[i].id);
                pthread_mutex_unlock(&data->write_lock); //on unlock write_lock
                pthread_mutex_unlock(philosophers[i].meal_lock); //on unlock le meal_lock du philo i
                return NULL;
            }
            pthread_mutex_unlock(philosophers[i].meal_lock); //on unlock le meal_lock de si on est pas entré dans la boucle
            i++; //on avance au philo suivant
        }
            usleep(1000); //on attends pour ne pas que ça boucle trop vite
    }
    return NULL;
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

void    *routine (void *arg) //routine individuelle
{
    t_philo *philosophers;
    t_data *data;
    t_regroup *regroup;
    bool argv;
    long unsigned int i;

    i = 0;
    regroup = (t_regroup *)arg;
    philosophers = regroup->philo;
    data = regroup->data;
    if (philosophers->id % 2) //on fait dormir un philosophe sur deux pour ne pas que les threads se  chevauchent
        usleep(philosophers->number_of_philosophers * 1000); //anciennement 15000
    argv = false;
    if (philosophers->time_to_eat_number != -1)
    argv = true;
    if (argv == true)
    {
        while (philosophers->meals_eaten < philosophers->time_to_eat_number && data->finished != 1)
        {
            if(philosophers->number_of_philosophers == 1)
                break;
            eat_routine(philosophers, data);
            sleep_routine(philosophers, data);
            think_routine(philosophers, data);
        }
    }
    else if (argv == false)
    {
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
    pthread_mutex_lock(philosophers->meal_lock); //on lock le meal lock pour ne pas qu'il soit modifiable
    // philosophers->done = true; // on met le done a true car tous les philo auront manger leurs repas
    pthread_mutex_unlock(philosophers->meal_lock);
    return NULL;
}

void create_threads(t_data *data) //création des threads des philosophes et du moniteur et lancement
{
    int         i;
    pthread_t   monitoring;
    t_regroup regroup[PHIL_MAX];

    if(pthread_create(&monitoring, NULL, &monitor, data) != 0) //thread qui sert a savoir si un philosophe meurt ou tous les philo ont mangés
    {
        ft_error(); //sécurité
        return ;
    }
    i = 0;
    while(i < data->philosophers[0].number_of_philosophers) //boucle qui créer les threads des philosophes 
    {
        regroup[i].philo = &data->philosophers[i];
        regroup[i].data = data;
        if(pthread_create(&data->philosophers[i].t1, NULL, &routine, &regroup[i]) != 0 )
        {
            ft_error();
            return ;
        }
        i++;
    }
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
    return ;
}
