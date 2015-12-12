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
#define HARD_SUBJECTS (4)
#define MEDIUM_SUBJECTS (7)
#define SOFT_SUBJECTS (8)

//Enumerations
enum subjects {
    //Hard Subjects - 4 
    math,       //matematik
    danish,     //dansk
    english,    //engelsk
    german,     //tysk

    //Medium subjects - 7
    physics,    //fysik
    nature,     //natur-teknik
    biology,    //biologi
    history,    //historie
    religion,   //kristendomskundskab
    geography,  //geografi
    socialstud, //samfundsfag

    //Soft subjects - 8
    classtime,  //klassens time
    art,        //billedkunst
    crafting,   //håndarbejde
    music,      //musik
    cooking,    //hjemkundskab
    woodwork,   //sløjd
    phys_ed,    //idræt
    elective,    //valgfag

    //Special subjects - 1
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
void next_generation(struct week* population_pool, unsigned int n);
int fitness_of_week(const struct week* individual);
void print_fittest_week(const struct week* population_pool);
int fitness_function_mulitiple_lessons(const struct week* individual, int i, int j, int k);
int fitness_function_module_time(const struct week* individual, int i, int j, int k);
int fitness_function_no_free_space(const struct week* individual, i, j, k)


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
    return;
}

/* Krav
    1: Skriftlige fag tidligt på dagen
    2: Et sæt af samme fag (2)
    3: Tjek om dagene er cika lige lange
    4: Tjek om der ikke hul timer
    5: 
    */

int fitness_of_week(const struct week* individual) {    
    int fitness_module_time = 0;                     //Placering af fag tidsmæssigt.
    int fitness_multiple_lessons = 0;                //To fag i streg.
    int fitness_no_free_space = 0;                   //Ingen hul timer.
    int i;
    int j;
    int k;

    for (i = 0; i < DAYS_PR_WEEK; i++) {
        for (j = 0; i < MODULES_PR_DAY; j++) {
            for (k = 0; k < JOBS_PR_MODULE; k++) {
                
                fitness_module_time = fitness_function_module_time(individual, i, j, k);

                fitness_multiple_lessons = fitness_function_mulitiple_lessons(individual, i, j, k);

                fitness_no_free_space = fitness_function_no_free_space(individual, i, j, k);
            }
        }
    }

    return (fitness_module_time + fitness_multiple_lessons + fitness_preparation_time);
}


int fitness_function_module_time(const struct week* individual, int i, int j, int k) {
    int fitness_modules_time;

    if (individual->days[i].modules[j].jobs[k].subject < 4 && j < 2) {
        fitness_modules_time = 3;
    }
    else if (individual->days[i].modules[j].jobs[k].subject < 4 && j < 4) {
        fitness_modules_time = 2;
    }
    else if (individual->days[i].modules[j].jobs[k].subject < 4 && j < 8) {
        fitness_modules_time = 1;
    }

    
    if (individual->days[i].modules[j].jobs[k].subject >= 4 && individual->days[i].modules[j].jobs[k].subject < 11 && j < 2) {
        fitness_modules_time = 2;
    }
    else if (individual->days[i].modules[j].jobs[k].subject >= 4 && individual->days[i].modules[j].jobs[k].subject < 11 && j < 4) {
        fitness_modules_time = 3;
    }
    else if (individual->days[i].modules[j].jobs[k].subject >= 4 && individual->days[i].modules[j].jobs[k].subject < 11 && j < 8) {
        fitness_modules_time = 1;
    }


    if (individual->days[i].modules[j].jobs[k].subject >= 4 && individual->days[i].modules[j].jobs[k].subject < 19 && j < 2) {
        fitness_modules_time = 1;
    }
    else if (individual->days[i].modules[j].jobs[k].subject >= 4 && individual->days[i].modules[j].jobs[k].subject < 19 && j < 4) {
        fitness_modules_time = 2;
    }
    else if (individual->days[i].modules[j].jobs[k].subject >= 4 && individual->days[i].modules[j].jobs[k].subject < 19 && j < 8) {
        fitness_modules_time = 3;
    }
return fitness_modules_time;
}

int fitness_function_mulitiple_lessons(const struct week* individual, int i, int j, int k){
    int fitness_multiple_lessons;
    
    if (individual->days[i].modules[j].jobs[k].subject == individual->days[i].modules[j+1].jobs[k].subject && 
        individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[j+1].jobs[k].class && j % 2 == 0) {
    fitness_multiple_lessons = 6;
    }
    else if (individual->days[i].modules[j].jobs[k].subject == individual->days[i].modules[j+1].jobs[k].subject && 
             individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[j+1].jobs[k].class && j % 2 == 1) {
    fitness_multiple_lessons = 4;
    }


    return fitness_multiple_lessons;
}

int fitness_function_no_free_space(const struct week* individual, i, j, k) {
    int fitness_no_free_space = 0;
    int h = 1;

    if (individual->days[i].modules[j].jobs[k].free_time == 1){
        while (j + h < MODULES_PR_DAY) {
            if (individual->days[i].modules[j+h].jobs[k].free_time == 1) {
            h++;
            }
            
            else {
            fitness_no_free_space -= 9001; 
            }
        }
    }
    return fitness_no_free_space;
}

//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* population_pool) {
    return;
}

int fitness_function_day_length(const struct week* individual, i, k) {
    int day1 = 0; day2 = 0; day3 = 0; day4 = 0; day5 = 0;
    int i, k;
    int j = 0;

    if(i == 0){
        while (individual->days[i].modules[j].jobs[k].free_time == 0 && 
               individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[0].jobs[k].class){
            day1++;
            j++;
        }
    }
    else if (i == 1) {
        while (individual->days[i].modules[j].jobs[k].free_time == 0 && 
               individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[0].jobs[k].class){
            day2++;
            j++;
        }
    }
    else if (i == 2) {
        while (individual->days[i].modules[j].jobs[k].free_time == 0 && 
               individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[0].jobs[k].class){
            day3++;
            j++;        
        }
    }
    else if (i == 3) {
        while (individual->days[i].modules[j].jobs[k].free_time == 0 && 
                           individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[0].jobs[k].class){
            day4++;
            j++;       
        }
    }
    else if (i == 4) {
        while (individual->days[i].modules[j].jobs[k].free_time == 0 && 
               individual->days[i].modules[j].jobs[k].class == individual->days[i].modules[0].jobs[k].class){
            day5++;
            j++;        
        }
    }
    return 
}
