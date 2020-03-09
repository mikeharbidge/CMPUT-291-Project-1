//CMPUT 291 Mini-Project 1 Harbidge, sayedpar, Wielgus
//listproducts does the list products function described in the specifications
/*
Resources: 
https://stackoverflow.com/questions/50016807/how-can-i-set-a-variable-placeholder-inside-a-string-in-c, Medalib
*/

#include "ListProducts.h"

void list_products(sqlite3 *db, char *user)
{
    /*
    The only function that needs to be called inside main
    It takes the database and the current user email as arguments
    */
    const char *listProductsQuery = "SELECT DISTINCT p.pid, p.descr, count(pr.rid), avg(pr.rating), count(s.edate) \
    FROM products p LEFT OUTER JOIN previews pr ON p.pid = pr.pid LEFT OUTER JOIN sales s ON p.pid = s.pid \
    WHERE s.edate > datetime('now') \
    GROUP BY p.pid, p.descr, s.sid \
    ORDER BY count(s.edate) DESC; \
    ";
    printf("Product ID\t\tProduct Description\t\tReview count\t\tAverage rating\t\tNumber of active sales\n");
    sqlite3_exec(db, listProductsQuery, list_products_callback, NULL, NULL); //TODO: Error handling needed
    const char *numberOfReviewsQuery = "SELECT count(*) \
    FROM previews";
    //TODO: number of reviews
    product_actions(db, 100, user);
}

static int list_products_callback(void *list, int count, char **data, char **columns)
{
    printf("%d\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("%s\t\t", data[i]);
        if (i == count - 1)
        {
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}

void product_actions(sqlite3 *db, long numberOfReviews, char *reviewer)
{
    bool performingAction = true;
    while (performingAction)
    {
        const char *pidPromptMsg = "Choose a product ID to perform action on (or 0 to abort):\n";
        const char *pidRepromptMsg = "Wrong input!\nEnter a valid product ID (or 0 to abort):\n";
        char *pid = 0;
        get_pid(db, pidPromptMsg, pidRepromptMsg, PID_LEN, pid);
        if (pid != NULL)
        {
            printf("0. Abort\n");
            printf("1. Write a product review\n");
            printf("2. List reviews of the product\n");
            printf("3. List all active sales associated with the product\n");
            long action = get_int_input("Choose an action:\n", "Choose a valid option:\n", 0, 3);
            switch (action)
            {
            case 0:
                return;
            case 1:
                write_review(db, pid, numberOfReviews, reviewer);
                break;
            case 2:
                list_reviews(db, pid);
                break;
            case 3:
                list_sales(db, pid);
                break;
            default:
                continue;
            }
        }
        else
        {
            return; //Abort
        }
    }
}

long get_int_input(char *promptMsg, char *repromptMsg, long lowerBound, long upperBound)
{
    long input = 0;
    printf("%s", promptMsg);
    int scanfOutput = scanf("%ld", &input);
    while (!scanfOutput || input < lowerBound || input > upperBound)
    {
        printf("%s", repromptMsg);
        scanfOutput = scanf("%ld", &input);
    }
    return input;
}

int get_pid(sqlite3 *db, const char *promptMsg, const char *repromptMsg, int strlen, char *pid)
{
    char input[strlen];
    printf("%s", promptMsg);
    int scanfOutput = scanf("%s", input);
    char *zErrMsg = 0;
    int rc = 0;
    while (!scanfOutput)
    {
        printf("%s", repromptMsg);
        scanfOutput = scanf("%s", input);
    }
    char *pidValidation = sqlite3_mprintf("SELECT count(*) \
    FROM products \
    WHERE pid = \"%s\"",
                                          input);
    rc = sqlite3_exec(db, pidValidation, pid_validation_callback, NULL, &zErrMsg);
    if (rc == SQLITE_OK)
    {
        pid = input;
        return SQLITE_OK;
    }
    else
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        pid = "";
        return SQLITE_ERROR;
    }
}

static int pid_validation_callback(void *list, int count, char **data, char **columns)
{
    if (data[0] > 0)
    {
        return SQLITE_OK;
    }
    else
    {
        return SQLITE_ERROR;
    }
}

float get_float_input(char *promptMsg, char *repromptMsg, long lowerBound, long upperBound)
{
    float input = 0;
    printf("%s", promptMsg);
    int scanfOutput = scanf("%f", &input);
    while (!scanfOutput || input < lowerBound || input > upperBound)
    {
        printf("%s", repromptMsg);
        scanfOutput = scanf("%f", &input);
    }
    return input;
}

void write_review(sqlite3 *db, char *pid, long numberOfReviews, char *reviewer)
{
    printf("Review text:\n");
    char text[REVIEW_SIZE] = {0};
    scanf("%s", text);
    puts("Rating: ");
    float rating = 0;
    rating = get_float_input(NULL, "Rating must be a number between 1 to 5:\n", 1, 5);
    const char *writeReviewQuery = sqlite3_mprintf("insert into previews values \
    (%d, %s , %s , %f, %s,  datetime('now');",
                                                   numberOfReviews + 1, pid, reviewer, rating, text);
    sqlite3_exec(db, writeReviewQuery, NULL, NULL, NULL); //Error handling
}

void list_reviews(sqlite3 *db, char *pid)
{
    const char *listReviewsQuery = sqlite3_mprintf("SELECT * \
    FROM previews pr \
    WHERE pr.pid = \"%s\"",
                                                   pid);
    printf("Review ID\tProduct ID\tReviewer\tRating\tReview Text\tReview Date\n");
    sqlite3_exec(db, listReviewsQuery, list_reviews_callback, NULL, NULL); //TODO: Error handling
}

static int list_reviews_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s\t\t", data[i]);
        if (i == count - 1)
        {
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}

void list_sales(sqlite3 *db, char *pid)
{
    const char *listSalesQuery = sqlite3_mprintf("SELECT s.sid AS Sale ID, s.lister AS Lister \
    s.descr AS Description, count(b.bid) AS number of bids, s.rprice AS Reserved price, \
    max(b.amount) AS Max bid, datetime(s.edate - 'now') AS sale ends \
    FROM sales s LEFT OUTER JOIN bids b USING s.sid = b.sid \
    WHERE s.pid = \"%s\"\
    GROUP BY s.sid \
    ORDER BY sale ends",
                                                 pid);
    sqlite3_exec(db, listSalesQuery, list_sales_callback, NULL, NULL);
}

static int list_sales_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        if (i == 3)
        {
            if (data[i] > 0)
            {
                i = 5;
                printf("%s: %s\n", columns[i], data[i]);
                continue;
            }
            else
            {
                printf("%s: %s\n", columns[4], data[4]);
                i = 5;
                continue;
            }
        }
        else
        {
            printf("%s: %s\n", columns[i], data[i]);
        }
    }
    return 0;
}
