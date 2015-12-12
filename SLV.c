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
#include <math.h>

//Constants
#define TEACHER_IDENTIFIER_LENGTH (3+1)
#define CLASS_IDENTIFIER_LENGTH (4+1)
#define TEACHERS_PR_JOB (3)
#define JOBS_PR_MODULE (15)
#define MODULES_PR_DAY (6)
#define DAYS_PR_WEEK (5)
#define HARD_SUBJECTS (3)
#define MEDIUM_SUBJECTS (10)
#define SOFT_SUBJECTS (17)
#define EXTREME_REWARD (6)
#define BIG_REWARD (3)
#define MEDIUM_REWARD (2)
#define SMALL_REWARD (1)
#define MAX_Q_VALUE (100)

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
    //crafting,   //håndarbejde DEBUG Slettes evt.
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
int fitness_of_week(struct week* individual);
int fitness_function_mulitiple_lessons(struct week* individual, int d, int m, int j);
int fitness_function_module_time(struct week* individual, int d, int m, int j);
int fitness_function_no_free_space(struct week* individual, int d, int m, int j);
int fitness_function_day_length(struct week* individual);
int get_day_length(struct day *day);


//main function
int main(int argc, char *argv[]) {




    return 0;
}

//Tests the total fitness of a week.
//Returns this fitness
int fitness_of_week(struct week* individual) {
    int fitness_module_time = 0;                     //Placering af fag tidsmæssigt.
    int fitness_multiple_lessons = 0;                //To fag i streg.
    int fitness_no_free_space = 0;                   //Ingen hul timer.
    int fitness_day_length = 0;
    int d, m, j, total_fitness = 0;

    fitness_day_length = fitness_function_day_length(individual);

    for (d = 0; d < DAYS_PR_WEEK; d++) {
        for (m = 0; m < MODULES_PR_DAY; m++) {
            for (j = 0; j < JOBS_PR_MODULE; j++) {

                fitness_module_time += fitness_function_module_time(individual, d, m, j);

                fitness_multiple_lessons += fitness_function_mulitiple_lessons(individual, d, m, j);

                fitness_no_free_space += fitness_function_no_free_space(individual, d, m, j);
            }
        }
    }

    total_fitness = (fitness_day_length + fitness_module_time + fitness_multiple_lessons +
                    fitness_no_free_space);

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

    if(m >= MODULES_PR_DAY){
        return 0;
    }

    tested_module = &individual->days[d].modules[m];
    next_module = &individual->days[d].modules[m+1];

    //Loop tests if 2 subjects for the same class appears consecutively in 2 modules, either with
    //a break between the modules or without (hence m%2).
    for(i = 0; i < JOBS_PR_MODULE; i++){
        if (tested_module->jobs[j].subject == next_module->jobs[i].subject &&
            tested_module->jobs[j].class == next_module->jobs[i].class &&
            m % 2 == 0) {

            fitness_multiple_lessons += EXTREME_REWARD;
        }
        else if (tested_module->jobs[j].subject == next_module->jobs[i].subject &&
                tested_module->jobs[j].class == next_module->jobs[i].class &&
                m % 2 == 1) {

            fitness_multiple_lessons += BIG_REWARD;
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

//Function tests deviation from optimal day length in a week
int fitness_function_day_length(struct week* individual) {
    int d, day_length[DAYS_PR_WEEK];
    int sum_of_lengths = 0, week_points = 0;
    double avg_day_length = 0, q_value = 0; //q_value worst case is 44, and this value expresses
                                            //the deviation from the optimal day length this week

    for(d = 0; d < DAYS_PR_WEEK; d++){
        day_length[d] = get_day_length(&individual->days[d]);
        sum_of_lengths += day_length[d];
    }

    avg_day_length = sum_of_lengths / DAYS_PR_WEEK;

    for(d = 0; d < DAYS_PR_WEEK; d++){
        q_value += (pow((day_length[d] - avg_day_length),2) / avg_day_length);
    }

    if(q_value >= 33){
        week_points += 0;
    }
    else if(q_value >= 22 && q_value < 33){
        week_points += SMALL_REWARD;
    }
    else if(q_value >= 11 && q_value < 22){
        week_points += MEDIUM_REWARD;
    }
    else if(q_value < 11){
        week_points += BIG_REWARD;
    }

    return week_points;
}

//counts amount of non-free modules in a day.
int get_day_length(struct day *day){
    int m,j;
    int count;

    for(m = 0; m < MODULES_PR_DAY; m++){
        for(j = 0; j < JOBS_PR_MODULE; j++){
            if (strcmp(day->modules[m].jobs[j].teacher[0], "\0") != 0){
                count++; break;
            }
        }
    }
    return count;
}
