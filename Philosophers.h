#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>

# define PHIL_MAX 200

// uint64_t board; //define euint64 board
struct    s_data; //this line is to define the structure before actually saying what's inside it

typedef struct s_philo //personnal data of each philosopher
{
    pthread_t       t1; //une partie du processus en action
    int             id;
    int             meals_eaten;
    int             eating;
    uint64_t        last_meal;
    uint64_t        time_to_die;
    uint64_t        time_to_eat;
    uint64_t        time_to_sleep;
    uint64_t        start; //type can store 64bits
    pthread_mutex_t    *write_lock;
    pthread_mutex_t    *dead_lock;
    pthread_mutex_t    *meal_lock;
    pthread_mutex_t    *r_chopstick;
    pthread_mutex_t    *l_chopstick;
    int             number_of_philosophers;
    int             time_to_eat_number;
    int             *dead;
    bool             done;
} t_philo;

typedef struct s_data //general data
{
    int             finished;
    t_philo         *philosophers;
    pthread_mutex_t    write_lock;
    pthread_mutex_t    dead_lock;
    pthread_mutex_t    meal_lock;
} t_data;

typedef struct s_regroup {
    t_philo *philo;
    t_data  *data;
} t_regroup;

// ------------------------------------------------------------------secure.c
int     input_checker(int argc, char **argv) ;
int     is_number(char *tab);
//-------------------------------------------------------------------helpers.c
int            ft_atoi(const char *str);
long int    get_time(void);
int            ft_error(void);
void        ft_putchar_fd(char c, int fd);
void        ft_putstr_fd(char *str, int fd);
int            ft_usleep(size_t milliseconds);
//-------------------------------------------------------------------initialisation.c
void     data_init(t_philo *philosophers, t_data *data);
void     chopsticks_init(pthread_mutex_t *chopsticks, int number_of_philosophers);
void    philosophers_init(char **argv, t_philo *philosophers, t_data *data, pthread_mutex_t *chopsticks);
void     input_init(char **argv, t_philo *philosophers);
//------------------------------------------------------------------routine.c
void eat_routine(t_philo *philosophers, t_data *data);
void sleep_routine(t_philo *philosophers, t_data *data);
void think_routine(t_philo *philosophers, t_data *data);
void *routine (void *arg);
void create_threads(t_data *data);
void *monitor(void *arg);
int is_philosopher_alone(t_data *data, t_philo *philosophers);
int is_time_to_die_inferior(t_philo *philosophers, t_data *data);
//-----------------------------------------------------------------clean.c
void clean_mutex(t_data *data, pthread_mutex_t *chopsticks);

#endif