inherit M_CLEAN;

/*
    Shortest Day = 1 (and 200)
    Longest Day = 100 (and 101)
    Max Days = 200

    1..100 = 100 days
    101..200 = 100 days
*/

#define THE_BEGINNING   720561600
#define NOW(t)          (t - THE_BEGINNING)

private int SECOND  =  1;
private int MINUTE  = 20 * SECOND;  // 20
private int HOUR    = 60 * MINUTE;  // 1200
private int DAY     = 20 * HOUR;    // 24000
private int WEEK    =  5 * DAY;     // 120000
private int MONTH   =  4 * WEEK;    // 480000
private int YEAR    = 10 * MONTH;   // 4800000

private int SHORTEST_DAY = 0;
private int LONGEST_DAY = to_int(ceil(YEAR / DAY / 2)); // 100
private int TOTAL_DAYS = YEAR / DAY;

/* -----  ----- */

int query_shortest_day () { return SHORTEST_DAY; }
int query_longest_day () { return LONGEST_DAY; }
int query_now (int t) { return NOW(t); }

int query_second (int t) { return NOW(t) % MINUTE; }
int query_minute (int t) { return NOW(t) % HOUR / MINUTE; }
int query_hour (int t) { return NOW(t) % DAY / HOUR; }
int query_day (int t) { return NOW(t) % WEEK / DAY; }
int query_week (int t) { return NOW(t) % MONTH / WEEK; }
int query_month (int t) { return NOW(t) % YEAR / MONTH; }
int query_year (int t) { return NOW(t) / YEAR; }

// @TODO set_ functions

/* -----  ----- */

nosave private mapping __Almanac;
nosave private string __DayPhase;
nosave private int __NextPhase;

mapping query_almanac () { return __Almanac; }

int query_next_phase () { return __NextPhase; }
void set_next_phase (int t) { __NextPhase = t; }

string query_day_phase () { return __DayPhase; }
string set_day_phase (string phase) {
    __DayPhase = phase;
    return __DayPhase;
}

int query_day_of_year (int t) {
    // [0..199] == (0..9 * 20 == 0..180) + (0..3 * 5  == 0..15) + (0..4)
    return (query_month(t) * 20) + query_week(t) * 5 + query_day(t);
}

/*
    dawn         4:00..6:00
    sunrise      5:00..7:00
    midday      10:00
    dusk        16:00..14:00
    night       17:00..15:00
    midnight    20:00

    0   04:00 - 16:00 = 12 hours day,  8 hours night
    25  04:30 - 15:30 = 11 hours day,  9 hours night
    50  05:00 - 15:00 = 10 hours day, 10 hours night
    75  05:30 - 14:30 =  9 hours day, 11 hours night
    100 06:00 - 14:00 =  8 hours day, 12 hours night
    125 05:30 - 14:30 =  9 hours day, 11 hours night
    150 05:00 - 15:00 = 10 hours day, 10 hours night
    175 04:30 - 15:30 = 11 hours day,  9 hours night
    200 04:00 - 16:00 = 12 hours day,  8 hours night

    spring equinox:     [0] && [200]
    summer solstice:    [50]
    fall equinox:       [150]
    winter solstice:    [100]
*/

private mixed *format_almanac (int d, string prepend) {
    int hour = to_int((prepend+d)[0..1]);
    int min = to_int((prepend+d)[2..3]) * 60 / 100;
    return ({ hour, min, }); // sprintf("%s:%'0'2s", ""+hour, ""+min) });
}
mapping query_calculate_almanac (int t) {
    mapping almanac = ([ ]);
    int days = query_day_of_year(t);

    if (days == query_longest_day()) almanac["equinox"] = "fall";
    else if (days == query_shortest_day()) almanac["equinox"] = "spring";
    else if (days == query_longest_day() / 2) almanac["solstice"] = "summer";
    else if (days == query_longest_day() / 2 * 3) almanac["solstice"] = "winter";

    days = (days >= LONGEST_DAY ? (-1 * days + TOTAL_DAYS) : days);

    almanac["dawn"] = format_almanac(400 + (2 * days), "0");
    almanac["day"] = format_almanac(500 + (2 * days), "0");
    almanac["dusk"] = format_almanac(2000 - (400 + (2 * days)), "");
    almanac["night"] = format_almanac(2000 - (300 + (2 * days)), "");

    return almanac;
}

/* -----  ----- */

varargs string query_time_of_day (int t) {
    string hour, minute;
    if (undefinedp(t)) t = time();
    hour = "" + query_hour(t);
    minute = sprintf("%2'0's", "" + query_minute(t))[0..1];
    return hour + ":" + minute;
}
varargs string query_datetime_of_year (int t) {
    string day, month, year;
    if (undefinedp(t)) t = time();
    day = "" + (query_week(t)*(WEEK/DAY) + query_day(t));
    month = "" + (query_month(t) + 1);
    year = "" + query_year(t);
    return day + " of Month" + month + " " + query_year(t);
}

// string query_full_time (int t) {
//     string year = "" + query_year(t);
//     string month = "" + (query_month(t)+1);
//     string week = "" + (query_week(t)+1);
//     string day = "" + (query_day(t)+1);
//     string hour = "" + query_hour(t);
//     string minute = "" + query_minute(t);
//     string second = "" + query_second(t);
//     if (strlen(minute) < 2) minute = "0" + minute;
//     if (strlen(second) < 2) second = "0" + second;
//     return hour + ":" + minute + ":" + second + " of day " + day + ", of week " + week + ", month " + month + " year " + year;
// }

void update_almanac (int t) {
    __Almanac = query_calculate_almanac(t);
}

void update_day_phase () {
    int t = time(), h = query_hour(t), m = query_minute(t);
    int dawnH, dawnM, dayH, dayM;
    int duskH, duskM, nightH, nightM;

    dawnH = __Almanac["dawn"][0];
    dawnM = __Almanac["dawn"][1];
    dayH = __Almanac["day"][0];
    dayM = __Almanac["day"][1];
    duskH = __Almanac["dusk"][0];
    duskM = __Almanac["dusk"][1];
    nightH = __Almanac["night"][0];
    nightM = __Almanac["night"][1];

    if ((h < dawnH) || (h == dawnH && m < dawnM) || (h >= nightH && m >= nightM)) {
        __DayPhase = "night";
    } else if ((h < dayH) || (h == dayH && m < dayM)) {
        __DayPhase = "dawn";
    } else if ((h < duskH) || (h == duskH && m < duskM)) {
        __DayPhase = "day";
    } else if ((h < nightH) || (h == nightH && m < nightM)) {
        __DayPhase = "dusk";
    }
}

void create () {
    if (clonep()) {
        update_almanac(time());
        update_day_phase();
        set_no_clean(1);
    }
}