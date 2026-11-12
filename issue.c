#include <stdio.h>
#include <limits.h>
#include "lms.h"

int is_book_issued(int book_id)
{
    for (int i = 0; i < issue_count; i++)
        if (issues[i].book_id == book_id && !issues[i].returned)
            return 1;
    return 0;
}

int calculate_fine(Date issue, Date returned)
{
    int late_days = days_between(issue, returned) - LOAN_DAYS;

    if (late_days <= 0)
        return 0;
    return late_days * FINE_PER_DAY;
}

void issue_book(void)
{
    Issue rec;
    int idx, max_id = 0;
    char d1[16], d2[16];

    if (issue_count >= MAX_ISSUES) {
        printf("Error: Issue record storage is full.\n");
        return;
    }

    rec.book_id = read_int("Enter Book ID: ", 1, INT_MAX);
    idx = find_book_by_id(rec.book_id);
    if (idx < 0) {
        printf("Error: Book not found.\n");
        return;
    }
    if (books[idx].quantity <= 0) {
        printf("Error: No copies of \"%s\" are available.\n", books[idx].title);
        return;
    }

    rec.user_id = read_int("Enter User ID: ", 1, INT_MAX);

    for (int i = 0; i < issue_count; i++) {
        if (!issues[i].returned && issues[i].book_id == rec.book_id &&
            issues[i].user_id == rec.user_id) {
            printf("Error: This user already has this book issued.\n");
            return;
        }
        if (issues[i].issue_id > max_id)
            max_id = issues[i].issue_id;
    }

    read_line("Enter User Name: ", rec.user_name, sizeof rec.user_name);
    if (rec.user_name[0] == '\0') {
        printf("Error: User name cannot be empty.\n");
        return;
    }

    read_date("Enter Issue Date (DD-MM): ", &rec.issue_date);

    rec.issue_id = max_id + 1;
    rec.due_date = add_days(rec.issue_date, LOAN_DAYS);
    rec.return_date.day = rec.return_date.month = 0;
    rec.fine     = 0;
    rec.returned = 0;

    issues[issue_count++] = rec;
    books[idx].quantity--;
    unsaved_changes = 1;

    format_date(rec.issue_date, d1, sizeof d1);
    format_date(rec.due_date, d2, sizeof d2);
    printf("Success: Book issued (Issue ID %d).\n", rec.issue_id);
    printf("Issue Date: %s   Due Date: %s\n", d1, d2);

    save_data();
}

void return_book(void)
{
    int book_id, user_id, found = -1, idx;
    Date ret;
    char d[16];

    book_id = read_int("Enter Book ID: ", 1, INT_MAX);
    user_id = read_int("Enter User ID: ", 1, INT_MAX);

    for (int i = 0; i < issue_count; i++) {
        if (!issues[i].returned && issues[i].book_id == book_id &&
            issues[i].user_id == user_id) {
            found = i;
            break;
        }
    }
    if (found < 0) {
        printf("Error: No active issue record for this Book ID and User ID.\n");
        return;
    }

    read_date("Enter Return Date (DD-MM): ", &ret);

    issues[found].return_date = ret;
    issues[found].fine = calculate_fine(issues[found].issue_date, ret);
    issues[found].returned = 1;

    idx = find_book_by_id(book_id);
    if (idx >= 0)
        books[idx].quantity++;

    unsaved_changes = 1;

    format_date(ret, d, sizeof d);
    printf("Success: Book returned on %s.\n", d);
    if (issues[found].fine > 0)
        printf("Book returned late! Fine: Rs. %d\n", issues[found].fine);
    else
        printf("Returned on time. No fine.\n");

    save_data();
}

void list_issued_books(void)
{
    char d1[16], d2[16], d3[16];
    int total_fine = 0;

    if (issue_count == 0) {
        printf("No books have been issued yet.\n");
        return;
    }

    printf("\n%-8s %-8s %-22s %-8s %-15s %-11s %-11s %-11s %s\n",
           "Issue ID", "Book ID", "Book Title", "User ID", "User Name",
           "Issue Date", "Due Date", "Return Date", "Fine");
    printf("--------------------------------------------------------------------"
           "------------------------------------------\n");

    for (int i = 0; i < issue_count; i++) {
        const Issue *r = &issues[i];
        int idx = find_book_by_id(r->book_id);

        format_date(r->issue_date, d1, sizeof d1);
        format_date(r->due_date, d2, sizeof d2);
        format_date(r->return_date, d3, sizeof d3);

        printf("%-8d %-8d %-22.22s %-8d %-15.15s %-11s %-11s %-11s Rs.%d\n",
               r->issue_id, r->book_id,
               idx >= 0 ? books[idx].title : "(removed)",
               r->user_id, r->user_name, d1, d2, d3, r->fine);
        total_fine += r->fine;
    }
    printf("\nTotal records: %d   Total fine: Rs. %d\n", issue_count, total_fine);
}
