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
    1: Forberedelse skal ligge tæt på den kommende leksion
    2: Skriftlige fag tidligt på dagen
    3: Et sæt af samme fag (2)
    */

    /*Ombytning af fag i enum subjects for at gøre det mere
    effektivt at tjekke fagene*/

    /*Hvordan man tilgår det rigtige fag i det rigtige job
    i det rigtige modul*/
    /**/



int fitness_of_week(const struct week* individual) {    
    int Fitness_ModuleTime = 0;                     //Placering af fag tidsmæssigt
    int Fitness_ModuleMultipleLessons = 0;          //To fag i streg
    int Fitness_PreparationTime = 0;                //Forberedelse ska ligge ved den kommende leksion
    int i;
    int j;
    int k;

    for (i = 0; i < DAYS_PR_WEEK; i++) {
        for (j = 0; i < MODULES_PR_DAY; j++) {
            for (k = 0; k < JOBS_PR_MODULE; k++) {

                fitness_function_preparation_time(individual, i, j , k);

                fitness_function_module_time(individual, i, j, k);

                if (individual.day[i].module[j].job[] == individual.day[i].module[j+1].job[] && j % 2 == 0) {
                    Fitness_ModuleMultipleLessons += Magisk Tal;
                }
            }
        }
    }

    return (Fitness_ModuleTime + Fitness_ModuleMultipleLessons + Fitness_PreparationTime);
}

fitness_function_preparation_time(const struct week* individual, int i, int j, int k) {

}

fitness_function_module_time(const struct week* individual, int i, int j, int k) {
    if (individual.day[i].module[j].job[k].subject < 4 && j < 3) {
        //Giv højt antal point
        Fitness_ModuleTime += Magisk Tal;
    }
}



//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* population_pool) {
    return;
}















