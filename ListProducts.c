//CMPUT 291 Mini-Project 1 Harbidge, sayedpar, Wielgus
//listproducts does the list products function described in the specifications
/*
Resources: 
https://stackoverflow.com/questions/50016807/how-can-i-set-a-variable-placeholder-inside-a-string-in-c, Medalib
*/

#include "ListProducts.h"

int numberOfReviews = 0;
float largestBid = 0;
int numberOfbids = 0;
char saleLister[20] = {0};

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
    printf("Product ID\t\tProduct Description\t\tReview count\t\t\t\tAverage rating\t\tNumber of active sales\n");
    sqlite3_exec(db, listProductsQuery, list_products_callback, NULL, NULL); //TODO: Error handling needed
    const char *numberOfReviewsQuery = "SELECT * \
    FROM previews";
    sqlite3_exec(db, numberOfReviewsQuery, number_of_reviews_callback, NULL, NULL);
    product_actions(db, numberOfReviews, user);
    return;
}

static int number_of_reviews_callback(void *list, int count, char **data, char **columns)
{
    numberOfReviews++;
    return 0;
}

static int list_products_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s\t\t\t\t", data[i]);
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
        char pid[PID_LEN] = {0};
        int rc = get_pid(db, pidPromptMsg, pidRepromptMsg, PID_LEN, pid);
        if (rc == SQLITE_OK && pid[0] != '\0')
        {
            printf("0. Abort\n");
            printf("1. Write a product review\n");
            printf("2. List reviews of the product\n");
            printf("3. List all active sales associated with the product\n");
            int action = get_int_input("Choose an action:\n", "Choose a valid option:\n", 0, 3);
            switch (action)
            {
            case 0:
                return;
            case 1:
                write_preview(db, pid, numberOfReviews, reviewer);
                break;
            case 2:
                list_reviews(db, pid);
                break;
            case 3:
                list_sales(db, pid);
                sale_action(db, reviewer);
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

int get_pid(sqlite3 *db, const char *promptMsg, const char *repromptMsg, int strlen, char pid[PID_LEN])
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
    if (input[0] == '0')
    {
        return 0;
    }
    char *pidValidation = sqlite3_mprintf("SELECT count(*) \
    FROM products \
    WHERE pid = \"%s\"",
                                          input);
    rc = sqlite3_exec(db, pidValidation, pid_validation_callback, NULL, &zErrMsg);
    if (rc == SQLITE_OK)
    {
        for (int i = 0; i < PID_LEN; i++)
        {
            pid[i] = input[i];
        }
        return SQLITE_OK;
    }
    else
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return SQLITE_ERROR;
    }
}

static int pid_validation_callback(void *list, int count, char **data, char **columns)
{
    if (data[0][0] > '0')
    {
        return SQLITE_OK;
    }
    else
    {
        return SQLITE_ERROR;
    }
}

float get_float_input(char *promptMsg, char *repromptMsg, float lowerBound, float upperBound)
{
    float input = 0;
    if (promptMsg != NULL)
    {
        printf("%s ", promptMsg);
    }
    int scanfOutput = scanf("%f", &input);
    while (!scanfOutput || input < lowerBound || input > upperBound)
    {
        printf("%s", repromptMsg);
        scanfOutput = scanf("%f", &input);
    }
    return input;
}

void write_preview(sqlite3 *db, char *pid, long numberOfReviews, char *reviewer)
{
    printf("Review text:\n");
    char text[REVIEW_SIZE] = {0};
    scanf("%s", text);
    printf("Rating: ");
    float rating = 0;
    rating = get_float_input(NULL, "Rating must be a number between 1 to 5:\n", 1, 5);
    const char *writeReviewQuery = sqlite3_mprintf("INSERT INTO previews VALUES \
    (%d, '%s' , '%s' , %f, '%s',  datetime('now'));",
                                                   (int)(++numberOfReviews), pid, reviewer, rating, text);
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, writeReviewQuery, NULL, NULL, &zErrMsg); 
    if (rc == SQLITE_OK)
    {
        printf("Review submitted Successfully!\n");
        return;
    }
    else
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        return;
    }
}

