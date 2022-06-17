inherit M_TEST;

#define MINUTE 20
#define DAY 24000

private int __Time;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/astronomy.c");
    __Time = 720561600; // The Beginning: Sat Oct 31 15:00:00 1992
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_now () {
    expect_function("query_now", testOb);

    expect("handles now", (: ({
        assert(testOb->query_now(__Time), "==", 0),
        assert(testOb->query_now(time()), ">", 1),
    }) :));
}

void test_time_intervals () {
    expect_function("query_second", testOb);
    expect_function("query_minute", testOb);
    expect_function("query_day", testOb);
    expect_function("query_hour", testOb);
    expect_function("query_week", testOb);
    expect_function("query_month", testOb);
    expect_function("query_year", testOb);

    expect("handles the beginning", (: ({
        assert(testOb->query_now(__Time), "==", 0),
        assert(testOb->query_minutes(__Time), "==", 0),
        assert(testOb->query_day(__Time), "==", 0),
        assert(testOb->query_hour(__Time), "==", 0),
        assert(testOb->query_week(__Time), "==", 0),
        assert(testOb->query_month(__Time), "==", 0),
        assert(testOb->query_year(__Time), "==", 0),
    }) :));

    expect("handles minute rolling to hour", (: ({
        __Time += MINUTE,
        assert(testOb->query_minute(__Time), "==", 1),
        __Time += MINUTE * 59,
        assert(testOb->query_minute(__Time), "==", 0),
        assert(testOb->query_hour(__Time), "==", 1),
    }) :));

    expect("handles hour rolling to day", (: ({
        __Time += (MINUTE * 60) * 19,
        assert(testOb->query_hour(__Time), "==", 0),
        assert(testOb->query_day(__Time), "==", 1),
    }) :));

    expect("handles day rolling to week", (: ({
        __Time += ((MINUTE * 60) * 20) * 4,
        assert(testOb->query_day(__Time), "==", 0),
        assert(testOb->query_week(__Time), "==", 1),
    }) :));

    expect("handles week rolling to month", (: ({
        __Time += (((MINUTE * 60) * 20) * 5) * 3,
        assert(testOb->query_week(__Time), "==", 0),
        assert(testOb->query_month(__Time), "==", 1),
    }) :));

    expect("handles month rolling to year", (: ({
        __Time += ((((MINUTE * 60) * 20) * 5) * 4) * 9,
        assert(testOb->query_month(__Time), "==", 0),
        assert(testOb->query_year(__Time), "==", 1),
    }) :));
}

void test_time_of_day () {
    expect_function("query_time_of_day", testOb);

    expect("handles time of day", (: ({
        assert(testOb->query_now(__Time), "==", 0),
        assert(testOb->query_time_of_day(__Time), "==", "0:00"),
        __Time += MINUTE,
        assert(testOb->query_time_of_day(__Time), "==", "0:01"),
        __Time += MINUTE,
        assert(testOb->query_time_of_day(__Time), "==", "0:02"),
        __Time += MINUTE,
        assert(testOb->query_time_of_day(__Time), "==", "0:03"),
        __Time += MINUTE * 7,
        assert(testOb->query_time_of_day(__Time), "==", "0:10"),
        __Time += MINUTE * 50,
        assert(testOb->query_time_of_day(__Time), "==", "1:00"),
        __Time += (MINUTE * 60),
        assert(testOb->query_time_of_day(__Time), "==", "2:00"),
        __Time += (MINUTE * 60) * 7,
        assert(testOb->query_time_of_day(__Time), "==", "9:00"),
        __Time += (MINUTE * 60),
        assert(testOb->query_time_of_day(__Time), "==", "10:00"),
        __Time += (MINUTE * 60) * 9,
        assert(testOb->query_time_of_day(__Time), "==", "19:00"),
        __Time += (MINUTE * 60),
        assert(testOb->query_time_of_day(__Time), "==", "0:00"),
    }) :));
}

void test_day_of_year () {
    expect_function("query_shortest_day", testOb);
    expect_function("query_longest_day", testOb);
    expect_function("query_day_of_year", testOb);

    expect("handles shortest and longest day", (: ({
        assert(testOb->query_shortest_day(), "==", 0),
        assert(testOb->query_longest_day(), "==", 100),
    }) :));

    expect("day of year stays within bounds", (: ({
        assert(testOb->query_day_of_year(__Time+(DAY*0)), "==", 0),
        assert(testOb->query_day_of_year(__Time+(DAY*1)), "==", 1),
        assert(testOb->query_day_of_year(__Time+(DAY*199)), "==", 199),
        assert(testOb->query_day_of_year(__Time+(DAY*200)), "==", 0),
    }) :));
}

void test_almanac () {
    expect_function("query_calculate_almanac", testOb);

    expect("handles dawn and dusk throughout year", (: ({
        assert(testOb->query_calculate_almanac(__Time+(DAY*0)), "==", ([ "dawn": ({ 4, 0 }), "day": ({ 5, 0 }), "dusk": ({ 16, 0 }), "night": ({ 17, 0 }), "equinox": "spring" ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*25)), "==", ([ "dawn": ({ 4, 30 }), "day": ({ 5, 30 }), "dusk": ({ 15, 30 }), "night": ({ 16, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*50)), "==", ([ "dawn": ({ 5, 0 }), "day": ({ 6, 0 }), "dusk": ({ 15, 0 }), "night": ({ 16, 0 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*75)), "==", ([ "dawn": ({ 5, 30 }), "day": ({ 6, 30 }), "dusk": ({ 14, 30 }), "night": ({ 15, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*100)), "==", ([ "dawn": ({ 6, 0 }), "day": ({ 7, 0 }), "dusk": ({ 14, 0 }), "night": ({ 15, 0 }), "equinox": "winter" ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*125)), "==", ([ "dawn": ({ 5, 30 }), "day": ({ 6, 30 }), "dusk": ({ 14, 30 }), "night": ({ 15, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*150)), "==", ([ "dawn": ({ 5, 0 }), "day": ({ 6, 0 }), "dusk": ({ 15, 0 }), "night": ({ 16, 0 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*175)), "==", ([ "dawn": ({ 4, 30 }), "day": ({ 5, 30 }), "dusk": ({ 15, 30 }), "night": ({ 16, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*200)), "==", ([ "dawn": ({ 4, 0 }), "day": ({ 5, 0 }), "dusk": ({ 16, 0 }), "night": ({ 17, 0 }), "equinox": "spring" ])),
    }) :));
}