#include <stdio.h>
#include "lms.h"

Book  books[MAX_BOOKS];
int   book_count = 0;
Issue issues[MAX_ISSUES];
int   issue_count = 0;
int   unsaved_changes = 0;

void show_menu(void)
{
    printf("\n-----------------------------------------\n");
    printf("|         Book Management Menu           |\n");
    printf("|----------------------------------------|\n");
    printf("| 1. Add New Book                        |\n");
    printf("| 2. Update Book Details                 |\n");
    printf("| 3. Remove Book                         |\n");
    printf("| 4. Search Book                         |\n");
    printf("| 5. View All Books                      |\n");
    printf("| 6. Issue Book                          |\n");
    printf("| 7. Return Book                         |\n");
    printf("| 8. List Issued Books                   |\n");
    printf("| 9. Save                                |\n");
    printf("| 10. Exit                               |\n");
    printf("------------------------------------------\n");
}

int main(void)
{
    int choice;

    load_data();

    for (;;) {
        show_menu();
        choice = read_int("Enter your choice (1-10): ", 1, 10);

        switch (choice) {
        case 1:  add_book();          break;
        case 2:  update_book();       break;
        case 3:  remove_book();       break;
        case 4:  search_book();       break;
        case 5:  view_all_books();    break;
        case 6:  issue_book();        break;
        case 7:  return_book();       break;
        case 8:  list_issued_books(); break;
        case 9:  save_data();         break;
        case 10: {
            if (unsaved_changes) {
                char ans[8];
                read_line("You have unsaved changes. Save before exit? (y/n): ",
                          ans, sizeof ans);
                if (to_lower((unsigned char)ans[0]) == 'y')
                    save_data();
            }
            printf("Goodbye!\n");
            return 0;
        }
        }
    }
}
