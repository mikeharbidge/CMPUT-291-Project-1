//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//searchSales executes
#include "SearchSales.h"

int searchSales()
{
    char SQL_base[1000] = "SELECT * FROM sales s, products p WHERE s.pid = p.pid AND s.edate > time(\'now\') AND (";
    char SQL_sd[400]; //separate strings for sale and product desc
    char SQL_pd[400]; //added to base to form final statement

    int n, i;

    printf("Enter amount of keywords to search with: ");
    scanf("%d", &n);

    char *a[n];
    char keyword[100]; //buffer used when inputting strings
    int slen;

    printf("Enter %d keywords, one keyword per line: ", n);
    for (i = 0; i <= n; i++)
    {
        //scanf("%s",&a[i]);
        fgets(keyword, 100, stdin);

        //change newline added by fgets to null terminator
        slen = strlen(keyword);
        keyword[slen-1] = '\0';

        a[i] = malloc(slen+1);

        //check if malloc succeeded
        if (!a[i]) {
            printf("Cannot allocate memory for string.\n");
            return 0;
        }

        strcpy(a[i], keyword);
    }

    printf("Keywords: ");
    for(i = 0; i <= n; i++)
        printf("%s ", a[i]);
    
    //Add each keyword to both parts, then add the whole thing together, then exec
    //SQL_sd ="s.descr LIKE \"\" OR p.descr LIKE \"\");";
    
    for(i = 0; i <= n; i++)
        free(a[i]);

    return 0;
}

/*
SELECT * FROM sales s, products p
WHERE s.pid = p.pid AND
s.edate > time('now') AND
(s.descr LIKE "%kettle%" OR
p.descr LIKE "%kettle%");
*/