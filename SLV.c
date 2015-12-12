


























































































































































































































































































































































































































































































































































//
//
//
//
//
//

//Includes
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include <assert.h>

//Constants
#define TEACHER_IDENTIFIER_LENGTH (3+1)
#define CLASS_IDENTIFIER_LENGTH (4+1)
#define TEACHERS_PR_JOB (3)
#define JOBS_PR_MODULE (15)
#define MODULES_PR_DAY (6)
#define DAYS_PR_WEEK (5)
#define ELIMINATION_PART (0.5)
#define MUTATION_PART 0.10
#define DAY_TO_SWAP1 1
#define DAY_TO_SWAP2 5

//Enumerations
enum subjects {
    art,        //billedkunst
    biology,    //biologi
    danish,     //dansk
    english,    //engelsk
    physics,    //fysik
    geography,  //geografi
    history,    //historie
    crafting,   //håndarbejde
    phys_ed,    //idræt
    classtime,  //klassens time
    religion,   //kristendomskundskab
    cooking,    //hjemkundskab
    math,       //matematik
    music,      //musik
    nature,     //natur-teknik
    socialstud, //samfundsfag
    woodwork,   //sløjd
    german,     //tysk
    elective,    //valgfag
    prep        //forberedelsestid
};


//Structures
struct job {
    char teacher[TEACHERS_PR_JOB][TEACHER_IDENTIFIER_LENGTH];
    char class[CLASS_IDENTIFIER_LENGTH];
    enum subjects subject;
    int prep_confirmed;
};

struct module {
    struct job jobs[JOBS_PR_MODULE];
};

struct day {
    struct module modules[MODULES_PR_DAY];
};

struct week {
    struct day days[DAYS_PR_WEEK];
};

typedef struct fitted_population {
    int week_fitness;
    struct week *week_pointer;
    double roulette_part;
} fitted_population_t;

//Function prototypes
void next_generation(struct week* population_pool, unsigned int n);
int compare_fitness(const void *a, const void *b);
void init_fitness_of_weeks(fitted_population_t *population_fitnesses,
                              struct week* population_pool, unsigned int n);
void assign_roulette_part(fitted_population_t *population_fitnesses, unsigned int n);
void individual_picker(fitted_population_t *population_fitnesses,
                       int *individuals_killed, int amount_killed,
                       unsigned int n);
int gene_rand_num(int amount_killed);
void mutator(int j, int rand_1, fitted_population_t *population_fitnesses);
void create_new_individual(int j, int rand_1, int rand_2, fitted_population_t *population_fitnesses);

//main function
int main(int argc, char *argv[]) {

srand(1);


    return 0;
}

//Function that generates a new population.
//This function overwrites the initial population (thus returning void).
void next_generation(struct week* population_pool, unsigned int n) {
    int j, random_week_1=-123, random_week_2=-123,
        amount_killed=0, amount_living=0;
    int *individuals_killed = 0;
    fitted_population_t *population_fitnesses = 0;

    init_fitness_of_weeks(population_fitnesses, population_pool, n);

    amount_killed = n*ELIMINATION_PART;
    amount_living = n - amount_killed;

    individuals_killed = (int *)malloc(sizeof(int)*amount_killed);
    assert(individuals_killed != NULL);

    //sorts the array, so that the fittest individuals are placed first.
    qsort(population_fitnesses,n,sizeof(fitted_population_t),compare_fitness);

    assign_roulette_part(population_fitnesses,n);

    individual_picker(population_fitnesses, individuals_killed, amount_killed, n);

    //Generates new population to fill out killed individuals.
    //Creating from the surviving indivduals. FIX
    for(j=0; j < amount_killed; j++){
        random_week_1 = gene_rand_num(amount_living);
        random_week_2 = gene_rand_num(amount_living);

        //New mutated individual.
        if(random_week_1 < (n * MUTATION_PART)){
            mutator(individuals_killed[j], random_week_1, population_fitnesses);
        }
        //New individual
        else{
            create_new_individual(individuals_killed[j], random_week_1, random_week_2, population_fitnesses);
        }

    }

    free(population_fitnesses);
    free(individuals_killed);
}
void init_fitness_of_weeks(fitted_population_t *population_fitnesses,
                              struct week* population_pool, unsigned int n){
    int i;

    population_fitnesses = (fitted_population_t *)malloc(sizeof(fitted_population_t) * n);
    //Fills array with fitnesses.
    for(i=0; i < n; i++){
        population_fitnesses[i].week_fitness = fitness_of_week(&population_pool[i]);
        population_fitnesses[i].week_pointer = &population_pool[i];
    }
}

