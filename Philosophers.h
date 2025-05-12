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

//!----------------------------------------------------------------HELPERS
//-----------------------------------------------------------------clean.c
void        clean_mutex(t_data *data, pthread_mutex_t *chopsticks);
//-----------------------------------------------------------------helpers.c
int         ft_atoi(const char *str);
long int    get_time(void);
void        ft_putchar_fd(char c, int fd);
void        ft_putstr_fd(char *str, int fd);
int         ft_error(void);
//-----------------------------------------------------------------routine_helpers_2.c
int         is_data_finished(t_data *data);
void        eat_routine_which_usleep(t_philo *philosophers);
int         eat_routine_fork(t_data *data, t_philo *philosophers);
int         intra_routine(t_philo *philosophers, t_data *data);
void        regroup_setup(t_regroup *regroup, t_data *data, int i);
//-----------------------------------------------------------------routine_helpers.c
int 		is_philosopher_alone(t_data *data, t_philo *philosophers);
void 		is_time_to_die_inferior_2(t_philo *philosophers, t_data *data);
int 		is_time_to_die_inferior(t_philo *philosophers, t_data *data);
int 		ate_all_verif_monit(t_data *data);
int 		death_verif_monit(t_philo *philosophers, t_data *data);
//!-------------------------------------------------------------------SRC
//--------------------------------------------------------------------create_threads.c
int 		create_threads_2(t_data *data);
void 		create_threads(t_data *data);
//--------------------------------------------------------------------initialisation.c
void 		data_init(t_philo *philosophers, t_data *data);
void 		chopsticks_init(pthread_mutex_t *chopsticks, int number_of_philosophers);
void 		input_init(char **argv, t_philo *philosophers);
void 		philosophers_init_2(char **argv, t_philo *philosophers, t_data *data, pthread_mutex_t *chopsticks, int i);
void    	philosophers_init(char **argv, t_philo *philosophers, t_data *data, pthread_mutex_t *chopsticks);
//--------------------------------------------------------------------main.c
int 		is_number(char *tab);
int 		input_checker_2(char **argv, int i);
int 		input_checker(int argc, char **argv);
//--------------------------------------------------------------------routine.c
void 		*monitor(void *arg);
void 		eat_routine(t_philo *philosophers, t_data *data);
void 		sleep_routine(t_philo *philosophers, t_data *data);
void 		think_routine(t_philo *philosophers, t_data *data);
void    	*routine(void *arg);

#endif