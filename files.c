#include <stdio.h>
#include "lms.h"

void save_data(void)
{
    FILE *fp = fopen(BOOKS_FILE, "w");
    if (!fp) {
        printf("Error: Cannot write %s\n", BOOKS_FILE);
        return;
    }
    for (int i = 0; i < book_count; i++)
        fprintf(fp, "%d|%s|%s|%d\n", books[i].id, books[i].title,
                books[i].author, books[i].quantity);
    fclose(fp);

    fp = fopen(ISSUED_FILE, "w");
    if (!fp) {
        printf("Error: Cannot write %s\n", ISSUED_FILE);
        return;
    }
    for (int i = 0; i < issue_count; i++)
        fprintf(fp, "%d|%d|%d|%s|%d-%d|%d-%d|%d-%d|%d|%d\n",
                issues[i].issue_id, issues[i].book_id, issues[i].user_id,
                issues[i].user_name,
                issues[i].issue_date.day, issues[i].issue_date.month,
                issues[i].due_date.day, issues[i].due_date.month,
                issues[i].return_date.day, issues[i].return_date.month,
                issues[i].fine, issues[i].returned);
    fclose(fp);

    unsaved_changes = 0;
    printf("Data saved successfully.\n");
}

void load_data(void)
{
    char line[512];
    FILE *fp;

    book_count = 0;
    issue_count = 0;

    fp = fopen(BOOKS_FILE, "r");
    if (fp) {
        while (book_count < MAX_BOOKS && fgets(line, sizeof line, fp)) {
            Book b;
            if (sscanf(line, "%d|%99[^|]|%59[^|]|%d",
                       &b.id, b.title, b.author, &b.quantity) == 4)
                books[book_count++] = b;
        }
        fclose(fp);
    }

    fp = fopen(ISSUED_FILE, "r");
    if (fp) {
        while (issue_count < MAX_ISSUES && fgets(line, sizeof line, fp)) {
            Issue r;
            if (sscanf(line, "%d|%d|%d|%59[^|]|%d-%d|%d-%d|%d-%d|%d|%d",
                       &r.issue_id, &r.book_id, &r.user_id, r.user_name,
                       &r.issue_date.day, &r.issue_date.month,
                       &r.due_date.day, &r.due_date.month,
                       &r.return_date.day, &r.return_date.month,
                       &r.fine, &r.returned) == 12) {
                issues[issue_count++] = r;
            }
        }
        fclose(fp);
    }

    unsaved_changes = 0;
    printf("Loaded %d book(s) and %d issue record(s).\n", book_count, issue_count);
}
