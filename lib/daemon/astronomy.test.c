inherit M_TEST;

private int __Time;
private mapping __Almanac = ([ ]);

private int MINUTE = 20;
private int DAY = 24000;


private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/astronomy.c");
    __Time = 720561600; // The Beginning: Sat Oct 31 15:00:00 1992
    __Almanac["SECONDS_PER_MINUTE"] = 20;
    __Almanac["MINUTES_PER_HOUR"] = 60;
    __Almanac["HOURS_PER_DAY"] = 20;
    __Almanac["DAYS_PER_WEEK"] = 5;
    __Almanac["WEEKS_PER_MONTH"] = 4;
    __Almanac["MONTHS_PER_YEAR"] = 10;

    __Almanac["SECOND"]  = 1;                                                       // defaults:
    __Almanac["MINUTE"]  = __Almanac["SECONDS_PER_MINUTE"] * __Almanac["SECOND"];   // 20
    __Almanac["HOUR"]    = __Almanac["MINUTES_PER_HOUR"] * __Almanac["MINUTE"];     // 1200
    __Almanac["DAY"]     = __Almanac["HOURS_PER_DAY"] * __Almanac["HOUR"];          // 24000
    __Almanac["WEEK"]    = __Almanac["DAYS_PER_WEEK"] * __Almanac["DAY"];           // 120000
    __Almanac["MONTH"]   = __Almanac["WEEKS_PER_MONTH"] * __Almanac["WEEK"];        // 480000
    __Almanac["YEAR"]    = __Almanac["MONTHS_PER_YEAR"] * __Almanac["MONTH"];       // 4800000

    __Almanac["TOTAL_DAYS"] = __Almanac["YEAR"] / __Almanac["DAY"];
    __Almanac["SHORTEST_DAY"] = 0;
    __Almanac["LONGEST_DAY"] = to_int(ceil(__Almanac["TOTAL_DAYS"] / 2)); // 100

    __Almanac["MOONS"] = ([
        "laros": ([ "orbit": __Almanac["TOTAL_DAYS"]/10.0, "color": "red" ]),
        "spyefel": ([ "orbit": __Almanac["TOTAL_DAYS"]/2.0, "color": "green" ]),
        "slayar": ([ "orbit": __Almanac["TOTAL_DAYS"], "color": "blue" ]),
    ]);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    D_ASTRONOMY->create(); // have to force restart heartbeat
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
        assert(testOb->query_minutes(__Time, __Almanac), "==", 0),
        assert(testOb->query_day(__Time, __Almanac), "==", 0),
        assert(testOb->query_hour(__Time, __Almanac), "==", 0),
        assert(testOb->query_week(__Time, __Almanac), "==", 0),
        assert(testOb->query_month(__Time, __Almanac), "==", 0),
        assert(testOb->query_year(__Time, __Almanac), "==", 0),
    }) :));

    expect("handles minute rolling to hour", (: ({
        __Time += MINUTE,
        assert(testOb->query_minute(__Time, __Almanac), "==", 1),
        __Time += MINUTE * 59,
        assert(testOb->query_minute(__Time, __Almanac), "==", 0),
        assert(testOb->query_hour(__Time, __Almanac), "==", 1),
    }) :));

    expect("handles hour rolling to day", (: ({
        __Time += (MINUTE * 60) * 19,
        assert(testOb->query_hour(__Time, __Almanac), "==", 0),
        assert(testOb->query_day(__Time, __Almanac), "==", 1),
    }) :));

    expect("handles day rolling to week", (: ({
        __Time += ((MINUTE * 60) * 20) * 4,
        assert(testOb->query_day(__Time, __Almanac), "==", 0),
        assert(testOb->query_week(__Time, __Almanac), "==", 1),
    }) :));

    expect("handles week rolling to month", (: ({
        __Time += (((MINUTE * 60) * 20) * 5) * 3,
        assert(testOb->query_week(__Time, __Almanac), "==", 0),
        assert(testOb->query_month(__Time, __Almanac), "==", 1),
    }) :));

    expect("handles month rolling to year", (: ({
        __Time += ((((MINUTE * 60) * 20) * 5) * 4) * 9,
        assert(testOb->query_month(__Time, __Almanac), "==", 0),
        assert(testOb->query_year(__Time, __Almanac), "==", 1),
    }) :));
}

void test_day_of_year () {
    expect_function("query_day_of_year", testOb);

    expect("day of year stays within bounds", (: ({
        assert(testOb->query_day_of_year(__Time+(DAY*0), __Almanac), "==", 0),
        assert(testOb->query_day_of_year(__Time+(DAY*1), __Almanac), "==", 1),
        assert(testOb->query_day_of_year(__Time+(DAY*25), __Almanac), "==", 25),
        assert(testOb->query_day_of_year(__Time+(DAY*50), __Almanac), "==", 50),
        assert(testOb->query_day_of_year(__Time+(DAY*75), __Almanac), "==", 75),
        assert(testOb->query_day_of_year(__Time+(DAY*100), __Almanac), "==", 100),
        assert(testOb->query_day_of_year(__Time+(DAY*199), __Almanac), "==", 199),
        assert(testOb->query_day_of_year(__Time+(DAY*200), __Almanac), "==", 0),
    }) :));
}

void test_almanac () {
    expect_function("query_calculate_almanac", testOb);

    expect("handles dawn and dusk throughout year", (: ({
        assert(testOb->query_calculate_almanac(__Time+(DAY*0), __Almanac), "==", ([ "dawn": ({ 4, 0 }), "day": ({ 5, 0 }), "dusk": ({ 16, 0 }), "night": ({ 17, 0 }), "equinox": "fall" ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*25), __Almanac), "==", ([ "dawn": ({ 4, 30 }), "day": ({ 5, 30 }), "dusk": ({ 15, 30 }), "night": ({ 16, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*50), __Almanac), "==", ([ "dawn": ({ 5, 0 }), "day": ({ 6, 0 }), "dusk": ({ 15, 0 }), "night": ({ 16, 0 }), "solstice": "summer" ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*75), __Almanac), "==", ([ "dawn": ({ 5, 30 }), "day": ({ 6, 30 }), "dusk": ({ 14, 30 }), "night": ({ 15, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*100), __Almanac), "==", ([ "dawn": ({ 6, 0 }), "day": ({ 7, 0 }), "dusk": ({ 14, 0 }), "night": ({ 15, 0 }), "equinox": "spring" ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*125), __Almanac), "==", ([ "dawn": ({ 5, 30 }), "day": ({ 6, 30 }), "dusk": ({ 14, 30 }), "night": ({ 15, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*150), __Almanac), "==", ([ "dawn": ({ 5, 0 }), "day": ({ 6, 0 }), "dusk": ({ 15, 0 }), "night": ({ 16, 0 }), "solstice": "winter" ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*175), __Almanac), "==", ([ "dawn": ({ 4, 30 }), "day": ({ 5, 30 }), "dusk": ({ 15, 30 }), "night": ({ 16, 30 }) ])),
        assert(testOb->query_calculate_almanac(__Time+(DAY*200), __Almanac), "==", ([ "dawn": ({ 4, 0 }), "day": ({ 5, 0 }), "dusk": ({ 16, 0 }), "night": ({ 17, 0 }), "equinox": "fall" ])),
    }) :));
}
