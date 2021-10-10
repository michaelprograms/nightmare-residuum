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
    if (granularity > 0 && matches >= granularity) return msg + " ago";
    if (Months > 0) {
        msg += (strlen(msg)>0?" ":"") + Months + " month" + (Months > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) return msg + " ago";
    if (Weeks > 0) {
        msg += (strlen(msg)>0?" ":"") + Weeks + " week" + (Weeks > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) return msg + " ago";
    if (Days > 0) {
        msg +=  (strlen(msg)>0?" ":"") + Days + " day" + (Days > 1 ? "s" : "");
        matches ++;
    }
    if (granularity > 0 && matches >= granularity) return msg + " ago";

    if (strlen(msg) > 0) return msg + " ago";

    Hours = diff / HOUR_IN_SECS;
    diff = diff - (Hours * HOUR_IN_SECS);
    if (Hours > 0) return Hours + " hour" + (Hours > 1 ? "s" : "") + " ago";

    Minutes = diff / MINUTE_IN_SECS;
    diff = diff - (Minutes * MINUTE_IN_SECS);
    if (Minutes > 0) return Minutes + " minute" + (Minutes > 1 ? "s" : "") + " ago";

    return "very recently";
}