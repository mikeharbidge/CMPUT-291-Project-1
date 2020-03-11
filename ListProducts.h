#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <stdbool.h>

#define REVIEW_SIZE 20
#define PID_LEN 4
#define SID_LEN 4

void list_products(sqlite3 *db, char* user);
static int list_products_callback(void *list, int count, char **data, char **columns);
void product_actions(sqlite3 *db, long numberOfReviews, char *reviewer);
long get_int_input(char *promptMsg, char *repromptMsg, long lowerBound, long upperBound);
int get_pid(sqlite3* db, const char *promptMsg, const char *repromptMsg, int strlen, char pid[PID_LEN]);
static int pid_validation_callback(void *list, int count, char **data, char **columns);
float get_float_input(char *promptMsg, char *repromptMsg, float lowerBound, float upperBound);
void write_preview(sqlite3 *db, char *pid, long numberOfReviews, char *reviewer);
void list_reviews_lp(sqlite3 *db, char *pid);
static int list_reviews_lp_callback(void *list, int count, char **data, char **columns);
void list_sales(sqlite3 *db, char *pid);
static int list_sales_callback(void *list, int count, char **data, char **columns);
static int number_of_reviews_callback(void *list, int count, char **data, char **columns);
int sale_action(sqlite3 *db, char *user);
static int show_sale_details_callback(void *list, int count, char **data, char **columns);
static int show_sale_details_2_callback(void *list, int count, char **data, char **columns);
int get_sid(sqlite3 *db, const char *promptMsg, const char *repromptMsg, int strlen, char sid[SID_LEN]);
static int sid_validation_callback(void *list, int count, char **data, char **columns);
static int largest_bid_callback(void *list, int count, char **data, char **columns);
static int number_of_bids_callback(void *list, int count, char **data, char **columns);
int place_bid(sqlite3 *db, char *user, char *sid);
void list_sellers_active_sales(sqlite3 *db, char *user);
static int get_seller_callback(void *list, int count, char **data, char **columns);
void list_sellers_reviews(sqlite3 *db, char *user);
static int list_sellers_reviews_callback(void *list, int count, char **data, char **columns);