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
#define JOBS_PR_MODULE (20)
#define MODULES_PR_DAY (30)
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
    crafting,   //håndarbejde (ER IKKE MED I DATAEN MERE)
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

void print_week(const struct week* fittest_week, char* teacher);
void print_module(const struct module* module, char* teacher, FILE* out_ptr);
void print_subject(enum subjects subject, FILE* out_ptr);



//Main function
int main(int argc, char *argv[]){

    struct week* week_pool = initialize_weeks(1);
    assert(week_pool != 0);

    print_week(&week_pool[0], "HM");

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
            printf("INSERTION POINT = %d\n", job);//DEBUG
        }
    }

    //Insert empty job
    job_pool[job].teacher[0][0] = 0;

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
        temp_job.teacher[i][0] = 0;
        temp_job.teacher[i][TEACHER_IDENTIFIER_LENGTH - 1] = 0;
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

    printf("%d", n_jobs);

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
    else if(strncmp(temp_subject, "Forberedelsestid", 4) == 0) {
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
    assert(week_pool != 0);

    //seed random number generator
    srand(time(0));//Move to main?!

    //Generate n weeks
    for (i = 0; i < number_of_weeks; i++) {
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
        printf("JOB POOL INDEX: %d: ", i);//DEBUG

        //DEBUG
        random_day = 0;//rand() % DAYS_PR_WEEK;//Select a random day
        random_module = 0;//rand() % MODULES_PR_DAY;//Select a random module

        if (!insert_job_in_week(&job_pool[i], week, random_day, random_module)) {
            return 0;
        } else {
            i++;
        }
        printf("Inserted!\n");
    }

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
    if (is_class_conflict(module, job) || is_teacher_conflict(module, job)) {
        printf("CONFLICT!\n");//DEBUG
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

                if (!is_empty_job(&module->jobs[d_job])) {
                    if (!strcmp(module->jobs[d_job].teacher[d_teacher],
                                job->teacher[d_job_teacher])) {
                        printf("\tTEACHER ");//DEBUG
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
                printf("\tCLASS ");//DEBUG
                return 1;
            }
        }
    }

    return 0;
}



//Output the fittest of weeks in the population pool.
void print_week(const struct week* fittest_week, char* teacher) {

    int d,m;

    FILE* out_ptr = fopen("SCHEDULE.txt", "w");
    assert(out_ptr != 0);

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
