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
#include "assert.h"

//Constants
#define TEACHER_IDENTIFIER_LENGTH (3+1)
#define CLASS_IDENTIFIER_LENGTH (4+1)
#define TEACHERS_PR_JOB (3)
#define JOBS_PR_MODULE (17)
#define MODULES_PR_DAY (11)
#define DAYS_PR_WEEK (5)

#define MAX_SUBJECT_CHARS (20)
#define EXPECTED_MIN_SCANS (4)
#define MAX_LINE_LENGTH (50)
#define JOB_FILE_NAME "jobs.txt"
#define SCHEDULE_FILE_NAME "SKEMA.txt"

#define ELIMINATION_PART (0.5)
#define MUTATION_PART 0.10

#define HARD_SUBJECTS (3)
#define MEDIUM_SUBJECTS (10)
#define SOFT_SUBJECTS (17)
#define EXTREME_REWARD (6)
#define BIG_REWARD (3)
#define MEDIUM_REWARD (2)
#define SMALL_REWARD (1)

#define FITNESS_MODULE_TIME_BASE (1400)
#define FITNESS_MODULE_TIME_FACTOR (1.3)
#define FITNESS_MULTIPLE_LESSONS_BASE (0)
#define FITNESS_MULTIPLE_LESSONS__FACTOR (1)
#define FITNESS_NO_FREE_SPACE_BASE (350)
#define FITNESS_NO_FREE_SPACE_FACTOR (1)



//Enumerations
enum subjects {
    //Hard Subjects - 3
    math,       //matematik
    danish,     //dansk
    english,    //engelsk
    german,     //tysk

    //Medium subjects - 10
    physics,    //fysik
    nature,     //natur-teknik
    biology,    //biologi
    history,    //historie
    religion,   //kristendomskundskab
    geography,  //geografi
    socialstud, //samfundsfag

    //Soft subjects - 17
    classtime,  //klassens time
    art,        //billedkunst
    music,      //musik
    cooking,    //hjemkundskab
    woodwork,   //sløjd
    phys_ed,    //idræt
    elective,    //valgfag

    //Special subjects - 18
    prep        //forberedelsestid
};

//Structures
struct job {
    char teacher[TEACHERS_PR_JOB][TEACHER_IDENTIFIER_LENGTH];
    char class[CLASS_IDENTIFIER_LENGTH];
    enum subjects subject;
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
struct week* initialize_weeks(int n);
struct job* read_jobs(void);
int read_job(struct job* job, FILE* file);
struct job* job_counter(FILE* file);
enum subjects translate_subject(char temp_subject[]);

struct week* insert_jobs(struct job* job_pool, int number_of_weeks);
int generate_week(struct week* week, struct job* job_pool);
int insert_job_in_week(struct job* job, struct week* week, int day, int module);
int insert_job_in_module(struct job* job, struct module* module);
int is_empty_job(struct job* job);
int is_teacher_conflict(struct module* module, struct job* job);
int is_class_conflict(struct module* module, struct job* job);

void next_generation(struct week* population_pool, unsigned int n);
int compare_fitness(const void *a, const void *b);
fitted_population_t* init_fitness_of_weeks(struct week* population_pool, unsigned int n);
void assign_roulette_part(fitted_population_t *population_fitnesses, unsigned int n);
void individual_picker(fitted_population_t *population_fitnesses,
                       int *individuals_killed, int amount_killed,
                       unsigned int n);
int check_not_picked(int *individuals_killed, int j, int i);
int gene_rand_num(int amount_killed);
void mutator(int j, int random_week, fitted_population_t *population_fitnesses);
void create_new_individual(int j, int random_week, fitted_population_t *population_fitnesses);

int fitness_of_week(struct week* individual);
int fitness_function_mulitiple_lessons(struct week* individual, int d, int m, int j);
int fitness_function_module_time(struct week* individual, int d, int m, int j);
int fitness_function_no_free_space(struct week* individual, int d, int m, int j);

void print_week(const struct week* fittest_week, char* teacher);
void print_module(const struct module* module, char* teacher, FILE* out_ptr);
void print_subject(enum subjects subject, FILE* out_ptr);



//Main function
int main(int argc, char *argv[]) {
    int i, g;
    struct week* week_pool;
    fitted_population_t *population_fitnesses;

    printf("START!\n");

    //Create a pool of random weeks
    week_pool = initialize_weeks(100);
    assert(week_pool != 0);

    print_week(&week_pool[0], "LC");

    scanf("%d", &g);
    //Make ONE new generation
    for(i=0; i<g; i++){
      next_generation(week_pool, 100);
    }

    printf("SO FAR SO GOOD\n");


    population_fitnesses = init_fitness_of_weeks(week_pool, 100);

    print_week(population_fitnesses[0].week_pointer, "LC");


    printf("\nDONE!\n");
    return 0;
}



//Input + generation + sanitation
//Initialize n weeks with jobs from jobs.txt
//Returns NULL if error.
struct week* initialize_weeks(int number_of_weeks) {
    struct job* job_pool = 0;
    struct week* week_pool = 0;

