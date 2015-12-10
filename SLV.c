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
#define MAX_SUBJECT_CHARS (20)
#define EXPECTED_MIN_SCANS (4)
#define MAX_LINE_LENGTH (50)
#define JOB_FILE_NAME "jobs.txt"

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
struct job* read_jobs(void);
int read_job(struct job* job, FILE* file);
struct job* job_counter(FILE* file);
enum subjects translate_subject(char temp_subject[]);

//main function
int main(int argc, char *argv[]) {



    read_jobs();



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

//Function for reading all jobs from a file
//Returns NULL if error.
struct job* read_jobs(void) {
    int job = 0;//job pool index
    int done = 0;//while-loop condition flag
    int n_insertions;//number of insertions

    FILE* file = fopen(JOB_FILE_NAME, "r");
    if (!file) {
        return 0;
    }

    printf("IN READ_JOBS: FILE: %d\n", file);

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

    fclose(file);
    return job_pool;
}

//Function for reading one job from a file.
//Returns NULL if EOF, else returns number of inserted jobs in job pool.
int read_job(struct job* job, FILE* file) {
    int i;//loop counter
    int status = 0;
    int n_insertions;
    char buffer[MAX_LINE_LENGTH];
    struct job temp_job;
    char temp_subject[MAX_SUBJECT_CHARS];

    //Nullify teachers
    for (i = 0; i < TEACHERS_PR_JOB; i++) {
        temp_job.teacher[i][0] = 0;
        temp_job.teacher[i][TEACHER_IDENTIFIER_LENGTH - 1] = 0;
    }

    //Read one line from the file
    if(!fgets(buffer, MAX_LINE_LENGTH, file)) {
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

    //DEBUG:
    printf("\n%d\t%s\t%s\t%s\t%s\t%s\n",
        n_insertions,
        temp_subject,
        temp_job.class,
        temp_job.teacher[0],
        temp_job.teacher[1],
        temp_job.teacher[2]
    );

    //Return if EOF
    if (status < EXPECTED_MIN_SCANS) {
        return 0;
    }

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
        //DEBUG:
        printf("%d %d\n", new, n_jobs);
    }


    struct job* job_pool = malloc(sizeof(struct job) * (n_jobs+1));

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
