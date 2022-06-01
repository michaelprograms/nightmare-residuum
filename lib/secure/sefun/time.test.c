#include <time.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/time.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_time_ago () {
    int now = time();

    expect_function("time_ago", testOb);

    expect("time_ago handles time inputs", (: ({
        assert(testOb->time_ago(now - 1), "==", "very recently"),
        assert(testOb->time_ago(now - 2), "==", "very recently"),
        assert(testOb->time_ago(now - 30), "==", "very recently"),
        assert(testOb->time_ago(now - 59), "==", "very recently"),

        assert(testOb->time_ago(now - MINUTE_IN_SECS), "==", "1 minute ago"),
        assert(testOb->time_ago(now - (2 * MINUTE_IN_SECS)), "==", "2 minutes ago"),
        assert(testOb->time_ago(now - (30 * MINUTE_IN_SECS)), "==", "30 minutes ago"),
        assert(testOb->time_ago(now - (59 * MINUTE_IN_SECS)), "==", "59 minutes ago"),

        assert(testOb->time_ago(now - HOUR_IN_SECS), "==", "1 hour ago"),
        assert(testOb->time_ago(now - (2 * HOUR_IN_SECS)), "==", "2 hours ago"),
        assert(testOb->time_ago(now - (12 * HOUR_IN_SECS)), "==", "12 hours ago"),
        assert(testOb->time_ago(now - (23 * HOUR_IN_SECS)), "==", "23 hours ago"),

        assert(testOb->time_ago(now - DAY_IN_SECS), "==", "1 day ago"),
        assert(testOb->time_ago(now - (2 * DAY_IN_SECS)), "==", "2 days ago"),
        assert(testOb->time_ago(now - (6 * DAY_IN_SECS)), "==", "6 days ago"),

        assert(testOb->time_ago(now - WEEK_IN_SECS), "==", "1 week ago"),
        assert(testOb->time_ago(now - (2 * WEEK_IN_SECS)), "==", "2 weeks ago"),
        assert(testOb->time_ago(now - (3 * WEEK_IN_SECS)), "==", "3 weeks ago"),

        assert(testOb->time_ago(now - MONTH_IN_SECS), "==", "1 month ago"),
        assert(testOb->time_ago(now - (2 * MONTH_IN_SECS)), "==", "2 months ago"),
        assert(testOb->time_ago(now - (6 * MONTH_IN_SECS)), "==", "6 months ago"),
        assert(testOb->time_ago(now - (11 * MONTH_IN_SECS)), "==", "11 months ago"),

        assert(testOb->time_ago(now - YEAR_IN_SECS), "==", "1 year ago"),
        assert(testOb->time_ago(now - (2 * YEAR_IN_SECS)), "==", "2 years ago"),
        assert(testOb->time_ago(now - (5 * YEAR_IN_SECS)), "==", "5 years ago"),
        assert(testOb->time_ago(now - (100 * YEAR_IN_SECS)), "==", "100 years ago"),

        assert(testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 4), "==", "1 year 7 months 2 weeks 4 days ago"),
        assert(testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 3), "==", "1 year 7 months 2 weeks ago"),
        assert(testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 2), "==", "1 year 7 months ago"),
        assert(testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 1), "==", "1 year ago"),

        // default granularity:
        assert(testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29), "==", "1 year 7 months ago"),
        // disable granularity:
        assert(testOb->time_ago(now - (1 * YEAR_IN_SECS) - (7 * MONTH_IN_SECS) - (2 * WEEK_IN_SECS) - (4 * DAY_IN_SECS) - (15 * HOUR_IN_SECS) - (42 * MINUTE_IN_SECS) - 29, 0), "==", "1 year 7 months 2 weeks 4 days ago"),
    }) :));
}

void test_time_from_seconds () {
    expect_function("time_from_seconds", testOb);

    expect("time_from_seconds handled times", (: ({
        assert(testOb->time_from_seconds(0), "==", "0s"),
        assert(testOb->time_from_seconds(1), "==", "1s"),
        assert(testOb->time_from_seconds(MINUTE_IN_SECS - 1), "==", "59s"),
        assert(testOb->time_from_seconds(MINUTE_IN_SECS), "==", "1m"),
        assert(testOb->time_from_seconds(MINUTE_IN_SECS + 1), "==", "1m 1s"),
        assert(testOb->time_from_seconds(MINUTE_IN_SECS * 2 + 2), "==", "2m 2s"),
        assert(testOb->time_from_seconds(HOUR_IN_SECS + MINUTE_IN_SECS), "==", "1h 1m"),
        assert(testOb->time_from_seconds(DAY_IN_SECS + HOUR_IN_SECS + MINUTE_IN_SECS), "==", "1d 1h 1m"),
        assert(testOb->time_from_seconds(WEEK_IN_SECS + DAY_IN_SECS + HOUR_IN_SECS + MINUTE_IN_SECS), "==", "1w 1d 1h 1m"),
        assert(testOb->time_from_seconds(MONTH_IN_SECS), "==", "4w 2d 10h"),
        assert(testOb->time_from_seconds(MONTH_IN_SECS + WEEK_IN_SECS + 3 * DAY_IN_SECS + 4 * HOUR_IN_SECS + 17 * MINUTE_IN_SECS + 23), "==", "5w 5d 14h 17m 23s"),
    }) :));
}