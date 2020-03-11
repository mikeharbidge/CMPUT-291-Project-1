//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//searchSales executes
#include "SearchSales.h"

int searchSales()
{
    char SQL[1000] = "SELECT s.sid, s.descr  FROM sales s, products p WHERE s.pid = p.pid AND s.edate > time(\'now\') AND (";

    int n, i = 0;

    int rc = 0;
    char *zErrMsg = 0;
    const char* data = "Callback function called";

    printf("Enter amount of keywords to search with: ");
    scanf("%d", &n);

    char *a[n];
    char keyword[100]; //buffer used when inputting strings
    int slen;

    printf("Enter %d keywords, one keyword per line. ", n);
    for (i = 0; i < n; i++)
    {
        printf("Enter Keyword %d: ", i+1);
        scanf("%99s", keyword);
        
        slen = strlen(keyword);
        a[i] = malloc(slen+1);

        //check if malloc succeeded
        if (!a[i]) {
            printf("Error: Cannot allocate memory for string.\n");
            return 0;
        }
        strcpy(a[i], keyword);
    }

    printf("Keywords: ");
    for(i = 0; i < n; i++)
        printf("%s ", a[i]);
    printf("\n");
    char section[150];
    //Add each keyword in both parts, then add the whole thing together, then exec
    for(i = 0; i < n; i++)
    {
        sprintf(section, "s.descr LIKE \"%%%s%%\" OR ", a[i]);
        strcat(SQL, section);
    }

    for(i = 0; i < n; i++)
    {
        sprintf(section, "p.descr LIKE \"%%%s%%\" ", a[i]);
        strcat(SQL, section);
        if (i < n-1)
            strcat(SQL,"OR "); //dont need another OR at the end
    }

    strcat(SQL, ");"); //add ending to statement
    //printf("SQL: %s\n", SQL);

    rc = sqlite3_exec(db, SQL,callback,(void *)data, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        
    }

    for(i = 0; i < n; i++)
        free(a[i]);

    

    return 0;
}

static int callback(void *data, int argc, char **argv, char **aColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for(i = 0; i<argc; i++)
    {
    printf("%s = %s, ", aColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}