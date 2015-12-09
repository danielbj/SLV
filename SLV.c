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
#define MAX_SUBJECT_CHARS (15)

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
void next_generation(struct week* population_pool, unsigned int n);
int fitness_of_week(const struct week* individual);
void print_fittest_week(const struct week* population_pool);
void output(struct week result_week);
char *enum_to_string(enum subjects subject);



//main function
int main(int argc, char *argv[]){

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



//Function for finding fitness of one week.
int fitness_of_week(const struct week* individual) {
    return;
}



//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* population_pool) {
    return;
}

void output(struct week result_week){
    int d,m,j;
    char temp_string_subject[MAX_SUBJECT_CHARS];
    FILE *out_ptr;

    out_ptr = fopen("Optimal_schedule.txt", "w");
    assert(out_ptr != NULL);

    fprintf(out_ptr, "The schedule:\n\n");
    fprintf(out_ptr, "%s%40s%40s%40s%40s\n" "", "TUE", "WED", "THU", "FRI");
    for(d=0; d < DAYS_PR_WEEK; d++){
        for(m=0; m < MODULES_PR_DAY; m++){
            for (j=0; j < JOBS_PR_MODULE; j++){
                strcpy(temp_string_subject, enum_to_string(result_week.days[d].modules[m].jobs[j].subject));
                fprintf(out_ptr, "%20.20s   %5.5s   %4.4s\n",
                       temp_string_subject, result_week.days[d].modules[m].jobs[j].class,
                       result_week.days[d].modules[m].jobs[j].teacher[0]);
            }
            fprintf(out_ptr, "____________________\n");
        }
    }
}
char *enum_to_string(enum subjects subject){
    char temp_string[MAX_SUBJECT_CHARS];

    switch (subject) {
        case art:
            strcpy(temp_string, "billedkunst"); break;
        case biology:
            strcpy(temp_string, "biologi"); break;
        case danish:
            strcpy(temp_string, "dansk"); break;
        case english:
            strcpy(temp_string, "engelsk"); break;
        case physics:
            strcpy(temp_string, "fysik"); break;
        case geography:
            strcpy(temp_string, "geografi"); break;
        case history:
            strcpy(temp_string, "historie"); break;
        case phys_ed:
            strcpy(temp_string, "idræt"); break;
        case classtime:
            strcpy(temp_string, "klassens time"); break;
        case religion:
            strcpy(temp_string, "kristendomskundskab"); break;
        case cooking:
            strcpy(temp_string, "hjemkundskab"); break;
        case math:
            strcpy(temp_string, "matematik"); break;
        case music:
            strcpy(temp_string, "musik"); break;
        case nature:
            strcpy(temp_string, "natur-teknik"); break;
        case socialstud:
            strcpy(temp_string, "samfundsfag"); break;
        case woodwork:
            strcpy(temp_string, "sløjd"); break;
        case german:
            strcpy(temp_string, "tysk"); break;
        case elective:
            strcpy(temp_string, "valgfag"); break;
        case prep:
            strcpy(temp_string, "forberedelsestid"); break;
    }

    return temp_string;
}
