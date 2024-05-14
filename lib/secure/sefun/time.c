#include <time.h>

varargs string time_ago (int timestamp, int granularity) {
    int Years, Months, Weeks, Days, Hours, Minutes;
    int diff = time() - timestamp;
    int matches;
    string msg = "";

    if (undefinedp(granularity)) {
        granularity = 2;
    }

    Years = diff / YEAR_IN_SECS;
    diff = diff - (Years * YEAR_IN_SECS);
    Months = diff / MONTH_IN_SECS;
    diff = diff - (Months * MONTH_IN_SECS);
    Weeks = diff / WEEK_IN_SECS;
    diff = diff - (Weeks * WEEK_IN_SECS);
    Days = diff / DAY_IN_SECS;
    diff = diff - (Days * DAY_IN_SECS);

    if (Years > 0) {
        msg += Years + " year" + (Years > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) {
        return msg + " ago";
    }
    if (Months > 0) {
        msg += (sizeof(msg)>0?" ":"") + Months + " month" + (Months > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) {
        return msg + " ago";
    }
    if (Weeks > 0) {
        msg += (sizeof(msg)>0?" ":"") + Weeks + " week" + (Weeks > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) {
        return msg + " ago";
    }
    if (Days > 0) {
        msg +=  (sizeof(msg)>0?" ":"") + Days + " day" + (Days > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) {
        return msg + " ago";
    }

    if (sizeof(msg) > 0) {
        return msg + " ago";
    }

    Hours = diff / HOUR_IN_SECS;
    diff = diff - (Hours * HOUR_IN_SECS);
    if (Hours > 0) {
        return Hours + " hour" + (Hours > 1 ? "s" : "") + " ago";
    }

    Minutes = diff / MINUTE_IN_SECS;
    diff = diff - (Minutes * MINUTE_IN_SECS);
    if (Minutes > 0) {
        return Minutes + " minute" + (Minutes > 1 ? "s" : "") + " ago";
    }

    return "very recently";
}

string time_from_seconds (int seconds) {
    int s = seconds, w, d, h, m;
    string *results = ({ });

    w = s / WEEK_IN_SECS;
    s = s - (w * WEEK_IN_SECS);
    d = s / DAY_IN_SECS;
    s = s - (d * DAY_IN_SECS);
    h = s / HOUR_IN_SECS;
    s = s - (h * HOUR_IN_SECS);
    m = s / MINUTE_IN_SECS;
    s = s - (m * MINUTE_IN_SECS);

    if (w) {
        results += ({ w + "w" });
    }
    if (d) {
        results += ({ d + "d" });
    }
    if (h) {
        results += ({ h + "h" });
    }
    if (m) {
        results += ({ m + "m" });
    }
    if (s || (!w && !d && !h && !m && !s)) {
        results += ({ s + "s" });
    }

    return implode(results, " ");
}