void list_reviews(sqlite3 *db, char *pid)
{
    const char *listReviewsQuery = sqlite3_mprintf("SELECT * \
    FROM previews pr \
    WHERE pr.pid = \"%s\"",
                                                   pid);
    printf("Review ID\tProduct ID\tReviewer\tRating\t\tReview Text\tReview Date\n");
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
    const char *listSalesQuery = sqlite3_mprintf("SELECT s.sid AS Sale_ID, s.lister AS Lister, \
    s.descr AS Description, count(b.bid) AS number_of_bids, s.rprice AS Reserved_price, \
    ifnull(max(b.amount), 0) AS Max_bid, ifnull(cast(julianday(s.edate)-julianday('now') as int),0) AS sale_ends \
    FROM sales s LEFT OUTER JOIN bids b ON s.sid = b.sid \
    WHERE s.pid = \"%s\"\
    GROUP BY s.sid \
    ORDER BY sale_ends",
                                                 pid);
    int rc = sqlite3_exec(db, listSalesQuery, list_sales_callback, NULL, NULL);
    if (rc == SQLITE_OK)
    {
        return;
    }
    else
    {
        fprintf(stderr, "Error\n");
    }
}

static int list_sales_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        if (i == 3)
        {
            if (data[i][0] > '0')
            {
                i = 5;
                printf("%s: %s\n", columns[i], data[i] != NULL ? data[i] : 0);
                continue;
            }
            else
            {
                printf("%s: %s\n", columns[4], data[4] != NULL ? data[4] : 0);
                i = 5;
                continue;
            }
        }
        else
        {
            printf("%s: %s\n", columns[i], data[i]);
        }
        if (i == count - 1)
        {
            printf("\n");
        }
    }
    return 0;
}

int sale_action(sqlite3 *db, char *user)
{
    const char *pidPromptMsg = "Choose a sale ID to perform action on (or 0 to abort):\n";
    const char *pidRepromptMsg = "Wrong input!\nEnter a valid sale ID (or 0 to abort):\n";
    char sid[SID_LEN] = {0};
    int rc = get_sid(db, pidPromptMsg, pidRepromptMsg, PID_LEN, sid);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error\n");
        return SQLITE_ERROR;
    }
    const char *getSellerQuery = sqlite3_mprintf("SELECT lister \
    FROM sales \
    WHERE sid = '%s'",
                                                 sid);
    sqlite3_exec(db, getSellerQuery, get_seller_callback, NULL, NULL);
    char *zErrMsg = 0;
    char *zErrMsg2 = 0;
    const char *showSaleDetailsQuery = sqlite3_mprintf("SELECT s.lister AS Lister, count(r.reviewee) AS Number_of_reviews, avg(r.rating) AS Average_rating, \
    s.descr AS Description, s.edate AS End_date, s.cond AS Condition, max(b.amount) AS Max_bid, s.rprice AS Reserved_price \
    FROM sales s LEFT OUTER JOIN bids b ON b.sid = s.sid LEFT OUTER JOIN reviews r ON r.reviewee = s.lister \
    WHERE s.sid = \"%s\" AND r.reviewee = s.lister \
    GROUP BY r.reviewee;",
                                                       sid);
    const char *showSaleDetailsQuery2 = sqlite3_mprintf("SELECT p.pid AS Product_ID, p.descr AS Description, count(pr.rid) AS Number_of_reviews, avg(pr.rating) AS Average_rating \
    FROM sales s LEFT OUTER JOIN products p ON s.pid = p.pid LEFT OUTER JOIN previews pr ON pr.pid = p.pid \
    WHERE s.sid = \"%s\" \
    GROUP BY p.pid;",
                                                        sid);
    rc = sqlite3_exec(db, showSaleDetailsQuery, show_sale_details_callback, NULL, &zErrMsg);
    printf("Product:\n");
    int rc2 = sqlite3_exec(db, showSaleDetailsQuery2, show_sale_details_2_callback, NULL, &zErrMsg2);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return SQLITE_ERROR;
    }
    if (rc2 != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg2);
        sqlite3_free(zErrMsg2);
        return SQLITE_ERROR;
    }
    printf("0. Abort\n");
    printf("1. Place a bid\n");
    printf("2. List all active sales of the seller\n");
    printf("3. List reviews of the seller\n");
    int action = get_int_input("Choose an action:\n", "Choose a valid option:\n", 0, 3);
    switch (action)
    {
    case 0:
        return 0;
        break;
    case 1:
        place_bid(db, user, sid);
        break;
    case 2:
        list_sellers_active_sales(db, saleLister);
        break;
    case 3:
        list_sellers_reviews(db, saleLister);
        break;
    default:
        break;
    }
    return 0;
}

static int get_seller_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < 20; i++)
    {
        saleLister[i] = data[0][i];
    }
    return 0;
}

static int show_sale_details_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        if (i == 2)
        {
            if (data[i][0] != '0')
            {
                printf("%s: %s\n", columns[i], data[i]);
            }
            else
            {
                printf("Product is not reviewed\n");
                return 0;
            }
        }
        else
        {
            printf("%s: %s\n", columns[i], data[i]);
        }
    }
    return 0;
}