    job_pool = read_jobs();
    assert(job_pool != 0);

    week_pool = insert_jobs(job_pool, number_of_weeks);
    assert(week_pool != 0);

    return week_pool;
}

//Function for reading all jobs from a file
//Generated job pool ends with empty job.
//Returns NULL if error.
struct job* read_jobs(void) {
    int job = 0;//job pool index
    int done = 0;//while-loop condition flag
    int n_insertions;//number of insertions

    FILE* file = fopen(JOB_FILE_NAME, "r");
    if (!file) {
        return 0;
    }

    struct job* job_pool = job_counter(file);
    if (!job_pool) {
        fclose(file);
        return 0;
    }

    while (!done) {
        n_insertions = read_job(&job_pool[job], file);
        if (!n_insertions) {
            done = 1;
        } else {
            job += n_insertions;
        }
    }

    printf("Jobs inserted %d \n", job); //debug

    //Insert empty job
    strcpy(job_pool[job].teacher[0], "\0");

    fclose(file);
    return job_pool;
}

//Function for reading one job from a file.
//Asserts on error. Else, returns number of read jobs.
int read_job(struct job* job, FILE* file) {
    int i;//loop counter
    int status = 0;
    int n_insertions;
    char buffer[MAX_LINE_LENGTH];
    struct job temp_job;
    char temp_subject[MAX_SUBJECT_CHARS];

    //Nullify teachers
    for (i = 0; i < TEACHERS_PR_JOB; i++) {
        strcpy(temp_job.teacher[i],"\0");
    }

    //Read one line from the file
    if (fgets(buffer, MAX_LINE_LENGTH, file) == 0) {
        return 0;
    }

    status = sscanf(buffer, "%d %s %s %s %s %s",
        &n_insertions,
        temp_subject,
        temp_job.class,
        temp_job.teacher[0],
        temp_job.teacher[1],
        temp_job.teacher[2]
    );

    //Check if EOF
    assert(status >= EXPECTED_MIN_SCANS);

    //Translate the subject to enum
    temp_job.subject = translate_subject(temp_subject);

    for (i = 0; i < n_insertions; i++) {
        job[i] = temp_job;
    }

    return n_insertions;
}

//For calculating the size of the job pool
//Returns NULL on error.
struct job* job_counter(FILE* file) {
    int n_jobs = 0;
    int new;
    char buffer[MAX_LINE_LENGTH];

    while(fgets(buffer, MAX_LINE_LENGTH, file)) {
        sscanf(buffer, "%d", &new);
        n_jobs += new;
    }

    printf("Jobs counted %d\n", n_jobs); //Debug

    struct job* job_pool = malloc(sizeof(struct job) * (n_jobs+1));
    assert(job_pool != 0);

    rewind(file);

    return job_pool;
}

//translate from char to enum
enum subjects translate_subject(char temp_subject[]) {
    enum subjects trans_subject;

