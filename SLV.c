//
//
//
//
//
//

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

//Constants
#define TEACHER_IDENTIFIER_LENGTH (3+1)
#define CLASS_IDENTIFIER_LENGTH (4+1)
#define TEACHERS_PR_JOB (3)
#define JOBS_PR_MODULE (15)
#define MODULES_PR_DAY (11)
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

void print_week(const struct week* fittest_week, char* teacher);
void print_module(const struct module* module, char* teacher, FILE* out_ptr);
void print_subject(enum subjects subject, FILE* out_ptr);


//main function
    int main(int argc, char *argv[]){
    struct week a_week;
    printf("stsdas\n");

    strcpy(a_week.days[0].modules[0].jobs[0].teacher[0], "AH\0");
        a_week.days[0].modules[0].jobs[0].subject = art;
        strcpy(a_week.days[0].modules[0].jobs[0].class, "7.\0");

    strcpy(a_week.days[1].modules[1].jobs[0].teacher[0], "AH\0");
        a_week.days[1].modules[1].jobs[0].subject = biology;
        strcpy(a_week.days[1].modules[1].jobs[0].class, "8.\0");

    strcpy(a_week.days[2].modules[3].jobs[0].teacher[0], "AH\0");
        a_week.days[2].modules[3].jobs[0].subject = danish;
        strcpy(a_week.days[2].modules[3].jobs[0].class, "9.\0");

    strcpy(a_week.days[3].modules[5].jobs[0].teacher[0], "AH\0");
        a_week.days[3].modules[5].jobs[0].subject = music;
        strcpy(a_week.days[3].modules[5].jobs[0].class, "1.\0");

    strcpy(a_week.days[4].modules[7].jobs[0].teacher[0], "AH\0");
        a_week.days[4].modules[7].jobs[0].subject = math;
        strcpy(a_week.days[4].modules[7].jobs[0].class, "3.\0");

    print_week(&a_week, "AH\0");

    return 0;
}

//Output the fittest of weeks in the population pool.
void print_week(const struct week* fittest_week, char* teacher) {

    int d,m;

    FILE* out_ptr = fopen("SCHEDULE.txt", "w");
    assert(out_ptr != 0);
    printf("%d\n", out_ptr);

    fprintf(out_ptr, "The schedule for %s is as following:\n\n", teacher);
    fprintf(out_ptr, "%-25s%-25s%-25s%-25s%-25s\n", "MAN", "TUE", "WED", "THU", "FRI");

    //Loop checks every day, module and job for the teacher to print schedule for
    for(m=0; m < MODULES_PR_DAY; m++) {
        for(d=0; d < DAYS_PR_WEEK; d++) {
            print_module(&fittest_week->days[d].modules[m], teacher, out_ptr);
        }

        fprintf(out_ptr, "\n\n");
    }

    fprintf(out_ptr, "Week printed for given teacher\n");

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
}



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
            fprintf(out_ptr, "%-20s", "idrat"); break;
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
            fprintf(out_ptr, "%-20s", "slojd"); break;
        case german:
            fprintf(out_ptr, "%-20s", "tysk"); break;
        case elective:
            fprintf(out_ptr, "%-20s", "valgfag"); break;
        case prep:
            fprintf(out_ptr, "%-20s", "forberedelsestid"); break;
        case crafting:
            fprintf(out_ptr, "%-20s", "haand"); break;
    }

    return;
}
