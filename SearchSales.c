//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//searchSales executes 

int searchSales()
{
    char SQL_base[1000];
    char SQL_sd[400]; //separate strings for sale and product desc
    char SQL_pd[400]; //added to base to form final statement

    //needs to get keywords from user, stored in a temp array of pointers to the strings

    SQL_base = "SELECT * FROM sales s, products p
    WHERE s.pid = p.pid AND
    s.edate > time(\'now\') AND (";
    
    //Add each keyword to both parts, then add the whole thing together, then exec
    SQL_sd =
    "s.descr LIKE \"%KEYWORD%\" OR
    p.descr LIKE \"%KEYWORD%\");";
    return 0;
}

/*
SELECT * FROM sales s, products p
WHERE s.pid = p.pid AND
s.edate > time('now') AND
(s.descr LIKE "%kettle%" OR
p.descr LIKE "%kettle%");
*/