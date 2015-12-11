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
#define JOBS_PR_MODULE (10)
#define MODULES_PR_DAY (11)
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

struct week* insert_jobs(struct job* job_pool, int number_of_weeks);
int generate_week(struct week* week, struct job* job_pool);
int insert_job_in_week(struct job* job, struct week* week, int day, int module);
int insert_job_in_module(struct job* job, struct module* module);
int is_empty_job(struct job* job);
int is_teacher_conflict(struct module* module, struct job* job);
int is_class_conflict(struct module* module, struct job* job);

struct job* read_jobs(void);
int read_job(struct job* job, FILE* file);
struct job* job_counter(FILE* file);
enum subjects translate_subject(char temp_subject[]);

//main function
int main(int argc, char *argv[]) {

    struct week* week_pool = initialize_weeks(100);
    printf("%u\n", week_pool);

    //printf("%s\n", week_pool[0].days[0].modules[0].jobs[0].class);

    return 0;
}



//Input + generation + sanitation
//Initialize n weeks with jobs from jobs.txt
//Returns NULL if error.
struct week* initialize_weeks(int number_of_weeks) {
    struct job* job_pool = 0;

    job_pool = read_jobs();
    if(!job_pool) {
        return 0;
    }

    return insert_jobs(job_pool, number_of_weeks);
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

    //Insert empty job
    job_pool[job].teacher[0][0] = 0;

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

    //Return if EOF
    if (status < EXPECTED_MIN_SCANS) {
        return 0;
    }

    //Translate the subject to enum
    temp_job.subject = translate_subject(temp_subject);

    n_insertions /= 2;//DEBUG to ease the week generation.

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

    n_jobs /= 2;//DEBUG to ease the week generation.

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
    srand(time(0));//Move to main?!

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
    int e_day, e_module;//For storing the effective day and module indexes.
    struct module* current_module;//For holding the module currently being tested for insertion.


    for (d_day = 0; d_day < DAYS_PR_WEEK; d_day++) {
        for (d_module = 0; d_module < MODULES_PR_DAY; d_module++) {
            e_day = (day + d_day) % DAYS_PR_WEEK;
            e_module = (module + d_module) % MODULES_PR_DAY;
            current_module = &week->days[e_day].modules[e_module];
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
    return job->teacher[0][0] == 0;
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

                //printf("%s vs. %s\n", module->jobs[d_job].teacher[d_teacher], job->teacher[d_job_teacher]);/DEBUG

                if (module->jobs[d_job].teacher[d_teacher][0]) {
                    if (!strcmp(module->jobs[d_job].teacher[d_teacher],
                                job->teacher[d_job_teacher])) {
                        printf("TEACHER CONFLICT!\n");//DEBUG
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
        if(!strcmp(module->jobs[d_job].class, job->class)) {
            return 1;
        }
    }

    return 0;
}
