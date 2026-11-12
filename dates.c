#include <stdio.h>
#include "lms.h"

void format_date(Date d, char *buf, int size)
{
    if (d.month == 0)
        snprintf(buf, size, "-");
    else
        snprintf(buf, size, "%02d-%02d", d.day, d.month);
}

int days_in_month(int month)
{
    static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return days[month - 1];
}

int is_valid_date(Date d)
{
    if (d.month < 1 || d.month > 12)
        return 0;
    if (d.day < 1 || d.day > days_in_month(d.month))
        return 0;
    return 1;
}

void read_date(const char *prompt, Date *d)
{
    char buf[32];
    Date t;

    for (;;) {
        read_line(prompt, buf, sizeof buf);
        if (sscanf(buf, "%d-%d", &t.day, &t.month) == 2 && is_valid_date(t)) {
            *d = t;
            return;
        }
        printf("Invalid date! Use DD-MM (e.g. 25-12).\n");
    }
}

int date_to_days(Date d)
{
    int days = d.day;

    for (int m = 1; m < d.month; m++)
        days += days_in_month(m);
    return days;
}

int days_between(Date a, Date b)
{
    int diff = date_to_days(b) - date_to_days(a);

    if (diff < 0)
        diff += 365;
    return diff;
}

Date add_days(Date d, int n)
{
    while (n-- > 0) {
        d.day++;
        if (d.day > days_in_month(d.month)) {
            d.day = 1;
            d.month++;
            if (d.month > 12)
                d.month = 1;
        }
    }
    return d;
}
