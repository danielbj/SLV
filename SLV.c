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



//main function
int main(int argc, char *argv[]) {
    
    
    
    
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
    int i;
    double amount_killed=0;
    struct week temporary;
    struct fitted_population *population_fitnesses;

    population_fitnesses = (struct fitted_population *)malloc(sizeof(fitted_population)*n);

    amount_killed = n*ELIMINATION_PART;

    //Fills array with fitnesses.
    for(i=0;i<n;i++){
        population_fitnesses[i].week_fitness = fitness_of_week(population_pool[i]);
        population_fitnesses[i].*week_pointer = population_pool[i];
    }

    qsort(population_fitnesses,n,sizeof(struct fitted_population),COMPAREFUNCTION);


    free(population_fitnesses);
    return;
}

int compare_fitness(const void *a, const void *b){
    struct fitted_population *ca = (struct fitted_population *)a;
    struct fitted_population *cb = (struct fitted_population *)b;
    
        return *ca->week_fitness - *cb->week_fitness;
}


//Function for finding fitness of one week.
int fitness_of_week(const struct week* individual) {
    return;
}



//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* population_pool) {
    return;
}















