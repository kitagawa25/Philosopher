#include "Philosophers.h"

int is_number(char *tab) //!check si c'est autre chose qu'un nombre
{
    int i;

    i = 0;
    while(tab[i])
    {
        if(!(tab[i] >= '0' && tab[i] <= '9'))
        {
            printf("You may only enter numbers (non negative).\n");
            return (-1);
        }
        i++;
    }
    return (0);
}

int input_checker(int argc, char **argv) //!teste si on a bien juste des nombres positifs et corrects
{
    int i;

    i = 1;
    while(argv[i] && i < argc && argc >= 5 && argc <= 6)
    {
        if (is_number(argv[i]) == -1)
            return (-1);
        if((ft_atoi(argv[1]) > PHIL_MAX) || ft_atoi(argv[1]) <= 0)
        {
            printf("There can only be 1 to 200 philosophers\n");
            return (-1);
        }
        if ((argv[5] != NULL && ft_atoi(argv[5]) <= 0) || ( i < 5 && ft_atoi(argv[i]) < 0))
        {
            if(argv[5] != NULL && ft_atoi(argv[5]) < 0)
            {
                printf("Argument value can't be less than 0\n");
                return (-1);
            }
            else if(argv[5] != NULL && ft_atoi(argv[5]) == 0)
            {
                printf("Philosophers aren't able to do their routine because they're not allowed to eat any meal.\n");
                return (-1);
            }
            else 
            {
                printf("Argument value can't less than 0 or more than 2147483647\n");
                return (-1);
            }
        }
        if( i < 5 && ft_atoi(argv[2]) == 0)
        {
            printf("0 1 died\n");
            return (-1);
        }
        i++;
    }
    if(i < 5)
    {
        printf("You should enter the number of philosphers, the time to die, the time to eat, time to sleep and the number of loop.\n");
        return (-1);
    }
    return (0);
}

int main(int argc, char **argv)
{
    t_philo            philosophers[PHIL_MAX]; //!créer la structure pour les philo en limitant le nombre (200)
    t_data            data; //!créer la structure du programme
    pthread_mutex_t    chopsticks[PHIL_MAX]; //!créer le mutex qui va bloquer les baguettes ou non

    if (input_checker(argc, argv) == -1) //!check si les arguments sont valides
        return (-1);
    chopsticks_init(chopsticks, ft_atoi(argv[1])); //!initialiser les chopsticks
    data_init(philosophers, &data); //! initialiser les var de la struct data et la structure philo
    philosophers_init(argv, philosophers, &data, chopsticks); //! initialiser les philos
    if (is_time_to_die_inferior(philosophers, &data) == 1)
                return (0);
    create_threads(&data);
    clean_mutex(&data, chopsticks);
    return (0);
} 