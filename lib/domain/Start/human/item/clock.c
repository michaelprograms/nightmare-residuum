inherit STD_ITEM;

void create () {
    ::create();
    set_name("clock tower");
    set_id(({ "clock tower", "clock", "tower", }));
    set_short("a clock tower");
    set_long("A clock tower.");
    set_no_get(1);
}

nosave private mapping a; // astronomy
nosave private mixed *dots;

void set_dot (int x, int y) {
    int xoff, yoff, dot;

    xoff = x % 2;
    yoff = y % 4;
    dot = dots[x/2][y/4];
    if (xoff == 0 && yoff == 0) {
        dot |= 0x1;
    } else if (xoff == 1 && yoff == 0) {
        dot |= 0x8;
    } else if (xoff == 0 && yoff == 1) {
        dot |= 0x2;
    } else if (xoff == 1 && yoff == 1) {
        dot |= 0x10;
    } else if (xoff == 0 && yoff == 2) {
        dot |= 0x4;
    } else if (xoff == 1 && yoff == 2) {
        dot |= 0x20;
    } else if (xoff == 0 && yoff == 3) {
        dot |= 0x40;
    } else if (xoff == 1 && yoff == 3) {
        dot |= 0x80;
    }
    dots[x/2][y/4] = dot;
}

void plot_circle (int xm, int ym, int r)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
   do {
      set_dot (xm-x, ym+y); /*   I. Quadrant */
      set_dot (xm-y, ym-x); /*  II. Quadrant */
      set_dot (xm+x, ym-y); /* III. Quadrant */
      set_dot (xm+y, ym+x); /*  IV. Quadrant */
      r = err;
      if (r >  x) err += ++x*2+1; /* e_xy+e_x > 0 */
      if (r <= y) err += ++y*2+1; /* e_xy+e_y < 0 */
   } while (x < 0);
}

// Bresenham's line algorithm
private void plot_line (int x0, int y0, int x1, int y1) {
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; // error value e_xy

    for (;;) {  // loop
        set_dot(x0, y0);

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { // e_xy+e_x > 0
            err += dy; x0 += sx;
        }
        if (e2 <= dx) { // e_xy+e_y < 0
            err += dx; y0 += sy;
        }
    }
}

int fnSin (float i, int l) {
    return to_int(floor(sin(i * 2 * 3.1415962) * l + 80));
}
int fnCos (float i, int l) {
    return to_int(floor(cos(i * 2 * 3.1415962) * l + 80));
}

string query_long () {
    string long = ::query_long(), d = " ";
    int width, w, t;
    float h, m, s;
    string localtime;

    if (!this_user() || !clonep(this_object()) || !environment(this_object())) return long;

    width = this_user()->query_account()->query_setting("width");
    w = width * 2;
    a = D_ASTRONOMY->query_astronomy_from_room(environment(this_object()));
    t = time();
    dots = allocate(w/2, (: allocate($(w/4), (: 0 :)) :));
    long += "\n";

    localtime = D_ASTRONOMY->query_localtime(a) + " of " + a["HOURS_PER_DAY"] + ":00";

    localtime = D_ASTRONOMY->query_hour(t, a) + ":" + D_ASTRONOMY->format_minute(D_ASTRONOMY->query_minute(t, a)) + ":" + D_ASTRONOMY->format_minute(D_ASTRONOMY->query_second(t, a));

    long += localtime;

    // hour
    h = 1.0 * D_ASTRONOMY->query_hour(t, a) / a["HOURS_PER_DAY"];
    plot_line(80, 80, fnSin(h, 30), 160-fnCos(h, 30));

    // minute
    m = 1.0 * D_ASTRONOMY->query_minute(t, a) / a["MINUTES_PER_HOUR"];
    plot_line(80, 80, fnSin(m, 45), 160-fnCos(m, 45));

    // second
    s = 1.0 * D_ASTRONOMY->query_second(t, a) / a["SECONDS_PER_MINUTE"];
    plot_line(80, 80, fnSin(s, 60), 160-fnCos(s, 60));

    // border
    plot_circle(80, 80, 70);

    for (int y = 0; y < w/4; y ++) {
        for (int x = 0; x < w/2; x ++) {
            d[0] = 10240 + dots[x][y];
            long += d;
        }
        long += "\n";
    }
    message("system", long, this_user());
    return "";
    // return long;
}

/*
// Clock
function draw() {
    c.clear();
    var t = new Date();
    line(80, 80, sin(t.getHours()/24, 30), 160-cos(t.getHours()/24, 30), c.set.bind(c));
    line(80, 80, sin(t.getMinutes()/60, 50), 160-cos(t.getMinutes()/60, 50), c.set.bind(c));
    line(80, 80, sin(t.getSeconds()/60+(+t%1000)/60000, 75), 160-cos(t.getSeconds()/60+(+t%1000)/60000, 75), c.set.bind(c));
    process.stdout.write(c.frame());
}
*/