static int show_sale_details_2_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        if (i == 6)
        {
            if (data[i][0] != '\0')
            {
                printf("%s: %s\n", columns[i], data[i]);
                return 0;
            }
            else
            {
                printf("%s: %s\n", columns[7], data[7]);
                return 0;
            }
        }
        else
        {
            printf("%s: %s\n", columns[i], data[i]);
        }
        if (i == count - 1)
        {
            printf("\n");
        }
    }
    return 0;
}

int get_sid(sqlite3 *db, const char *promptMsg, const char *repromptMsg, int strlen, char sid[SID_LEN])
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
    if (input[0] == '0')
    {
        return 0;
    }
    char *sidValidation = sqlite3_mprintf("SELECT count(*) \
    FROM sales \
    WHERE sid = \"%s\"",
                                          input);
    rc = sqlite3_exec(db, sidValidation, sid_validation_callback, NULL, &zErrMsg);
    if (rc == SQLITE_OK)
    {
        for (int i = 0; i < SID_LEN; i++)
        {
            sid[i] = input[i];
        }
        return SQLITE_OK;
    }
    else
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return SQLITE_ERROR;
    }
}

static int sid_validation_callback(void *list, int count, char **data, char **columns)
{
    if (data[0][0] > '0')
    {
        return SQLITE_OK;
    }
    else
    {
        return SQLITE_ERROR;
    }
}

int place_bid(sqlite3 *db, char *user, char *sid)
{
    const char *largestBidQeury = sqlite3_mprintf("SELECT max(b.amount) \
    FROM sales s LEFT OUTER JOIN bids b ON s.sid = b.sid \
    WHERE s.sid = \"%s\"",
                                                  sid);

    float bid = get_float_input("Enter the amount\n", "A bid must be positive\n", 0, __FLT_MAX__);
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, largestBidQeury, largest_bid_callback, NULL, &zErrMsg);
    if (rc == SQLITE_OK)
    {
        const char *numberOfBidsQuery = "SELECT * \
        FROM bids";
        sqlite3_exec(db, numberOfBidsQuery, number_of_bids_callback, NULL, NULL);
        if (bid > largestBid)
        {
            char *placeBidQuery = sqlite3_mprintf("insert into bids values ('B%d', '%s', '%s', datetime('now'), %f);",
                                                  ++numberOfbids, user, sid, bid);
            char *zErrMsg = 0;
            int rc2 = sqlite3_exec(db, placeBidQuery, NULL, NULL, &zErrMsg);
            if (rc2 == SQLITE_OK)
            {
                printf("Bid placed successfully!\n");
                return SQLITE_OK;
            }
            else
            {
                fprintf(stderr, "SQL Error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
                return SQLITE_ERROR;
            }
        }
        return SQLITE_OK;
    }
    else
    {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return SQLITE_ERROR;
    }
}

static int number_of_bids_callback(void *list, int count, char **data, char **columns)
{
    numberOfbids++;
    return 0;
}

static int largest_bid_callback(void *list, int count, char **data, char **columns)
{
    largestBid = strtof(data[0], NULL);
    return 0;
}

void list_sellers_active_sales(sqlite3 *db, char *user)
{
    const char *listActiveSalesQuery = sqlite3_mprintf("SELECT s.sid AS Sale_ID, s.lister AS Lister, \
    s.descr AS Description, count(b.bid) AS number_of_bids, s.rprice AS Reserved_price, \
    ifnull(max(b.amount), 0) AS Max_bid, ifnull(cast(julianday(s.edate)-julianday('now') as int),0) AS sale_ends \
    FROM sales s LEFT OUTER JOIN bids b ON s.sid = b.sid \
    WHERE julianday(s.edate) > julianday('now') AND s.lister = '%s' \
    GROUP BY s.sid \
    ORDER BY sale_ends",
                                                       user);
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, listActiveSalesQuery, list_sales_callback, NULL, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr,"SQL Error: %s", zErrMsg);
    }
}

void list_sellers_reviews(sqlite3 *db, char *user)
{
    char *listSellersReviewsQuery = sqlite3_mprintf("SELECT * \
    FROM reviews r \
    WHERE r.reviewee = '%s'",
                                                    user);
    sqlite3_exec(db, listSellersReviewsQuery, list_sellers_reviews_callback, NULL, NULL);
}

static int list_sellers_reviews_callback(void *list, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s: %s\n", columns[i], data[i]);
    }
    printf("\n");
    return 0;
}