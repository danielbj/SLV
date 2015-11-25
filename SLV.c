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

//Constants
#define TEACHER_IDENTIFIER_LENGTH (3+1)
#define CLASS_IDENTIFIER_LENGTH (4+1)
#define TEACHERS_PR_JOB (3)
#define JOBS_PR_MODULE (15)
#define MODULES_PR_DAY (6)
#define DAYS_PR_WEEK (5)
#define ELIMINATION_PART (0.5)
#define MUTATION_PCT 0.10
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
    enum subjects sec_subject;
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

struct fitted_population {
    int week_fitness;
    struct week *week_pointer;
};

//Function prototypes
struct week* initialize_weeks(int n);
void next_generation(struct week* population_pool, unsigned int n);
int fitness_of_week(const struct week* individual);
void print_fittest_week(const struct week* population_pool);
int compare_fitness(const void *a, const void *b);
struct fitted_population* init_array_fitness_weeks(struct week* population_pool, unsigned int n);
int gene_rand_num(int amount_killed);
void mutator(int j, int rand_1, struct fitted_population *population_fitnesses);
void create_new_individual(int j, int rand_1, int rand_2, struct fitted_population *population_fitnesses);

//main function
int main(int argc, char *argv[]) {
    
srand(1);    
    
    
    return 0;
}



//Input + generation + sanitation
//Initialize n weeks with jobs from jobs.txt
struct week* initialize_weeks(int n) {
    return;
}



//Function that generates a new population.
//This function overwrites the initial population (thus returning void).
void next_generation(struct week* population_pool, unsigned int n) {
    int j, random_number_1=-123, random_number_2=-123,
        amount_killed=0;
    struct fitted_population *population_fitnesses;

    population_fitnesses = init_array_fitness_weeks(population_pool,n);

    amount_killed = n*ELIMINATION_PART;

    //sorts the array, so that the worst individuals are placed first.
    qsort(population_fitnesses,n,sizeof(struct fitted_population),compare_fitness);

    //Generates new population to fill out killed individuals.
    //Creating from the surviving indivduals.
    for(j=0; j < amount_killed; j++){
        random_number_1 = gene_rand_num(amount_killed); 
        random_number_2 = gene_rand_num(amount_killed);

        if(random_number_1 < (n * MUTATION_PCT) + amount_killed){ //New mutated individual.
            mutator(j, random_number_1, population_fitnesses);
        }
        else{ //New individual
            create_new_individual(j, random_number_1, random_number_2, population_fitnesses);
        }

    }

    free(population_fitnesses);
}

int compare_fitness(const void *a, const void *b){
    struct fitted_population *ca = (struct fitted_population *)a;
    struct fitted_population *cb = (struct fitted_population *)b;
    
        return ca->week_fitness - cb->week_fitness;
}

struct fitted_population *init_array_fitness_weeks(struct week* population_pool, unsigned int n){
    int i;
    struct fitted_population *population_fitnesses;

    population_fitnesses = (struct fitted_population *)malloc(sizeof(struct fitted_population) * n);
    //Fills array with fitnesses.
    for(i=0; i < n; i++){
        population_fitnesses[i].week_fitness = fitness_of_week(&population_pool[i]); 
        population_fitnesses[i].week_pointer = &population_pool[i];
    }

    return population_fitnesses;
}

//Generates random number among the weeks that wont be killed.
int gene_rand_num(int amount_killed){
    return rand()%amount_killed + amount_killed;
}

void mutator(int j, int rand_1, struct fitted_population *population_fitnesses){
    int d;
    struct week temporary_week;

    //intitialize and fills temporary week.
    for(d=0; d < DAYS_PR_WEEK; d++){
        temporary_week.days[d] = population_fitnesses[rand_1].week_pointer->days[d];
    }

    //Mutation
    temporary_week.days[DAY_TO_SWAP1] = population_fitnesses[rand_1].week_pointer->days[DAY_TO_SWAP2];
    temporary_week.days[DAY_TO_SWAP2] = population_fitnesses[rand_1].week_pointer->days[DAY_TO_SWAP1];
    
    //make new individual consisting of a good week, with swapped days.
    population_fitnesses[j].week_pointer[0] = temporary_week; //FIX week_pointer (evt fyld alle dage)
}

void create_new_individual(int j, int rand_1, int rand_2, struct fitted_population *population_fitnesses){
    int d, m;
    struct week temporary_week;

    //These loops mixes days and modules into a new week.
    for(d=0; d < DAYS_PR_WEEK; d++){
        for(m=0; m < MODULES_PR_DAY; m++){ 
            if(0 <= d && d < 2){
                if(0 <= m && m <3){
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
    population_fitnesses[j].week_pointer[0] = temporary_week; //FIX
}

//Function for finding fitness of one week.
int fitness_of_week(const struct week* individual) {
    return;
}



//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* population_pool) {
    return;
}
















