mapping query_astronomy () {
    mapping a = ([]);

    a["SECONDS_PER_MINUTE"] = 20;
    a["MINUTES_PER_HOUR"] = 60;
    a["HOURS_PER_DAY"] = 20;
    a["DAYS_PER_WEEK"] = 5;
    a["WEEKS_PER_MONTH"] = 4;
    a["MONTHS_PER_YEAR"] = 10;

    a["SECOND"]  = 1;                                       // defaults:
    a["MINUTE"]  = a["SECONDS_PER_MINUTE"] * a["SECOND"];   // 20
    a["HOUR"]    = a["MINUTES_PER_HOUR"] * a["MINUTE"];     // 1200
    a["DAY"]     = a["HOURS_PER_DAY"] * a["HOUR"];          // 24000
    a["WEEK"]    = a["DAYS_PER_WEEK"] * a["DAY"];           // 120000
    a["MONTH"]   = a["WEEKS_PER_MONTH"] * a["WEEK"];        // 480000
    a["YEAR"]    = a["MONTHS_PER_YEAR"] * a["MONTH"];       // 4800000

    a["SHORTEST_DAY"] = 0;
    a["LONGEST_DAY"] = to_int(ceil(a["YEAR"] / a["DAY"] / 2)); // 100
    a["TOTAL_DAYS"] = a["YEAR"] / a["DAY"];

    return a;
}