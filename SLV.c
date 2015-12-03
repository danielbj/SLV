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
    //crafting,   //håndarbejde (ER IKKE MED I DATAEN MERE)
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

//prototypes for readerfunction
int job_counter(void);
void file_reader(struct job *pool_jobs);
void clean_and_seperate_teachers(char temp_teacher[]);
enum subjects translator(char temp_subject[]);

struct job *pool_jobs;

//main function
int main(int argc, char *argv[]) {    
    //struct all_jobs{
       //struct job pool_jobs[job_counter()+1];
    //};
    
    file_reader(struct job *pool_jobs);

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

int job_counter(void) {
    FILE *new_text;
    new_text = fopen("jobs_comma","r");
    
    if(new_text == NULL) {
    printf("Error, could not read file");
    };
    
    int all = 0;
    int new;
    while (fscanf(new_text, "%d", &new) != EOF) {
        all += new;
    }
    struct job *pool_jobs;
    pool_jobs = (struct pool_jobs*) malloc(sizeof(struct job)*all);
    
return 0;
} 


void file_reader(struct job *pool_jobs) {
    FILE *raw_text;
    raw_text = fopen("jobs_comma","r");
    
    if(raw_text == NULL) {
    printf("Error, could not read file");
    };
    
    int g;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), raw_text) != EOF) {
        
        int temp_hour = 0;
        char temp_subject[20];
        char temp_class[20];
        char temp_teacher[20];
        
        sscanf(buffer,"%d %s %s %s", &temp_hour, temp_subject,
                                     temp_class, temp_teacher);
        //cleans buffer
        memset(buffer, '\0', sizeof(buffer));
        
        //finds individual teachers
        char all_teachers[TEACHERS_PR_JOB][TEACHER_IDENTIFIER_LENGTH];
        clean_and_seperate_teachers(temp_teacher);
        
        //translator from char to enum
        enum subjects translator(char temp_subject[]);
        
        // function for copying into all_jobs
        int i;
        for(i = 0; i < temp_hour; i++) {
            pool_jobs.teacher[g] = all_teachers;
            pool_jobs.class[g] = temp_class;
            pool_jobs.subject[g] = trans_subject;
            g++
        };
    
    }
    
    //places NULL at the end of pool_jobs
    pool_jobs.teacher[g+1] = NULL;
    pool_jobs.class[g+1] = NULL;
    pool_jobs.subject[g+1] = NULL;
    
return 0;
}

void clean_and_seperate_teachers(temp_teacher) {
    //cleans temp_teacher of commas
    int h;
    char space = " ";
    char comma = ",";
    for(h = 0; h < sizeof(temp_teacher); h++) {
        if(temp_teacher[h] == comma) {
        temp_teacher[h] = space;
        }
    }
        
   //teacher seperator
   char seperat_teachers[TEACHERS_PR_JOB][TEACHER_IDENTIFIER_LENGTH];
   sscanf(temp_teacher, "%s %s %s", seperat_teachers[0][0], 
                                    seperat_teachers[1][0], 
                                    seperat_teachers[2][0]);
        
   memcpy(all_teachers,seperat_teachers, 
   TEACHERS_PR_JOB*TEACHER_IDENTIFIER_LENGTH*sizeof(seperat_teachers));

return 0;
}

//translate from char to enum
enum subjects translator(char temp_subject[]) {
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
    else if(strncmp(temp_subject, "Idræt", 4) == 0) {
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
    else if(strncmp(temp_subject, "Sløjd", 4) == 0) {
    trans_subject = woodwork;
    }
    else if(strncmp(temp_subject, "Tysk", 4) == 0) {
    trans_subject = german;
    }
    else if(strncmp(temp_subject, "Valghold", 4) == 0) {
    trans_subject = elective;
    }
    
    return trans_subject;
}

//Function for generating weeks, by randomly inserting jobs in struct, removing teacher conflicts 
struct week* insert_jobs(int number_of_weeks) {
    
    return 0;
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















