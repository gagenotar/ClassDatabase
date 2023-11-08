/*
12/8/2022 Gage Notargiacomo
This is my code for my final project in my Intro to C course. The code
demonstrates extensive knowledge of programming in C, and focuses mainly on
structs, fileIO, and dedicated memory allocation (DMA). The program will first
take in files containing information about students, courses, and enrollments
then will ask for user input to retrieve from/add information to such files.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "leak_detector_c.h"

#define MAXFILE 31
#define MAXSTR 51
#define MAXSEM 101

// Global vriables
char studentfile[MAXFILE], coursfile[MAXFILE], enrollmentfile[MAXFILE];

// Structs
typedef struct Student {
  int stid;
  char *fname;
  char *lname;
  int day;
  int month;
  int year;
} Student;

typedef struct Enrollment {
  int stid;
  char cid[MAXSTR];
  char semester[MAXSTR];
  float score;
} Enrollment;

typedef struct Course {
  char cid[MAXSTR];
  char cname[MAXSTR];
  float tot_cred;
} Course;

// Function prototypes
Student *loadStudents(int *numStudents);
Enrollment *loadEnrollment(int *numEnrollments);
Course *loadCourses(int *numCourses);
char getScore(float score);
int checkCourse(char cid[MAXSTR], Course *CrsArray, int crsn);
void sortStudents(Student *StdArray, int stdn);
void free_memory(Student *StdArray, int stdn, Enrollment *EnrArray, int enrn, Course *CrsArray, int crsn);
void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses,
          int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
void nameSearch(char key[MAXSTR], Student *StdArray, int stdn,
                Enrollment *EnrArray, int enrn, Course *CrsArray, int crsn);
void idSearch(int key, Student *StdArray, int stdn, Enrollment *EnrArray,
              int enrn, Course *CrsArray, int crsn);
void yearSearch(int key, Student *StdArray, int stdn, Enrollment *EnrArray,
                int enrn, Course *CrsArray, int crsn);
void cidSearch(char key[MAXSTR], Student *StdArray, int stdn,
               Enrollment *EnrArray, int enrn, Course *CrsArray, int crsn);
void cidSemSearch(char cid[MAXSTR], char sem[MAXSTR], Student *StdArray,
                  int stdn, Enrollment *EnrArray, int enrn, Course *CrsArray,
                  int crsn);

int main(void) {
  // For memory leak detector
  // atexit(report_mem_leak);
  
  int stdn, enrn, crsn;
  Student *std_list;
  Enrollment *enr_list;
  Course *crs_list;

  // Call load functions to fill arrays
  std_list = loadStudents(&stdn);
  crs_list = loadCourses(&crsn);
  enr_list = loadEnrollment(&enrn);

  // Open menu
  menu(std_list, stdn, crs_list, crsn, enr_list, enrn);

  return 0;
}

// numStudents will dynamically allocate an array of students based on the
// number of students available in the file, fill up the array and then return
// array.
Student *loadStudents(int *numStudents) {
  // Obtain file name from user
  scanf("%s", studentfile);

  // Establish file ptr and open file
  FILE *std_file = fopen(studentfile, "r");
  fscanf(std_file, "%d", numStudents);

  // Allocate memory for the list
  Student *std_list = (Student *)malloc((*numStudents) * sizeof(Student));

  // Loop to obtain and assign values to n number of Enrollment structs
  char fname[MAXSTR];
  char lname[MAXSTR];
  for (int i = 0; i < (*numStudents); i++) {
    fscanf(std_file, "%d", &std_list[i].stid);
    fscanf(std_file, "%s", fname);
    fscanf(std_file, "%s", lname);
    fscanf(std_file, "%d", &std_list[i].day);
    fscanf(std_file, "%d", &std_list[i].month);
    fscanf(std_file, "%d", &std_list[i].year);

    // Allocate memory for the first and last names
    int lenf = strlen(fname) + 1;
    std_list[i].fname = (char *)malloc(lenf * sizeof(char));
    strcpy(std_list[i].fname, fname);
    int lenl = strlen(lname) + 1;
    std_list[i].lname = (char *)malloc(lenf * sizeof(char));
    strcpy(std_list[i].lname, lname);
  }

  // Always close files after opening
  fclose(std_file);

  return std_list;
}

// This function loads the enrollment records and returns a dynamically
// allocated array for the enrollment and returns it.
Enrollment *loadEnrollment(int *numEnrollments) {
  // Obtain file name from user
  scanf("%s", enrollmentfile);

  // Establish file ptr and open file
  Enrollment *enr_list = NULL;
  FILE *enr_file = fopen(enrollmentfile, "r");
  fscanf(enr_file, "%d", numEnrollments);

  enr_list = (Enrollment *)malloc((*numEnrollments) * sizeof(Enrollment));

  // Loop to obtain and assign values to n number of Enrollment structs
  for (int i = 0; i < (*numEnrollments); i++) {
    fscanf(enr_file, "%d", &enr_list[i].stid);
    fscanf(enr_file, "%s", enr_list[i].cid);
    fscanf(enr_file, "%s", enr_list[i].semester);
    fscanf(enr_file, "%fl", &enr_list[i].score);
  }

  // Always close files after opening
  fclose(enr_file);

  return enr_list;
}

// This function loads courses from the file into a dynamically allocated array
// of Couse and returns it.
Course *loadCourses(int *numCourses) {
  // Obtain file name from user
  scanf("%s", coursfile);

  // Establish file ptr and open file
  Course *crs_list = NULL;
  FILE *crs_file = fopen(coursfile, "r");
  fscanf(crs_file, "%d", numCourses);

  crs_list = (Course *)malloc((*numCourses) * sizeof(Course));

  // Loop to obtain and assign values to n number of Course structs
  for (int i = 0; i < (*numCourses); i++) {
    fscanf(crs_file, "%s", crs_list[i].cid);
    fscanf(crs_file, "%s", crs_list[i].cname);
    fscanf(crs_file, "%fl", &crs_list[i].tot_cred);
  }

  // Always close files after opening
  fclose(crs_file);

  return crs_list;
}

// This function should be called from the main function after loading the
// arrays and all the commands should be handled in this function in addition to
// the other functions.
void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses,
          int amountCourses, Enrollment *ArrayOfEnrollments,
          int amountEnrolls) {
  while (1) {
    printf("==================\n");
    char choice[MAXSTR];
    char x;
    scanf("%s", choice);

    // Exit command
    if (strcmp(choice, "exit") == 0){
      printf("------------------\n");
      printf("Bye!\n");
      free_memory(ArrayOfStudents, amountStudents, ArrayOfEnrollments,       
                  amountEnrolls, ArrayOfCourses, amountCourses);
      break;
    }

    // Search_students command
    else if (strcmp(choice, "search_students") == 0) {
      char std_search[MAXSTR];
      scanf("%s", std_search);

      if (strcmp(std_search, "lname") == 0) {
        char lname[MAXSTR];
        scanf("%s", lname);
        nameSearch(lname, ArrayOfStudents, amountStudents, ArrayOfEnrollments,
                   amountEnrolls, ArrayOfCourses, amountCourses);
      } else if (strcmp(std_search, "id") == 0) {
        int id = 0;
        scanf("%d", &id);
        idSearch(id, ArrayOfStudents, amountStudents, ArrayOfEnrollments,
                 amountEnrolls, ArrayOfCourses, amountCourses);
      } else if (strcmp(std_search, "byear") == 0) {
        int year = 0;
        scanf("%d", &year);
        yearSearch(year, ArrayOfStudents, amountStudents, ArrayOfEnrollments,
                   amountEnrolls, ArrayOfCourses, amountCourses);
      } else
        printf("Invalid option\n");
    }

    // Search_course command
    else if (strcmp(choice, "search_course") == 0) {
      char crs_search[MAXSTR];
      scanf("%s", crs_search);

      if (strcmp(crs_search, "cid") == 0) {
        char cid[MAXSTR];
        scanf("%s", cid);
        cidSearch(cid, ArrayOfStudents, amountStudents, ArrayOfEnrollments,
                  amountEnrolls, ArrayOfCourses, amountCourses);
      }

      else if (strcmp(crs_search, "cid_semester") == 0) {
        char cid[MAXSTR];
        char sem[MAXSTR];
        scanf("%s%s", cid, sem);
        cidSemSearch(cid, sem, ArrayOfStudents, amountStudents,
                     ArrayOfEnrollments, amountEnrolls, ArrayOfCourses,
                     amountCourses);
      }

      else
        printf("Invalid option\n");
    }

    // Add_course command
    else if (strcmp(choice, "add_course") == 0) {
      char cid[MAXSTR];
      char cname[MAXSTR];
      float cred = 0;
      scanf("%s%s%f", cid, cname, &cred);
      printf("------------------\n");

      int result = 0;
      result = checkCourse(cid, ArrayOfCourses, amountCourses);
      if (result == 1) {
        // Allocate memory for a new Course in ArrayOfCourses
        int size = amountCourses + 1;
        ArrayOfCourses =
            (Course *)realloc(ArrayOfCourses, (size) * sizeof(Course));

        // Update courses file
        FILE *crs_file = NULL;
        crs_file = fopen(coursfile, "r+");
        fseek(crs_file, 0, SEEK_SET);
        fprintf(crs_file, "%d", size);
        fseek(crs_file, 0, SEEK_END);
        fprintf(crs_file, "\n%s %s %.0f", cid, cname, cred);
        fseek(crs_file, 2, SEEK_SET);
        
        // Reload the ArrayOfCourses
        amountCourses = size;
        for (int i = 0; i < (amountCourses); i++) {
          fscanf(crs_file, "%s", ArrayOfCourses[i].cid);
          fscanf(crs_file, "%s", ArrayOfCourses[i].cname);
          fscanf(crs_file, "%fl", &ArrayOfCourses[i].tot_cred);          
        }

        // Always close files
        fclose(crs_file);

        // Provide feedback
        printf("record added\n");
      } 
      else {
        printf("violation of course id uniqueness. Cannot add the record.\n");
      }
      printf("------------------\n");
    }

    // Count command
    else if (strcmp(choice, "count") == 0) {
      char count[MAXSTR];
      scanf("%s", count);
      printf("------------------\n");

      if (strcmp(count, "students") == 0) {
        printf("total students %d\n", amountStudents);
        printf("------------------\n");
      }

      else if (strcmp(count, "students_course") == 0) {
        char cid_list[amountCourses][MAXSTR];
        int student_count[amountCourses];

        // Initialize student_count and cid_list
        for(int a=0; a<amountCourses; a++){
          student_count[a] = 0;
          strcpy(cid_list[a], " ");
        }

        int tracker = 0;
        // Loop through every enrollment
        for (int i = 0; i < amountEnrolls; i++) {
          int flag = 0;
          char cid[MAXSTR];
          strcpy(cid, ArrayOfEnrollments[i].cid);
          
          // If cid exists in cid_list, add 1 to its student_count
          for(int j=0; j<amountCourses; j++){
            if (strcmp(cid, cid_list[j]) == 0){
              student_count[j]++;
              tracker++;
              break;
            }
            flag++;
          }

          // If cid isn't in list, add it to the list and add 1 to its std_count
          if (flag == amountCourses){
            strcpy(cid_list[i-tracker], ArrayOfEnrollments[i].cid);
            student_count[i-tracker]++;
          }
        }

        
        // Print out results
        for(int i=0; i<amountCourses; i++){
          for(int j=0; j<amountCourses; j++){
            if(strcmp(cid_list[i], ArrayOfCourses[j].cid) == 0)
              break;
            else{
              strcpy(cid_list[i], ArrayOfCourses[i].cid);
            }
          }
          printf("%s %d\n", cid_list[i], student_count[i]);
        }
        printf("------------------\n");
      }

      else if (strcmp(count, "students_semester") == 0){
        char sem_list[MAXSEM][MAXSTR];
        int student_count[MAXSEM];

        // Initialize student_count and sem_list
        for(int a=0; a<MAXSEM; a++){
          student_count[a] = 0;
          strcpy(sem_list[a], " ");
        }        

        int tracker = 0;
        int size = 1;
        // Loop through every enrollment
        for (int i = 0; i < amountEnrolls; i++) {
          int flag = 0;
          char sem[MAXSTR];
          strcpy(sem, ArrayOfEnrollments[i].semester);
          
          // If sem exists in sem_list, add 1 to its student_count
          for(int j=0; j<size; j++){
            if (strcmp(sem, sem_list[j]) == 0){
              student_count[j]++;
              tracker++;
              break;
            }
            flag++;
          }

          // If cid isn't in list, add it to the list and add 1 to its std_count
          if (flag == size){
            strcpy(sem_list[i-tracker], ArrayOfEnrollments[i].semester);
            student_count[i-tracker]++;
            size++;
          }
        }

        
        // Print out results
        for(int i=0; i<size-1; i++){
          printf("%s %d\n", sem_list[i], student_count[i]);
        }
        printf("------------------\n");        
      }

    }

    // Sort command
    else if (strcmp(choice, "sort") == 0){
      printf("------------------\n");
      sortStudents(ArrayOfStudents, amountStudents);
      printf("------------------\n");
      }

    // Provide feedback
    else
      printf("Invalid option\n");
  }
}

// Function assigns a letter score after receiving a float
char getScore(float score) {
  char grade = ' ';
  if (score >= 90)
    grade = 'A';
  else if (score >= 80 && score < 90)
    grade = 'B';
  else if (score >= 70 && score < 80)
    grade = 'C';
  else if (score >= 60 && score < 70)
    grade = 'D';
  else if (score < 60)
    grade = 'F';
  return grade;
}

// Function checks if course already exists
int checkCourse(char cid[MAXSTR], Course *CrsArray, int crsn) {
  int result = 1;
  for (int i = 0; i < crsn; i++) {
    if (strcmp(cid, CrsArray[i].cid) == 0)
      result = 0;
  }
  return result;
}

// Function sorts students by sid
void sortStudents(Student *StdArray, int stdn){
  // Loop for each index
  for(int j=0; j<stdn; j++){
    int min = StdArray[j].stid;
    int index = j;
    
    // Find min stid
    for(int i=j; i<stdn; i++){
      if(StdArray[i].stid < min){
        min = StdArray[i].stid;
        index = i;
      }
    }

    // Initialize temp variables
    int temp_id = 0, temp_day = 0, temp_month = 0, temp_year = 0;
    char temp_fname[MAXSTR];
    char temp_lname[MAXSTR];

    // Swap StdArray[index] with StdArray[counter]
    temp_id = StdArray[j].stid;
    temp_day = StdArray[j].day;
    temp_month = StdArray[j].month;
    temp_year = StdArray[j].year;
    strcpy(temp_fname, StdArray[j].fname);
    strcpy(temp_lname, StdArray[j].lname);

    StdArray[j].stid = StdArray[index].stid;
    StdArray[j].day = StdArray[index].day;
    StdArray[j].month = StdArray[index].month;
    StdArray[j].year = StdArray[index].year;
    strcpy(StdArray[j].fname, StdArray[index].fname);
    strcpy(StdArray[j].lname, StdArray[index].lname);

    StdArray[index].stid = temp_id;
    StdArray[index].day = temp_day;
    StdArray[index].month = temp_month;
    StdArray[index].year = temp_year;
    strcpy(StdArray[index].fname, temp_fname);
    strcpy(StdArray[index].lname, temp_lname);  
  }
  
  for(int i=0; i<stdn; i++){
    printf("%d %s %s %d/%d/%d\n", StdArray[i].stid, StdArray[i].fname, StdArray[i].lname,                                           StdArray[i].month, StdArray[i].day, StdArray[i].year);
  
  }
}

// Function frees memory
void free_memory(Student *StdArray, int stdn, Enrollment *EnrArray, int enrn,                       Course *CrsArray, int crsn)
{
  // Free allocated strings from StdArray
  for(int i=0; i<stdn; i++)
  {
      free(StdArray[i].fname);
      free(StdArray[i].lname);
  }
  // Free the StdArray
  free(StdArray);
  
  // Free the EnrArray
  free(EnrArray);

  // Free the CrsArray
  free(CrsArray);
}

// Function used for name search command
void nameSearch(char key[MAXSTR], Student *StdArray, int stdn,
                Enrollment *EnrArray, int enrn, Course *CrsArray, int crsn) {
  int flag = 0;
  printf("------------------\n");
  for (int i = 0; i < stdn; i++) {
    char lname[MAXSTR];
    strcpy(lname, StdArray[i].lname);

    // If lname has a match...
    if (strcmp(key, StdArray[i].lname) == 0) {
      flag++;
      // Prints id, name, and bday
      printf("%d ", StdArray[i].stid);
      printf("%s ", StdArray[i].fname);
      printf("%s ", StdArray[i].lname);
      printf("%d/%d/%d\n", StdArray[i].month, StdArray[i].day,
             StdArray[i].year);
      // Prints classes, semester, score, and grade
      for (int j = 0; j < enrn; j++) {
        // If student is enrolled in a class...
        if (StdArray[i].stid == EnrArray[j].stid) {
          for (int k = 0; k < crsn; k++) {
            // And if the class matches a course ID, print the course name,
            // semester, score, and grade letter.
            if (strcmp(EnrArray[j].cid, CrsArray[k].cid) == 0) {
              printf("%s ", EnrArray[j].cid);
              printf("%s ", CrsArray[k].cname);
              printf("%s ", EnrArray[j].semester);
              printf("%.0f ", EnrArray[j].score);
              char grade = getScore(EnrArray[j].score);
              printf("%c\n", grade);
            }
          }
        }
      }
      printf("------------------\n");
    }
  }
  // If not found, provide feedback
  if (flag == 0) {
    printf("not found\n");
    printf("------------------\n");
  }
}

// Function used for student id search command
void idSearch(int key, Student *StdArray, int stdn, Enrollment *EnrArray,
              int enrn, Course *CrsArray, int crsn) {
  int flag = 0;
  printf("------------------\n");
  for (int i = 0; i < stdn; i++) {

    // If stid has a match...
    if (key == StdArray[i].stid) {
      flag++;
      // Prints id, name, and bday
      printf("%d ", StdArray[i].stid);
      printf("%s ", StdArray[i].fname);
      printf("%s ", StdArray[i].lname);
      printf("%d/%d/%d\n", StdArray[i].month, StdArray[i].day,
             StdArray[i].year);
      // Prints classes, semester, score, and grade
      for (int j = 0; j < enrn; j++) {
        // If student is enrolled in a class...
        if (StdArray[i].stid == EnrArray[j].stid) {
          for (int k = 0; k < crsn; k++) {
            // And if the class matches a course ID, print the course name,
            // semester, score, and grade letter.
            if (strcmp(EnrArray[j].cid, CrsArray[k].cid) == 0) {
              printf("%s ", EnrArray[j].cid);
              printf("%s ", CrsArray[k].cname);
              printf("%s ", EnrArray[j].semester);
              printf("%.0f ", EnrArray[j].score);
              char grade = getScore(EnrArray[j].score);
              printf("%c\n", grade);
            }
          }
        }
      }
      printf("------------------\n");
    }
  }
  // If not found, provide feedback
  if (flag == 0) {
    printf("not found\n");
    printf("------------------\n");
  }
}

// Function used for byear search command
void yearSearch(int key, Student *StdArray, int stdn, Enrollment *EnrArray,
                int enrn, Course *CrsArray, int crsn) {
  int flag = 0;
  printf("------------------\n");
  for (int i = 0; i < stdn; i++) {

    // If stid has a match...
    if (key == StdArray[i].year) {
      flag++;
      // Prints id, name, and bday
      printf("%d ", StdArray[i].stid);
      printf("%s ", StdArray[i].fname);
      printf("%s ", StdArray[i].lname);
      printf("%d/%d/%d\n", StdArray[i].month, StdArray[i].day,
             StdArray[i].year);
      // Prints classes, semester, score, and grade
      for (int j = 0; j < enrn; j++) {
        // If student is enrolled in a class...
        if (StdArray[i].stid == EnrArray[j].stid) {
          for (int k = 0; k < crsn; k++) {
            // And if the class matches a course ID, print the course name,
            // semester, score, and grade letter.
            if (strcmp(EnrArray[j].cid, CrsArray[k].cid) == 0) {
              printf("%s ", EnrArray[j].cid);
              printf("%s ", CrsArray[k].cname);
              printf("%s ", EnrArray[j].semester);
              printf("%.0f ", EnrArray[j].score);
              char grade = getScore(EnrArray[j].score);
              printf("%c\n", grade);
            }
          }
        }
      }
      printf("------------------\n");
    }
  }
  // If not found, provide feedback
  if (flag == 0) {
    printf("not found\n");
    printf("------------------\n");
  }
}

// Function used for course id search command
void cidSearch(char key[MAXSTR], Student *StdArray, int stdn,
               Enrollment *EnrArray, int enrn, Course *CrsArray, int crsn) {
  int flag = 0;
  printf("------------------\n");
  for (int i = 0; i < crsn; i++) {

    // If key matches a cid...
    if (strcmp(key, CrsArray[i].cid) == 0) {
      flag++;
      // Prints course name and credit
      printf("%s ", CrsArray[i].cname);
      printf("%.0f\n", CrsArray[i].tot_cred);

      // Prints student name, semester, and grade
      for (int j = 0; j < enrn; j++) {
        // If the class id matches an enrolled cid...
        if (strcmp(CrsArray[i].cid, EnrArray[j].cid) == 0) {
          for (int k = 0; k < stdn; k++) {
            // And if the enrolled stid matches a student ID,
            // print the student name, semester, and grade letter.
            if (EnrArray[j].stid == StdArray[k].stid) {
              printf("%s ", StdArray[k].fname);
              printf("%s ", StdArray[k].lname);
              printf("%s ", EnrArray[j].semester);
              printf("%.0f ", EnrArray[j].score);
              char grade = getScore(EnrArray[j].score);
              printf("%c\n", grade);
            }
          }
        }
      }
      printf("------------------\n");
    }
  }
  // If not found, provide feedback
  if (flag == 0) {
    printf("not found\n");
    printf("------------------\n");
  }
}

// Function used for course id and semester search command
void cidSemSearch(char cid[MAXSTR], char sem[MAXSTR], Student *StdArray,
                  int stdn, Enrollment *EnrArray, int enrn, Course *CrsArray,
                  int crsn) {
  int flag = 0;
  printf("------------------\n");
  for (int i = 0; i < crsn; i++) {

    // If key matches a cid...
    if (strcmp(cid, CrsArray[i].cid) == 0) {
      flag++;
      // Prints course name and credit
      printf("%s ", CrsArray[i].cname);
      printf("%.0f\n", CrsArray[i].tot_cred);

      // Prints student name, semester, and grade
      for (int j = 0; j < enrn; j++) {
        // If the class id matches an enrolled cid...
        if (strcmp(CrsArray[i].cid, EnrArray[j].cid) == 0) {
          for (int k = 0; k < stdn; k++) {
            // And if the enrolled stid matches a student ID,
            // print the student name, specified semester, and grade letter.
            if (EnrArray[j].stid == StdArray[k].stid &&
                (strcmp(sem, EnrArray[j].semester) == 0)) {
              printf("%s ", StdArray[k].fname);
              printf("%s ", StdArray[k].lname);
              printf("%s ", EnrArray[j].semester);
              printf("%.0f ", EnrArray[j].score);
              char grade = getScore(EnrArray[j].score);
              printf("%c\n", grade);
            }
          }
        }
      }
      printf("------------------\n");
    }
  }
  // If not found, provide feedback
  if (flag == 0) {
    printf("not found\n");
    printf("------------------\n");
  }
}
