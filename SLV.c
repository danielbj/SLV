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

//Function prototypes
struct week* initialize_weeks(int n);

struct week* insert_jobs(struct job* job_pool, int number_of_weeks);
int generate_week(struct week* week, struct job* job_pool);
int insert_job_in_week(struct job* job, struct week* week, int day, int module);
int insert_job_in_module(struct job* job, struct module* module);
int is_empty_job(struct job* job);
int is_teacher_conflict(struct module* module, struct job* job);
int is_class_conflict(struct module* module, struct job* job);

void next_generation(struct week* population_pool, unsigned int n);
int fitness_of_week(const struct week* individual);
void print_fittest_week(const struct week* population_pool);



//main function
int main(int argc, char *argv[]) {




    return 0;
}



//Input + generation + sanitation
//Initialize n weeks with jobs from jobs.txt
//Returns NULL if error.
struct week* initialize_weeks(int number_of_weeks) {

    //FUNCTION: Read data from file to local structs




    //FUNCTION: Randomly insert, regarding hard limits



    return 0;
}



//Function for reading data from jobs.txt to jobs structure.
//Insures that last element is NULL (teacher == 0).
struct job* read_jobs() {

    return 0;
}



//Function for generating a pool of weeks, by randomly inserting jobs, respecting teacher conflicts.
//Returns NULL on error.
struct week* insert_jobs(struct job* job_pool, int number_of_weeks) {
    int i;//loop counter

    //Allocate memory for pool of generated weeks
    struct week* week_pool = calloc(number_of_weeks, sizeof(struct week));
    if (!week_pool) {
        return 0;
    }

    //seed random number generator
    srand(time(0));

    //Generate n weeks
    for (i = 0; i < number_of_weeks; i++) {
        if (!generate_week(&week_pool[i], job_pool)) {
            return 0;
        }
    }

    //Free the job pool
    free(job_pool);

    //Return pool of weeks
    return week_pool;
}



//Function for randomly generating one week from a pool of jobs.
//Returns NULL if error.
int generate_week(struct week* week, struct job* job_pool) {
    int random_day, random_module;//For storing random indexes
    int i = 0;//job pool index

    while (!is_empty_job(&job_pool[i])) {
        random_day = rand() % DAYS_PR_WEEK;//Select a random day
        random_module = rand() % MODULES_PR_DAY;//Select a random module

        if (!insert_job_in_week(&job_pool[i], week, random_day, random_module)) {
            return 0;
        } else {
            i++;
        }
    }

    return 1;
}


//Function for placing a job in a week.
//The job will be inserted in the first free module, where no teacher conflicts appears.
//Returns NULL if error.
int insert_job_in_week(struct job* job, struct week* week, int day, int module) {
    int d_day, d_module;//Delta-values for offsetting day and module.
    struct module* current_module;//For holding the module currently being tested for insertion.

    for (d_day = 0; d_day < DAYS_PR_WEEK; d_day++) {
        for (d_module = 0; d_module < MODULES_PR_DAY; d_module++) {
            current_module = &week->days[(day + d_day) % DAYS_PR_WEEK].modules[(module + d_module) % MODULES_PR_DAY];
            if (insert_job_in_module(job, current_module)) {
                return 1;
            }
        }
    }

    return 0;
}


//Inserts a job in a module. Will respect teacher and class conflicts.
//Returns NULL if error.
int insert_job_in_module(struct job* job, struct module* module) {

    int i;//loop counter

    //Check for teacher and class conflicts.
    if (is_teacher_conflict(module, job) || is_class_conflict(module, job)) {
        return 0;
    }

    //Insert job in first free entry
    for (i = 0; i < JOBS_PR_MODULE; i++) {
        if (is_empty_job(&module->jobs[i])) {
            module->jobs[i] = *job;
            return 1;
        }
    }

    return 0;
}



//Function for checking if a job is empty.
//Returns !NULL if empty.
int is_empty_job(struct job* job) {
    return job->teacher[0][0] == 0;
}



//Function for checking for teacher conflicts in a module.
//Returns !NULL if conflict.
int is_teacher_conflict(struct module* module, struct job* job) {
    int d_job;//Job index offset
    int d_teacher;//Teacher index offset

    for (d_job = 0; d_job < JOBS_PR_MODULE; d_job++) {
        for (d_teacher = 0; d_teacher < TEACHERS_PR_JOB; d_teacher++) {
            if () {
                return 1;
            }
        }
    }

    return 0;
}



//Function for checking for class conflicts in a module.
//Returns !NULL if conflict.
int is_class_conflict(struct module* module, struct job* job) {
    int d_job;//job index offset

    for (d_day = 0; d_day < JOBS_PR_MODULE; d_day++) {
        //TODO
    }
}












//Function that generates a new population.
//This function overwrites the initial population (thus returning void).
void next_generation(struct week* population_pool, unsigned int n) {
    return;
}



//Function for finding fitness of one week.
int fitness_of_week(const struct week* individual) {
    return 0;
}



//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* population_pool) {
    return;
}