    if(strncmp(temp_subject, "Billedkunst", 4) == 0) {
        trans_subject = art;
    }
    else if(strncmp(temp_subject, "Biologi", 4) == 0) {
        trans_subject = biology;
    }
    else if(strncmp(temp_subject, "Dansk", 4) == 0) {
        trans_subject = danish;
    }
    else if(strncmp(temp_subject, "Engelsk", 4) == 0) {
        trans_subject = english;
    }
    else if(strncmp(temp_subject, "Fysik/kemi", 4) == 0) {
        trans_subject = physics;
    }
    else if(strncmp(temp_subject, "Geografi", 4) == 0) {
        trans_subject = geography;
    }
    else if(strncmp(temp_subject, "Historie", 4) == 0) {
        trans_subject = history;
    }
    else if(strncmp(temp_subject, "Idraet", 4) == 0) {
        trans_subject = phys_ed;
    }
    else if(strncmp(temp_subject, "Klassenstid", 4) == 0) {
        trans_subject = classtime;
    }
    else if(strncmp(temp_subject, "Kristendom", 4) == 0) {
        trans_subject = religion;
    }
    else if(strncmp(temp_subject, "Madkundskab", 4) == 0) {
        trans_subject = cooking;
    }
    else if(strncmp(temp_subject, "Matematik", 4) == 0) {
        trans_subject = math;
    }
    else if(strncmp(temp_subject, "Musik", 4) == 0) {
        trans_subject = music;
    }
    else if(strncmp(temp_subject, "Natur/teknik", 4) == 0) {
        trans_subject = nature;
    }
    else if(strncmp(temp_subject, "Samfundsfag", 4) == 0) {
        trans_subject = socialstud;
    }
    else if(strncmp(temp_subject, "Sloejd", 4) == 0) {
        trans_subject = woodwork;
    }
    else if(strncmp(temp_subject, "Tysk", 4) == 0) {
        trans_subject = german;
    }
    else if(strncmp(temp_subject, "Valghold", 4) == 0) {
        trans_subject = elective;
    }
    else if(strncmp(temp_subject, "Forberedelsestid", 4) == 0) {
        trans_subject = prep;
    }

    return trans_subject;
}

//Function for generating a pool of weeks, by randomly inserting jobs, respecting teacher conflicts.
//Returns NULL on error.
struct week* insert_jobs(struct job* job_pool, int number_of_weeks) {
    int i;//loop counter

    //Allocate memory for pool of generated weeks
    struct week* week_pool = calloc(number_of_weeks, sizeof(struct week));
    assert(week_pool != 0);

    //seed random number generator
    srand(time(0));//Move to main?!

