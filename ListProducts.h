#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <stdbool.h>

#define REVIEW_SIZE 20
#define PID_LEN 4

void list_products(sqlite3 *db, char* user);
static int list_products_callback(void *list, int count, char **data, char **columns);
void product_actions(sqlite3 *db, long numberOfReviews, char *reviewer);
long get_int_input(char *promptMsg, char *repromptMsg, long lowerBound, long upperBound);
int get_pid(sqlite3* db, const char *promptMsg, const char *repromptMsg, int strlen, char* pid);
static int pid_validation_callback(void *list, int count, char **data, char **columns);
float get_float_input(char *promptMsg, char *repromptMsg, long lowerBound, long upperBound);
void write_review(sqlite3 *db, char *pid, long numberOfReviews, char *reviewer);
void list_reviews(sqlite3 *db, char *pid);
static int list_reviews_callback(void *list, int count, char **data, char **columns);
void list_sales(sqlite3 *db, char *pid);
static int list_sales_callback(void *list, int count, char **data, char **columns);
