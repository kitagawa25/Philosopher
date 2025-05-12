#include "../Philosophers.h"

void data_init(t_philo *philosophers, t_data *data)
{  // initialiser les variables de la structure data
    pthread_mutex_init(&data->meal_lock, NULL); //!initialiser le mutex qui sert à lock les meals
    pthread_mutex_init(&data->write_lock, NULL); //!initialiser le mutex qui sert à lock les write
    pthread_mutex_init(&data->dead_lock, NULL); //!initialiser le mutex qui sert à lock les deaths
    data->finished = 0; //! combiens de philosophers on finish -> 0 au début
    data->philosophers = philosophers; //! synchroniser les deux structures pour init (sans malloc)
}

void chopsticks_init(pthread_mutex_t *chopsticks, int number_of_philosophers)
{ // initialiser les chopsticks toujours recréer la variable, et recréer ce dont on a besoin dans la structure  
    int i;

    i = 0;
    while (i < number_of_philosophers)
    {
        pthread_mutex_init(&chopsticks[i], NULL); //! initaliser le nombre de chopsticks en fonction du nombre de philosophers, initialiser avec un indice pour donner le sens
        i++;
    }
    return ;
}

void input_init(char **argv, t_philo *philosophers)
{ //synchroniser les time to 'action' avec les arguments entrés
    philosophers->time_to_eat = ft_atoi(argv[3]);
    philosophers->time_to_die = ft_atoi(argv[2]);
    philosophers->time_to_sleep = ft_atoi(argv[4]);
    philosophers->number_of_philosophers = ft_atoi(argv[1]);
    if (ft_atoi(argv[3]) == 0)
        philosophers->time_to_eat = ft_atoi(argv[3]) + 3;
    if (ft_atoi(argv[4]) == 0)
        philosophers->time_to_sleep = ft_atoi(argv[4]) + 3;
    if(argv[5] != NULL)
        philosophers->time_to_eat_number = ft_atoi(argv[5]);
    else
        philosophers->time_to_eat_number = -1; // car peut être égal à 0
}

void philosophers_init_2(char **argv, t_philo *philosophers, t_data *data, pthread_mutex_t *chopsticks, int i)
{
    philosophers[i].done = false; //on met le done a false pour le valider uniquement si tous les philo ont manger leur repas (argv 5)
    philosophers[i].start = get_time(); //On initialise le temps de départ du philosophe
    philosophers[i].last_meal = get_time(); //On initialise le temps du dernier repas
    philosophers[i].id = i + 1; //on set le premier philosophe à 1 et non 0
    philosophers[i].eating = 0; //aucun philosophe ne mange au départ
    philosophers[i].meals_eaten = 0; //aucun philosophe n'a mangé de repas au départ
    input_init(argv, &philosophers[i]); //initialiser la struct de philosophers en fonction des inputs entrés
    philosophers[i].write_lock = &data->write_lock; //On set les locks 
    philosophers[i].dead_lock = &data->dead_lock;
    philosophers[i].meal_lock = &data->meal_lock;
    philosophers[i].dead = &data->finished; //récupère le flag de la structure data qui indique si le philosophe est mort, et ajoute ce flag à la structure du philosophe
    philosophers[i].l_chopstick = &chopsticks[i]; //Le chopstick gauche correspond à l'indice du philosophe
}

void    philosophers_init(char **argv, t_philo *philosophers, t_data *data, pthread_mutex_t *chopsticks)
{ //!init des paramètres du philo et set des chopsticks dans le sens du cercle
    int i;

    i = 0;
    philosophers->number_of_philosophers = ft_atoi(argv[1]);
    data->finished = 0;
    while(i < philosophers->number_of_philosophers)
    {
        if (argv[5] != NULL)
            philosophers[i].time_to_eat_number = ft_atoi(argv[5]);
        else
            philosophers[i].time_to_eat_number = -1;
        philosophers_init_2(argv, philosophers, data, chopsticks, i);
        if (i == 0) //si nous sommes sur notre premier philo, alors sa chopstick droite est égal au dernier philo set du cercle
            philosophers[i].r_chopstick = &chopsticks[philosophers[i].number_of_philosophers - 1];
        else
            philosophers[i].r_chopstick = &chopsticks[i - 1]; //sinon le chopstick droit est égal au philosophe d'avant
        i++;
    }
    return ;
} 