    //Generate n weeks
    for (i = 0; i < number_of_weeks; i++) {
        //printf("Week %d ", i); //Debug

        if (generate_week(&week_pool[i], job_pool) == 0) {
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

    //Debug
    //printf("JOB POOL INDEX: %d: INSERTED\n", i);

    return 1;
}

//Function for placing a job in a week.
//The job will be inserted in the first free module, where no teacher conflicts appears.
//Returns NULL if error.
int insert_job_in_week(struct job* job, struct week* week, int day, int module) {
    int d_day, d_module;//Delta-values for offsetting day and module.
    int e_day, e_module;//For storing the effective day and module indexes.
    struct module* current_module = 0;//For holding the module currently being tested for insertion.


    for (d_day = 0; d_day < DAYS_PR_WEEK; d_day++) {
        for (d_module = 0; d_module < MODULES_PR_DAY; d_module++) {
            e_day = (day + d_day) % DAYS_PR_WEEK;
            e_module = (module + d_module) % MODULES_PR_DAY;
            current_module = &week->days[e_day].modules[e_module]; //CONFLIC e_day = 0 & e_module = 1 se længere nede
            if (insert_job_in_module(job, current_module)) {
                return 1;
            }
        }
    }
    printf("Couldnt place following job: teacher %s & %s & %s and class %s \n",
            job->teacher[0], job->teacher[1], job->teacher[2], job->class);
    return 0;
}

//Inserts a job in a module. Will respect teacher and class conflicts.
//Returns NULL if error.
int insert_job_in_module(struct job* job, struct module* module) {
    int i;//loop counter

    //Check for teacher and class conflicts.
    if (is_class_conflict(module, job) || is_teacher_conflict(module, job)) {
        return 0;
    }

    //Insert job in first free job entry
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
    return (strcmp(job->teacher[0],"\0") == 0);
}

//Function for checking for teacher conflicts in a module.
//Returns !NULL if conflict.
int is_teacher_conflict(struct module* module, struct job* job) {
    int d_job;//Job index offset
    int d_teacher;//Teacher index offset
    int d_job_teacher;//New teacher index offset

    for (d_job_teacher = 0; d_job_teacher < TEACHERS_PR_JOB; d_job_teacher++) {
        for (d_job = 0; d_job < JOBS_PR_MODULE; d_job++) {
            for (d_teacher = 0; d_teacher < TEACHERS_PR_JOB; d_teacher++) {
                if (strcmp(module->jobs[d_job].teacher[d_teacher], "\0") != 0) {
                    if (strcmp(module->jobs[d_job].teacher[d_teacher],
                                job->teacher[d_job_teacher]) == 0) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

//Function for checking for class conflicts in a module.
//Returns !NULL if conflict.
int is_class_conflict(struct module* module, struct job* job) {
    int d_job;//job index offset

    for (d_job = 0; d_job < JOBS_PR_MODULE; d_job++) {
        if (module->jobs[d_job].subject != prep) {
            if(!strcmp(module->jobs[d_job].class, job->class)) {
                return 1;
            }
        }
    }

    return 0;
}



//Function that generates a new population.
//This function overwrites the initial population (thus returning void).
void next_generation(struct week* population_pool, unsigned int n) {
    int j, random_week,
        amount_killed=0, amount_living=0, random_decider = 0;
    int *individuals_killed = 0;
    fitted_population_t *population_fitnesses = 0;

    amount_killed = n * ELIMINATION_PART;
    amount_living = n - amount_killed;

    population_fitnesses = init_fitness_of_weeks(population_pool, n);

    individuals_killed = malloc(sizeof(int)*amount_killed);
    assert(individuals_killed != NULL);

    assign_roulette_part(population_fitnesses,n);

    individual_picker(population_fitnesses, individuals_killed, amount_killed, n);

    //DEBUG
    //printf("Amount Living: %d\n", amount_living);


    //Generates new population to fill out killed individuals.
    //Creating from the surviving indivduals.
    for(j=0; j < amount_killed; j++){
        //Picks the random weeks among all weeks to be parents of new child
        random_week = gene_rand_num(amount_living);
        random_decider = gene_rand_num(n);

        //DEBUG
        //printf("J: %d ", j);
        //printf("random week: %d\n", random_week_1);

        //New mutated individual.
        if(random_decider < (n * MUTATION_PART)){
            mutator(individuals_killed[j], random_week, population_fitnesses);
        }

        //New individual
        else{
            create_new_individual(individuals_killed[j], random_week, population_fitnesses);
        }

    }



    free(population_fitnesses);
    free(individuals_killed);
}

fitted_population_t* init_fitness_of_weeks(struct week* population_pool, unsigned int n){
    int i;

    //Allocate space for required array
    fitted_population_t* population_fitnesses = malloc(sizeof(fitted_population_t) * n);
    assert(population_fitnesses != 0);

    //Fills array with fitnesses.
    for(i=0; i < n; i++){
        population_fitnesses[i].week_fitness = fitness_of_week(&population_pool[i]);
        population_fitnesses[i].week_pointer = &population_pool[i];
    }

    //sorts the array, so that the fittest individuals are placed first.
    qsort(population_fitnesses, n, sizeof(fitted_population_t), compare_fitness);

    return population_fitnesses;
}

//Gives every individual a share of the roulette to be eliminated from.
void assign_roulette_part(fitted_population_t *population_fitnesses, unsigned int n){
    int i, total_fitness_of_weeks = 0;

    //Calculates the total fitness for this generation
    for (i = 0; i < n; i++) {
        total_fitness_of_weeks += population_fitnesses[i].week_fitness;
    }

    printf("Total fitness for the generation: %d\n", total_fitness_of_weeks);

    //Calculates roulette part for each individual
    for (i = 0; i < n; i++) {
        population_fitnesses[i].roulette_part = 1/((double)population_fitnesses[i].week_fitness/
                                                  (double)total_fitness_of_weeks);
    }
}

//Picks individuals with bigger value than the random number, which is constantly changed
//Individuals picked till amount_killed reached.
void individual_picker(fitted_population_t *population_fitnesses,
                       int *individuals_killed, int amount_killed,
                       unsigned int n){
    int i,j;
    double biggest_roulette_part = 0, selector = 0,
           smallest_roulette_part = 0, difference_roulette_part = 0;

    biggest_roulette_part = population_fitnesses[n-1].roulette_part;
    smallest_roulette_part = population_fitnesses[0].roulette_part;
    difference_roulette_part = biggest_roulette_part - smallest_roulette_part;

    j=0; i=0;
   while(i < amount_killed){
       if(j%n == 0){
               j=0;
           }
        for(; j < n; j++){
            selector = (double)gene_rand_num(difference_roulette_part) + smallest_roulette_part; // TODO: FEJL, double og int problem fix så interval er [mindst,biggest]
            if(selector < population_fitnesses[j].roulette_part &&
                check_not_picked(individuals_killed, j, i) == 1){
                individuals_killed[i] = j;
                i++; j++; break;
            }
        }
    }
}

//Returns 0 if the individual is picked and 1 if it isn't already picked
int check_not_picked(int *individuals_killed, int j, int i){
    int k;

    for (k = 0; k < i; k++) {
        if(individuals_killed[k] == j)
            return 0;
    }

    return 1;
}
//sorts the array (most fit to least fit)
int compare_fitness(const void *a, const void *b){
    fitted_population_t *ca = (fitted_population_t *)a;
    fitted_population_t *cb = (fitted_population_t *)b;

    return cb->week_fitness - ca->week_fitness;
}

//Generates random number from 0 to input-1.
int gene_rand_num(int n){
    if(n==0){
        return 0;
    }
    return rand()%n;
}

//Mutates individual by swapping 2 days in a random week chosen.
void mutator(int j, int random_week, fitted_population_t *population_fitnesses){
    int d, day_to_swap1 = 0, day_to_swap2 = 0;
    struct week temporary_week;

    day_to_swap1 = gene_rand_num(5);
    day_to_swap2 = gene_rand_num(5);

    temporary_week = *population_fitnesses[random_week].week_pointer;

    //intitialize and fills temporary week.
    for(d=0; d < DAYS_PR_WEEK; d++){
        temporary_week.days[d] = population_fitnesses[random_week].week_pointer->days[d];
    }

    //Mutation
    temporary_week.days[day_to_swap1] = population_fitnesses[random_week].week_pointer->days[day_to_swap2];
    temporary_week.days[day_to_swap2] = population_fitnesses[random_week].week_pointer->days[day_to_swap1];

    //kills individual j and makes new individual, with swapped days.
    *population_fitnesses[j].week_pointer = temporary_week;

    printf("Random Mutate: %d\n", random_week);//Debug

    return;
}

//Function makes new individual from 2 random weeks.
void create_new_individual(int j, int random_week, fitted_population_t *population_fitnesses){
    int d, m;
    struct week temporary_week;

    //DEBUG TODO: MIX DAYS OTHERWISE
    
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
    *population_fitnesses[j].week_pointer = temporary_week;
}



//Tests the total fitness of a week.
//Returns this fitness
int fitness_of_week(struct week* individual) {
    int fitness_module_time = 0;                     //Placering af fag tidsmæssigt.
    int fitness_multiple_lessons = 0;                //To fag i streg.
    int fitness_no_free_space = 0;                   //Ingen hul timer.
    int d, m, j, total_fitness = 0;

    for (d = 0; d < DAYS_PR_WEEK; d++) {
        for (m = 0; m < MODULES_PR_DAY; m++) {
            for (j = 0; j < JOBS_PR_MODULE; j++) {

                fitness_module_time += fitness_function_module_time(individual, d, m, j);

                fitness_multiple_lessons += fitness_function_mulitiple_lessons(individual, d, m, j);

                fitness_no_free_space += fitness_function_no_free_space(individual, d, m, j);
            }
        }
    }

    //Align fitness values according to base and factor
    fitness_module_time = (fitness_module_time - FITNESS_MODULE_TIME_BASE) * FITNESS_MODULE_TIME_FACTOR;
    fitness_multiple_lessons = (fitness_multiple_lessons - FITNESS_MULTIPLE_LESSONS_BASE) * FITNESS_MULTIPLE_LESSONS__FACTOR;
    fitness_no_free_space = (fitness_no_free_space - FITNESS_NO_FREE_SPACE_BASE) * FITNESS_NO_FREE_SPACE_FACTOR;

    total_fitness = (fitness_module_time + fitness_multiple_lessons + fitness_no_free_space);

    //DEBUG
    //printf("%-4d\t", fitness_module_time);
    //printf("%-4d\t", fitness_multiple_lessons);
    //printf("%-4d\t", fitness_no_free_space);
    //printf("total fitness: %-5d\n", total_fitness);



    return total_fitness;
}

//Funcion for testing a modules placement on a day,
//while considering the difficulty of the subject taught.
int fitness_function_module_time(struct week* individual, int d, int m, int j) {
    int fitness_modules_time = 0;
    enum subjects tested_subject = 0;

    tested_subject = individual->days[d].modules[m].jobs[j].subject;

    //Tests for placement of preperation time in the day
    if (tested_subject == prep) {
        if(m < 2) {
            fitness_modules_time += SMALL_REWARD;
        }
        else if (m < 4) {
            fitness_modules_time += MEDIUM_REWARD;
        }
        else if (m < 8) {
            fitness_modules_time += BIG_REWARD;
        }
        else {
            fitness_modules_time += EXTREME_REWARD;
        }
    }

    //Tests for hard subjects and placement on the day
    if (tested_subject <= HARD_SUBJECTS){
        if(m < 2) {
            fitness_modules_time += BIG_REWARD;
        }
        else if (m < 4) {
            fitness_modules_time += MEDIUM_REWARD;
        }
        else if (m < 8) {
            fitness_modules_time += SMALL_REWARD;
        }
    }

    //Tests for medium difficulty subjects and placement on the day
    if (tested_subject > HARD_SUBJECTS && tested_subject <= MEDIUM_SUBJECTS) {
        if(m < 2){
            fitness_modules_time += MEDIUM_REWARD;
        }
        else if(m < 4){
            fitness_modules_time += BIG_REWARD;
        }
        else if(m < 8){
            fitness_modules_time += SMALL_REWARD;
        }
    }

    //Tests for a soft subject and placement on the day
    if (tested_subject > MEDIUM_SUBJECTS && tested_subject <= SOFT_SUBJECTS){
        if(m < 2) {
            fitness_modules_time += SMALL_REWARD;
        }
        else if (m < 4) {
            fitness_modules_time += MEDIUM_REWARD;
        }
        else if (m < 8) {
            fitness_modules_time += BIG_REWARD;
        }
    }

    return fitness_modules_time;
}

//Funtion for testing the event of 2 lessons in a row, and reward such case.
int fitness_function_mulitiple_lessons(struct week* individual, int d, int m, int j){
    int i, fitness_multiple_lessons = 0;
    struct module* tested_module = 0;
    struct module* next_module = 0;

    //Test if exceeding last module for a day
    if(m >= MODULES_PR_DAY){
        return 0;
    }

    tested_module = &individual->days[d].modules[m];
    next_module = &individual->days[d].modules[m+1];

    //Loop tests if 2 subjects for the same class appears consecutively in 2 modules, either with
    //a break between the modules or without (hence m%2).
    for(i = 0; i < JOBS_PR_MODULE; i++){
        if (tested_module->jobs[j].subject == next_module->jobs[i].subject &&
            (strcmp(tested_module->jobs[j].class, next_module->jobs[i].class) == 0) &&
            tested_module->jobs[j].subject != prep &&
            (strcmp(tested_module->jobs[j].teacher[0], "\0") != 0) &&
            m % 2 == 0) {

            fitness_multiple_lessons += EXTREME_REWARD; break;
        }
        else if (tested_module->jobs[j].subject == next_module->jobs[i].subject &&
                (strcmp(tested_module->jobs[j].class, next_module->jobs[i].class) == 0) &&
                tested_module->jobs[j].subject != prep &&
                (strcmp(tested_module->jobs[j].teacher[0], "\0") != 0) &&
                m % 2 == 1) {

            fitness_multiple_lessons += BIG_REWARD; break;
        }
    }

    return fitness_multiple_lessons;
}

//Functions tests for space between modules on a day.
int fitness_function_no_free_space(struct week* individual, int d, int m, int j) {
    int fitness_no_free_space = 0;

    //Tests for a the presence of a teacher.
    if (strcmp(individual->days[d].modules[m].jobs[j].teacher[0], "\0") == 0){
        if (m >= 8){
            fitness_no_free_space += BIG_REWARD;
        }
        else{
            fitness_no_free_space += 0;
        }
    }

    return fitness_no_free_space;
}



//Output the fittest of weeks in the population pool.
void print_week(const struct week* fittest_week, char* teacher) {
    int d,m;

    FILE* out_ptr = fopen(SCHEDULE_FILE_NAME, "w");
    assert(out_ptr != 0);

    fprintf(out_ptr, "Skemaet for %s:\n\n", teacher);
    fprintf(out_ptr, "%-13s%-25s%-25s%-25s%-25s%-25s\n", " ", "MAN", "TIR", "ONS", "TOR", "FRE\n");

    //Loop checks every day, module and job for the teacher to print schedule for
    for(m=0; m < MODULES_PR_DAY; m++) {
        fprintf(out_ptr, "LEKTION %-2d   ", m+1);
        for(d=0; d < DAYS_PR_WEEK; d++) {
            print_module(&fittest_week->days[d].modules[m], teacher, out_ptr);
        }
        fprintf(out_ptr, "\n\n");
    }

    fclose(out_ptr);
}

//Function for printing one module for a teacher.
//Prints empty module if no teacher matched.
void print_module(const struct module* module, char* teacher, FILE* out_ptr) {
    int j, t, found = 0;

    for (j = 0; j < JOBS_PR_MODULE; j++) {
        for (t = 0; t < TEACHERS_PR_JOB; t++) {
            if (!strcmp(teacher, module->jobs[j].teacher[t])) {
                found = 1;
                print_subject(module->jobs[j].subject, out_ptr);
                fprintf(out_ptr, "%-5s", module->jobs[j].class);
            }
        }
    }

    if (!found) {
        fprintf(out_ptr, "%-25s", "-");
    }
    return;
}

//Function for printing the name of a subject
void print_subject(enum subjects subject, FILE* out_ptr) {
    switch (subject) {
        case art:
            fprintf(out_ptr, "%-20s", "billedkunst"); break;
        case biology:
            fprintf(out_ptr, "%-20s", "biologi"); break;
        case danish:
            fprintf(out_ptr, "%-20s", "dansk"); break;
        case english:
            fprintf(out_ptr, "%-20s", "engelsk"); break;
        case physics:
            fprintf(out_ptr, "%-20s", "fysik"); break;
        case geography:
            fprintf(out_ptr, "%-20s", "geografi"); break;
        case history:
            fprintf(out_ptr, "%-20s", "historie"); break;
        case phys_ed:
            fprintf(out_ptr, "%-20s", "idraet"); break;
        case classtime:
            fprintf(out_ptr, "%-20s", "klassens time"); break;
        case religion:
            fprintf(out_ptr, "%-20s", "kristendomskundskab"); break;
        case cooking:
            fprintf(out_ptr, "%-20s", "hjemkundskab"); break;
        case math:
            fprintf(out_ptr, "%-20s", "matematik"); break;
        case music:
            fprintf(out_ptr, "%-20s", "musik"); break;
        case nature:
            fprintf(out_ptr, "%-20s", "natur-teknik"); break;
        case socialstud:
            fprintf(out_ptr, "%-20s", "samfundsfag"); break;
        case woodwork:
            fprintf(out_ptr, "%-20s", "sloejd"); break;
        case german:
            fprintf(out_ptr, "%-20s", "tysk"); break;
        case elective:
            fprintf(out_ptr, "%-20s", "valgfag"); break;
        case prep:
            fprintf(out_ptr, "%-20s", "forberedelsestid"); break;
    }

    return;
}