//Gives every individual a share of the roulette to be eliminated from.
void assign_roulette_part(fitted_population_t *population_fitnesses, unsigned int n){
    int i, total_fitness_of_weeks = 0;

    //Calculates the total fitness for this generation
    for (i = 0; i < n; i++) {
        total_fitness_of_weeks += population_fitnesses[i].week_fitness;
    }

    for (i = 0; i < n; i++) {
        population_fitnesses[i].roulette_part = 1/(population_fitnesses[i].week_fitness/
                                                   total_fitness_of_weeks);
    }
}

//Picks individuals with bigger value than the random number, which is constantly changed
//Individuals picked till amount_killed reached.
void individual_picker(fitted_population_t *population_fitnesses,
                       int *individuals_killed, int amount_killed,
                       unsigned int n){
    int i,j;
    double biggest_roulette_part = 0, selector = 0;

    biggest_roulette_part = population_fitnesses[n-1].roulette_part;

    for(i=0; i < amount_killed;){
        for(j=0; j < n; j++){
            selector = (double)gene_rand_num(biggest_roulette_part*1000000)/1000000; // FEJL, double og int problem
            if(selector < population_fitnesses[j].roulette_part)
                individuals_killed[i] = j;
                i++;
        }
    }
}

//sorts the array (most fit to least fit)
int compare_fitness(const void *a, const void *b){
    fitted_population_t *ca = (fitted_population_t *)a;
    fitted_population_t *cb = (fitted_population_t *)b;

        return cb->week_fitness - ca->week_fitness;
}

//Generates random number from 0 to input-1.
int gene_rand_num(int n){
    return rand()%n;
}

//Mutates individual by swapping 2 days in a random week chosen.
void mutator(int j, int rand_1, fitted_population_t *population_fitnesses){
    int d;
    struct week temporary_week;

    //intitialize and fills temporary week.
    for(d=0; d < DAYS_PR_WEEK; d++){
        temporary_week.days[d] = population_fitnesses[rand_1].week_pointer->days[d];
    }

    //Mutation
    temporary_week.days[DAY_TO_SWAP1] = population_fitnesses[rand_1].week_pointer->days[DAY_TO_SWAP2];
    temporary_week.days[DAY_TO_SWAP2] = population_fitnesses[rand_1].week_pointer->days[DAY_TO_SWAP1];

    //kills individual j and makes new individual, with swapped days.
    *population_fitnesses[j].week_pointer = temporary_week; //FIX week_pointer (evt fyld alle dage)
}

//Function makes new individual from 2 random weeks. //FIX Lav bedre udvælgelse af gener.
void create_new_individual(int j, int rand_1, int rand_2, fitted_population_t *population_fitnesses){
    int d, m;
    struct week temporary_week;

    //These loops mixes days and modules into a new week(individual).
    for(d=0; d < DAYS_PR_WEEK; d++){
        for(m=0; m < MODULES_PR_DAY; m++){
            if(0 <= d && d < 2){
                if(0 <= m && m < 3){
                    temporary_week.days[d].modules[m] = population_fitnesses[rand_1].week_pointer->days[d].modules[m];
                }
                else{
                    temporary_week.days[d].modules[m] = population_fitnesses[rand_2].week_pointer->days[d].modules[m];
                }
            }
            else{
                if(0 <= m && m <3){
                    temporary_week.days[d].modules[m] = population_fitnesses[rand_2].week_pointer->days[d].modules[m];
                }
                else{
                    temporary_week.days[d].modules[m] = population_fitnesses[rand_1].week_pointer->days[d].modules[m];
                }
            }
        }
    }
     //make new individual on element j, which is being discarded.
    *population_fitnesses[j].week_pointer = temporary_week; //FIX week_pointer (evt fyld alle dage)
}
