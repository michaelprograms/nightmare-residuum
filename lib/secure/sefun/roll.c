mixed *roll_die (int times, int sides) {
    int *rolls;

    if (undefinedp(times)) error("Bad argument 1 to roll_die");
    if (undefinedp(sides) || sides < 2) error("Bad argument 2 to roll_die");

    rolls = allocate(times, (: 1 + random($(sides)) :));

    return ({
        SEFUN->reduce(rolls, (: $1 + $2 :), 0),
        rolls
    });
}