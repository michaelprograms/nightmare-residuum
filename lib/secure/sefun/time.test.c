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

    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 4) });
    results += ({ "1 year 7 months 2 weeks 4 days ago" });
    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 3) });
    results += ({ "1 year 7 months 2 weeks ago" });
    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 2) });
    results += ({ "1 year 7 months ago" });
    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 1) });
    results += ({ "1 year ago" });
    // default granularity:
    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29) });
    results += ({ "1 year 7 months ago" });
    // disable granularity:
    values += ({ testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 0) });
    results += ({ "1 year 7 months 2 weeks 4 days ago" });

    expect_arrays_equal(values, results, "time_ago handled times");
}

void test_time_from_seconds () {
    string *values = ({}), *results = ({});

    expect_function("time_from_seconds", testOb);

    values += ({ testOb->time_from_seconds(0) });
    results += ({ "0s" });
    values += ({ testOb->time_from_seconds(1) });
    results += ({ "1s" });
    values += ({ testOb->time_from_seconds(MINUTE_IN_SECS - 1) });
    results += ({ "59s" });
    values += ({ testOb->time_from_seconds(MINUTE_IN_SECS) });
    results += ({ "1m" });
    values += ({ testOb->time_from_seconds(MINUTE_IN_SECS + 1) });
    results += ({ "1m 1s" });
    values += ({ testOb->time_from_seconds(MINUTE_IN_SECS * 2 + 2) });
    results += ({ "2m 2s" });
    values += ({ testOb->time_from_seconds(HOUR_IN_SECS + MINUTE_IN_SECS) });
    results += ({ "1h 1m" });
    values += ({ testOb->time_from_seconds(DAY_IN_SECS + HOUR_IN_SECS + MINUTE_IN_SECS) });
    results += ({ "1d 1h 1m" });
    values += ({ testOb->time_from_seconds(WEEK_IN_SECS + DAY_IN_SECS + HOUR_IN_SECS + MINUTE_IN_SECS) });
    results += ({ "1w 1d 1h 1m" });
    values += ({ testOb->time_from_seconds(MONTH_IN_SECS) });
    results += ({ "4w 2d 10h" });
    values += ({ testOb->time_from_seconds(MONTH_IN_SECS + WEEK_IN_SECS + 3 * DAY_IN_SECS + 4 * HOUR_IN_SECS + 17 * MINUTE_IN_SECS + 23) });
    results += ({ "5w 5d 14h 17m 23s" });

    expect_arrays_equal(values, results, "time_from_seconds handled times");
}