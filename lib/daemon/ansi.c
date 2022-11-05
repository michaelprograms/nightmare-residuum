#define ANSI(p) "\e["+(p)+"m"
#define ESC(p) "\e"+(p)

protected mapping ansiTerm, unknownTerm;

void create () {
    ansiTerm = ([
        "INITTERM":         ESC("[H") + ESC("[2J"),
        "CLEARLINE":        ESC("[L") + ESC("[G"),
        "ENDTERM":          "",
        "RESET":            ANSI("0;37;40"),
        "BOLD":             ANSI(1),
        "ITALIC":           ANSI(3),
        "UNDERLINE":        ANSI(4),
        "FLASH":            ANSI(5),
        "INVERSE":          ANSI(7),
        "STRIKE":           ANSI(9),
        "BOLD_OFF":         ANSI(22),
        "ITALIC_OFF":       ANSI(23),
        "UNDERLINE_OFF":    ANSI(24),
        "FLASH_OFF":        ANSI(25),
        "INVERSE_OFF":      ANSI(27),
        "STRIKE_OFF":       ANSI(29),
        "BLACK":            ANSI(30),
        "RED":              ANSI(31),
        "GREEN":            ANSI(32),
        "ORANGE":           ANSI(33),
        "YELLOW":           ANSI("1;33"),
        "BLUE":             ANSI(34),
        "MAGENTA":          ANSI(35),
        "CYAN":             ANSI(36),
        "WHITE":            ANSI(37),
        "B_BLACK":          ANSI(40),
        "B_RED":            ANSI(41),
        "B_GREEN":          ANSI(42),
        "B_ORANGE":         ANSI(43),
        "B_YELLOW":         ANSI("1;43"),
        "B_BLUE":           ANSI(44),
        "B_MAGENTA":        ANSI(45),
        "B_CYAN":           ANSI(46),
        "B_WHITE":          ANSI(47),
    ]);
    unknownTerm = map(ansiTerm, (: "" :));
}

mapping query_ansi_term () { return ansiTerm; }
mapping query_unknown_term () { return unknownTerm; }
string parse (string str) {
    return terminal_colour(str, ansiTerm);
}