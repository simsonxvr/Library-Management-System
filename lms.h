
#define MAX_BOOKS    500
#define MAX_ISSUES   2000
#define TITLE_LEN    100
#define AUTHOR_LEN   60
#define NAME_LEN     60
#define LOAN_DAYS    7
#define FINE_PER_DAY 5
#define BOOKS_FILE   "books.txt"
#define ISSUED_FILE  "issued.txt"

typedef struct {
    int day;
    int month;
} Date;

typedef struct {
    int  id;
    char title[TITLE_LEN];
    char author[AUTHOR_LEN];
    int  quantity;
} Book;

typedef struct {
    int  issue_id;
    int  book_id;
    int  user_id;
    char user_name[NAME_LEN];
    Date issue_date;
    Date due_date;
    Date return_date;
    int  fine;
    int  returned;
} Issue;

extern Book  books[MAX_BOOKS];
extern int   book_count;
extern Issue issues[MAX_ISSUES];
extern int   issue_count;
extern int   unsaved_changes;

void add_book(void);
void update_book(void);
void remove_book(void);
void search_book(void);
void view_all_books(void);
void issue_book(void);
void return_book(void);
void list_issued_books(void);
void save_data(void);
void load_data(void);
void show_menu(void);

void read_line(const char *prompt, char *buf, int size);
int  read_int(const char *prompt, int min, int max);
int  to_lower(int c);
int  to_upper(int c);
int  str_contains_ci(const char *text, const char *part);
int  str_equals_ci(const char *a, const char *b);

void format_date(Date d, char *buf, int size);
int  days_in_month(int month);
int  is_valid_date(Date d);
void read_date(const char *prompt, Date *d);
int  date_to_days(Date d);
int  days_between(Date a, Date b);
Date add_days(Date d, int n);

int  find_book_by_id(int id);
int  find_book_by_name(const char *name);
int  is_book_issued(int book_id);
int  calculate_fine(Date issue, Date returned);
void print_book_header(void);
void print_book(const Book *b);
