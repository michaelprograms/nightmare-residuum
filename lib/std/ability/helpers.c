/**
 * Determine how many sides the die will have for calculations
 *
 * @param {STD_LIVING} source the source of the ability
 * @returns integer number of sides on the die
 */
int query_die_sides (object source) {
    if (source->query_class() == "psionist") {
        return 8;
    } else if (source->query_class() == "mystic") {
        return 7;
    } else {
        return 6;
    }
}