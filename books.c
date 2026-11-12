#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lms.h"

int find_book_by_id(int id)
{
    for (int i = 0; i < book_count; i++)
        if (books[i].id == id)
            return i;
    return -1;
}

int find_book_by_name(const char *name)
{
    for (int i = 0; i < book_count; i++)
        if (str_equals_ci(books[i].title, name))
            return i;
    return -1;
}

void print_book_header(void)
{
    printf("\n%-8s %-35s %-25s %s\n", "Book ID", "Title", "Author", "Qty");
    printf("---------------------------------------------------------------------------\n");
}

void print_book(const Book *b)
{
    printf("%-8d %-35.35s %-25.25s %d\n", b->id, b->title, b->author, b->quantity);
}

void add_book(void)
{
    Book b;

    if (book_count >= MAX_BOOKS) {
        printf("Error: Library is full. Cannot add more books.\n");
        return;
    }

    b.id = read_int("Enter Book ID: ", 1, INT_MAX);
    if (find_book_by_id(b.id) != -1) {
        printf("Error: A book with ID %d already exists.\n", b.id);
        return;
    }

    read_line("Enter Title: ", b.title, sizeof b.title);
    if (b.title[0] == '\0') {
        printf("Error: Title cannot be empty.\n");
        return;
    }

    read_line("Enter Author: ", b.author, sizeof b.author);
    if (b.author[0] == '\0') {
        printf("Error: Author cannot be empty.\n");
        return;
    }

    b.quantity = read_int("Enter Quantity: ", 1, 100000);

    books[book_count++] = b;
    unsaved_changes = 1;
    printf("Success: Book added.\n");
}

void update_book(void)
{
    char choice[8], buf[TITLE_LEN], num[16], *end;
    int idx = -1;
    long q;

    printf("\n--- Update Book Details ---\n");
    printf("A. By Book ID\nB. By Book Name\nC. Back to Main Menu\n");
    read_line("Enter your choice: ", choice, sizeof choice);

    switch (to_upper((unsigned char)choice[0])) {
    case 'A':
        idx = find_book_by_id(read_int("Enter Book ID: ", 1, INT_MAX));
        break;
    case 'B':
        read_line("Enter Book Name: ", buf, sizeof buf);
        idx = find_book_by_name(buf);
        break;
    case 'C':
        return;
    default:
        printf("Invalid choice!\n");
        return;
    }

    if (idx < 0) {
        printf("Error: Book not found.\n");
        return;
    }

    printf("\nCurrent details:");
    print_book_header();
    print_book(&books[idx]);
    printf("\n(Press Enter to keep the current value)\n");

    read_line("New Title: ", buf, sizeof buf);
    if (buf[0] != '\0')
        snprintf(books[idx].title, sizeof books[idx].title, "%s", buf);

    read_line("New Author: ", buf, sizeof buf);
    if (buf[0] != '\0')
        snprintf(books[idx].author, sizeof books[idx].author, "%.*s", AUTHOR_LEN - 1, buf);

    read_line("New Quantity: ", num, sizeof num);
    if (num[0] != '\0') {
        q = strtol(num, &end, 10);
        if (*end != '\0' || q < 0 || q > 100000)
            printf("Invalid quantity. Quantity not changed.\n");
        else
            books[idx].quantity = (int)q;
    }

    unsaved_changes = 1;
    printf("Success: Book details updated.\n");
}

void remove_book(void)
{
    char choice[8], buf[TITLE_LEN], ans[8];
    int idx = -1;

    printf("\n--- Remove Book ---\n");
    printf("A. By Book ID\nB. By Book Name\nC. Back to Main Menu\n");
    read_line("Enter your choice: ", choice, sizeof choice);

    switch (to_upper((unsigned char)choice[0])) {
    case 'A':
        idx = find_book_by_id(read_int("Enter Book ID: ", 1, INT_MAX));
        break;
    case 'B':
        read_line("Enter Book Name: ", buf, sizeof buf);
        idx = find_book_by_name(buf);
        break;
    case 'C':
        return;
    default:
        printf("Invalid choice!\n");
        return;
    }

    if (idx < 0) {
        printf("Error: Book not found.\n");
        return;
    }

    if (is_book_issued(books[idx].id)) {
        printf("Error: Cannot remove - copies of this book are currently issued.\n");
        return;
    }

    print_book_header();
    print_book(&books[idx]);
    read_line("Are you sure you want to delete this book? (y/n): ", ans, sizeof ans);
    if (to_lower((unsigned char)ans[0]) != 'y') {
        printf("Cancelled.\n");
        return;
    }

    for (int i = idx; i < book_count - 1; i++)
        books[i] = books[i + 1];
    book_count--;

    unsaved_changes = 1;
    printf("Success: Book removed.\n");
}

void search_book(void)
{
    char choice[8], key[TITLE_LEN];
    int found = 0;
    int id = 0;
    char mode;

    printf("\n--- Search Book ---\n");
    printf("A. By Book ID\nB. By Book Name\nC. By Author Name\nD. Back to Main Menu\n");
    read_line("Enter your choice: ", choice, sizeof choice);
    mode = (char)to_upper((unsigned char)choice[0]);

    if (mode == 'D')
        return;
    if (mode == 'A')
        id = read_int("Enter Book ID: ", 1, INT_MAX);
    else if (mode == 'B')
        read_line("Enter part of Book Name: ", key, sizeof key);
    else if (mode == 'C')
        read_line("Enter part of Author Name: ", key, sizeof key);
    else {
        printf("Invalid choice!\n");
        return;
    }

    for (int i = 0; i < book_count; i++) {
        int match = (mode == 'A') ? (books[i].id == id)
                  : (mode == 'B') ? str_contains_ci(books[i].title, key)
                                  : str_contains_ci(books[i].author, key);
        if (match) {
            if (!found)
                print_book_header();
            print_book(&books[i]);
            found++;
        }
    }

    if (!found)
        printf("No matching book found.\n");
    else
        printf("\n%d book(s) found.\n", found);
}

void view_all_books(void)
{
    if (book_count == 0) {
        printf("No books in the library.\n");
        return;
    }

    print_book_header();
    for (int i = 0; i < book_count; i++)
        print_book(&books[i]);
    printf("\nTotal records: %d\n", book_count);
}
