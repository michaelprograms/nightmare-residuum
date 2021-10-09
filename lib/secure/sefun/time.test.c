#include <time.h>

inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/time");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_time_ago () {
    string *values = ({}), *results = ({});
    int now = time();

    expect_function("time_ago", testOb);

    values += ({ testOb->time_ago(now - 1) });
    results += ({ "very recently" });
    values += ({ testOb->time_ago(now - 2) });
    results += ({ "very recently" });
    values += ({ testOb->time_ago(now - 30) });
    results += ({ "very recently" });
    values += ({ testOb->time_ago(now - 59) });
    results += ({ "very recently" });

    values += ({ testOb->time_ago(now - MINUTE_IN_SECS) });
    results += ({ "1 minute ago" });
    values += ({ testOb->time_ago(now - (2 * MINUTE_IN_SECS)) });
    results += ({ "2 minutes ago" });
    values += ({ testOb->time_ago(now - (30 * MINUTE_IN_SECS)) });
    results += ({ "30 minutes ago" });
    values += ({ testOb->time_ago(now - (59 * MINUTE_IN_SECS)) });
    results += ({ "59 minutes ago" });

    values += ({ testOb->time_ago(now - HOUR_IN_SECS) });
    results += ({ "1 hour ago" });
    values += ({ testOb->time_ago(now - (2 * HOUR_IN_SECS)) });
    results += ({ "2 hours ago" });
    values += ({ testOb->time_ago(now - (12 * HOUR_IN_SECS)) });
    results += ({ "12 hours ago" });
    values += ({ testOb->time_ago(now - (23 * HOUR_IN_SECS)) });
    results += ({ "23 hours ago" });

    values += ({ testOb->time_ago(now - DAY_IN_SECS) });
    results += ({ "1 day ago" });
    values += ({ testOb->time_ago(now - (2 * DAY_IN_SECS)) });
    results += ({ "2 days ago" });
    values += ({ testOb->time_ago(now - (6 * DAY_IN_SECS)) });
    results += ({ "6 days ago" });

    values += ({ testOb->time_ago(now - WEEK_IN_SECS) });
    results += ({ "1 week ago" });
    values += ({ testOb->time_ago(now - (2 * WEEK_IN_SECS)) });
    results += ({ "2 weeks ago" });
    values += ({ testOb->time_ago(now - (3 * WEEK_IN_SECS)) });
    results += ({ "3 weeks ago" });

    values += ({ testOb->time_ago(now - MONTH_IN_SECS) });
    results += ({ "1 month ago" });
    values += ({ testOb->time_ago(now - (2 * MONTH_IN_SECS)) });
    results += ({ "2 months ago" });
    values += ({ testOb->time_ago(now - (6 * MONTH_IN_SECS)) });
    results += ({ "6 months ago" });
    values += ({ testOb->time_ago(now - (11 * MONTH_IN_SECS)) });
    results += ({ "11 months ago" });

    values += ({ testOb->time_ago(now - YEAR_IN_SECS) });
    results += ({ "1 year ago" });
    values += ({ testOb->time_ago(now - (2 * YEAR_IN_SECS)) });
    results += ({ "2 years ago" });
    values += ({ testOb->time_ago(now - (5 * YEAR_IN_SECS)) });
    results += ({ "5 years ago" });
    values += ({ testOb->time_ago(now - (100 * YEAR_IN_SECS)) });
    results += ({ "100 years ago" });

    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29) });
    results += ({ "1 year 7 months 2 weeks 4 days ago" });

    expect_arrays_equal(values, results, "time_ago handled times");
}