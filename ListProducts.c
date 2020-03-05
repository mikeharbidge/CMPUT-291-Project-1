//CMPUT 291 Mini-Project 1 Harbidge, sayedpar, Wielgus
//listproducts does the list products function described in the specifications

#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>

void list_products(sqlite3 *db)
{
    const char* listProductsQuery = "SELECT DISTINCT p.pid, p.descr, count(pr.rid), avg(pr.rating), number_of_asales.cnt \
    FROM products p, previews pr, \
    (SELECT p.pid as pid, count(*) as cnt \
    FROM sales s \
    WHERE p.pid = s.sid AND julianday(s.edate) > julianday('now')) number_of_asales \
    WHERE p.pid = pr.pid AND p.pid = number_of_asales.pid \
    GROUP BY p.pid, p.pdescr, pr.rid, number_of_asales.cnt \
    ORDER BY number_of_asales.cnt|DESC; \
    ";
    printf("Product ID\t\tProduct Description\t\tReview count\t\tAverage rating\t\tNumber of active sales\n");
    sqlite3_exec(db, listProductsQuery,list_products_callback, NULL, NULL); //TODO: Error handling needed
    product_actions();
}

static long list_products_callback (void *list, long count, char **data, char **columns)
{
    for(long i = 0; i < count; i++){
        printf("%s\t\t", data[i]);
        if (i == count-1)
        {
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}

void product_actions()
{
    bool performingAction = true;
    while (performingAction)
    {
        const char* pidPromptMsg = "Choose a product ID to perform action on (or 0 to abort):\n";
        const char* pidRepromptMsg = "Wrong input!\nEnter a valid product ID (or 0 to abort):\n";
        long pid = get_int_input(pidPromptMsg, pidRepromptMsg, __LONG_MAX__);
        if(pid > 0)
        {
            printf("0. Abort\n");
            printf("1. Write a product review\n");
            printf("2. List reviews of the product\n");
            printf("3. List all active sales associated with the product\n");
            long action = get_int_input("Choose an action:\n","Choose a valid option:\n", 3);
            switch (action)
            {
            case 0:
                abort();
                break;
            case 1:
                //TODO: Write a product review
                break;
            case 2:
                //TODO: List Reviews of the product
                break;
            case 3:
                //TODO: List all active sales
                break;
            default:
                continue;
            }
        }
        else
        {
            abort();
        }
        
    }
}

long get_int_input(char* promptMsg, char* repromptMsg, long upperBound)
{
    long input = 0;
    printf(promptMsg);
    long scanfOutput = scanf("%d", &input);
    while (!scanfOutput || input < 0 || input >= upperBound)
    {
        printf(repromptMsg);
        scanfOutput = scanf("%d", &input);
    }
    return input;
}