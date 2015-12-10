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
void print_fittest_week(const struct week* fittest_week, char* teacher);
char *enum_to_string(enum subjects subject);



//main function VIL IKKE KØRE ORDENTILIGT
int main(int argc, char *argv[]){
struct week a_week;
printf("stsdas\n");
/*
strcpy(a_week.days[0].modules[0].jobs[0].teacher[0], "AH");
    a_week.days[0].modules[0].jobs[0].subject = art;
    strcpy(a_week.days[0].modules[0].jobs[0].class, "7.");
strcpy(a_week.days[1].modules[1].jobs[0].teacher[0], "AH");
    a_week.days[1].modules[1].jobs[0].subject = biology;
    strcpy(a_week.days[1].modules[1].jobs[0].class, "8.");
strcpy(a_week.days[2].modules[3].jobs[0].teacher[0], "AH");
    a_week.days[2].modules[3].jobs[0].subject = danish;
    strcpy(a_week.days[2].modules[3].jobs[0].class, "9.");
strcpy(a_week.days[3].modules[5].jobs[0].teacher[0], "AH");
    a_week.days[3].modules[5].jobs[0].subject = music;
    strcpy(a_week.days[3].modules[5].jobs[0].class, "1.");
strcpy(a_week.days[4].modules[7].jobs[0].teacher[0], "AH");
    a_week.days[4].modules[7].jobs[0].subject = math;
    strcpy(a_week.days[4].modules[7].jobs[0].class, "3.");

print_fittest_week(&a_week, "AH");
*/
    return 0;
}

//Output the fittest of weeks in the population pool.
void print_fittest_week(const struct week* fittest_week, char* teacher) {
    int d,m,j,t, status = -123 ;
    char temp_string_subject[MAX_SUBJECT_CHARS];
    FILE *out_ptr;

    out_ptr = fopen("Optimal_schedule.txt", "w");
    assert(out_ptr != NULL);

    fprintf(out_ptr, "The schedule for %s is as following:\n\n", teacher);
    fprintf(out_ptr, "%s%40s%40s%40s%40s\n" "MAN", "TUE", "WED", "THU", "FRI");
    //Loop checks every day, module and job for the teacher to print schedule for
    for(m=0; m < MODULES_PR_DAY; m++){
        for (j=0; j < JOBS_PR_MODULE; j++){
            for(d=0; d < DAYS_PR_WEEK; d++){
                for (t = 0; t < TEACHERS_PR_JOB; t++) {
                    if (strcmp(teacher, fittest_week->days[d].modules[m].jobs[j].teacher[t])){
                        strcpy(temp_string_subject, enum_to_string(fittest_week->days[d].modules[m].jobs[j].subject));
                        status = fprintf(out_ptr, "%20s   %5s %15s",
                               temp_string_subject, fittest_week->days[d].modules[m].jobs[j].class,
                               " ");
                        printf("%d\n", status);
                        //fseek(out_ptr, 15, SEEK_CUR); Designet til at layout passer så der skrives under hver dag hvis ikke space løsning virker
                    }
                }
            }
        }
        fprintf(out_ptr, "________________________________________\n");
    }

    fprintf(out_ptr, "Week printed for given teacher\n");
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
