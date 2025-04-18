nosave private mapping __Tests = ([
    /* Data format:
    "/std/module/test.test.c": ([
        "code": "/std/module/test.c",
    ])
    */
]);
nosave private string *__TestFiles = ({ }), *__TestsMissing = ({ });
nosave private mapping __Results = ([ ]);

nosave private int currentTest = 0, totalFiles = 0;
nosave private int shutdownAfterTests = 0, coverageAfterTests = 0;
private nosave int testStartTime;

/** @type {STD_USER} __User */
nosave private object __User;

mapping __Lines = ([ ]), __TotalLines = ([ ]);
string *__RawLines = ({ });

/* ----- function prototypes ----- */

void process_file (string file, mapping options);
private string format_total_line (string name, int current, int total);
void process ();
varargs void update_test_data (string path, string ignore);
varargs void run (mapping options);
void display_results (mapping results);
int *query_hit_lines ();
int *query_unhit_lines ();
string *query_hit_functions ();
string *query_unhit_functions ();

/* -----  ----- */

void reset_data () {
    currentTest = 0;
    coverageAfterTests = 0;
    __TotalLines = ([ ]);

    __Results = ([ ]);
    __Results["numTests"] = 0;
    __Results["passingExpects"] = 0;
    __Results["failingExpects"] = 0;
    __Results["testedFns"] = 0;
    __Results["untestedFns"] = 0;
    __Results["failLog"] = ({ });
    __Results["passingAsserts"] = 0;
    __Results["failingAsserts"] = 0;

    totalFiles = 0;
    __Tests = ([ ]);
    __TestFiles = ({ });
    __TestsMissing = ({ });
}

/* -----  ----- */

string convert_to_ranges (int *lines) {
    int start, end, i, l = sizeof(lines);
    string *ranges = ({ });

    lines = sort_array(lines, 1);

    for (i = 0; i < l; i ++) {
        start = lines[i];
        end = start;
        while (i + 1 < l && lines[i + 1] - lines[i] == 1) {
            end = lines[i + 1];
            i ++;
        }
        ranges += ({ start == end ? start + "" : start + "-" + end });
    }
    return implode(ranges, ",");
}

void done_test (mapping results) {
    if (results) {
        int hitLines, unhitLines, totalLines;
        int hitFns, unhitFns, totalFns;
        int *uncoveredLines;

        __Results["numTests"] += results["numTests"];
        __Results["passingExpects"] += results["passingExpects"];
        __Results["failingExpects"] += results["failingExpects"];
        __Results["testedFns"] += results["testedFns"];
        __Results["untestedFns"] += results["untestedFns"];
        __Results["passingAsserts"] += results["passingAsserts"];
        __Results["failingAsserts"] += results["failingAsserts"];
        if (sizeof(results["failLog"]) > 0) {
            __Results["failLog"] += ({ results["failLog"] });
        }
        hitLines = sizeof(query_hit_lines());
        unhitLines = sizeof(uncoveredLines = query_unhit_lines());
        totalLines = hitLines + unhitLines;
        hitFns = sizeof(query_hit_functions());
        unhitFns = sizeof(query_unhit_functions());
        totalFns = hitFns + unhitFns;
        if (hitLines + unhitLines > 0) {
            __Results["hitLines"] += hitLines;
            __Results["unhitLines"] += unhitLines;
        }
        if (coverageAfterTests) {
            string file = __TestFiles[currentTest][0..<8] + ".c";
            __TotalLines[file] = ([
                "fnsPct": totalFns > 0 ? hitFns * 100.0 / totalFns : 0.0,
                "fnsNum": hitFns + "/" + totalFns,
                "linesPct": totalLines > 0 ? hitLines * 100.0 / totalLines : 0.0,
                "linesNum": hitLines + "/" + totalLines,
                "uncovered": convert_to_ranges(uncoveredLines),
            ]);
        }
    }
    currentTest ++;
    process();
}

mapping __Options = ([
    "brief": 0,
    "coverage": 0,
    "reset": 0,
]);
int query_option (string key) {
    return __Options[key];
}

void process_file (string file, mapping options) {
    object t;
    string tmp;
    function fnDone;

    __Lines = ([ ]);
    __RawLines = ({ });

    if (mapp(options)) {
        __Options += options;
    }

    if (__Options["reset"]) {
        reset_data();
        update_test_data(file);
        testStartTime = time_ns();
        fnDone = (: display_results :);
    } else {
        fnDone = (: done_test :);
    }
    if (t = find_object(file)) {
        destruct(t);
    }
    tmp = catch (load_object(file));
    if (tmp) {
        message("system", "Error in test: " + tmp + "\n", this_user());
        return;
    }
    if (!inherits(M_TEST, load_object(file))) {
        evaluate(fnDone);
        return;
    }
    // call out clears the call stack, call other will chain the tests
    call_out_walltime(function(string test, function done) {
        object testFile = new(test);
        mixed err = catch (testFile->execute_test(done));
        if (err) {
            write("\n    " + test + " encountered an errored:\n" + err + "\n");
            evaluate(done);
        }
        if (objectp(testFile)) { // async tests require time
            call_out_walltime(function (object ob) {
                if (ob) {
                    destruct(ob);
                }
            }, 5, testFile);
        }
    }, 0, file, fnDone);
}

private string format_total_line (string name, int current, int total) {
    string tmp = sprintf("%-18s", name + ":");
    if (current == total) {
        tmp += "\e[32m\u2713 \e[0m";
    } else {
        tmp += "\e[31m\u2715 \e[0m";
    }
    tmp += sprintf("%4d", current) + " / " + sprintf("%-4d", total);
    tmp += "  " + sprintf("%6.2f", current * 100.0 / total) + "%";
    return tmp;
}
private string query_coverage_gradient (float f) {
    switch (to_int(f / 20)) {
        case 5:
            return "\e[38;5;10m";
        case 4:
            return "\e[38;5;2m";
        case 3:
            return "\e[38;5;11m";
        case 2:
            return "\e[38;5;3m";
        case 1:
            return "\e[38;5;1m";
        default:
            return "\e[38;5;9m";
    }
}
string format_coverage_line (int n, mapping coverage) {
    float fnsPct = coverage["fnsPct"];
    float linesPct = coverage["linesPct"];
    float combinedPct = (fnsPct + linesPct) / 2.0;
    string uncovered = coverage["uncovered"];
    string fnsC = "", linesC = "", combinedC = "", resetC = "\e[0m";
    string result;

    fnsC = query_coverage_gradient(fnsPct);
    linesC = query_coverage_gradient(linesPct);
    combinedC = query_coverage_gradient(combinedPct);

    result = sprintf("%-*' 's", 23 - n, "");
    result += sprintf("%s%3d%%", combinedC, linesPct) + " ";
    result += sprintf("%s%7|s", fnsC, coverage["fnsNum"]) + resetC + " ";
    result += sprintf("%s%7|s", linesC, coverage["linesNum"]) + resetC;
    if (sizeof(uncovered)) {
        if (sizeof(uncovered) > 31) {
            uncovered = uncovered[0..31] + "…";
        }
        result += " " + uncovered;
    }
    return result;
}

void display_results (mapping results) {
    int totalExpects = results["passingExpects"] + results["failingExpects"];
    int totalAsserts = results["passingAsserts"] + results["failingAsserts"];
    int totalFns = results["testedFns"] + results["untestedFns"];
    int time;

    if (testStartTime > 0) {
        time = time_ns() - testStartTime;
    }

    write("\n");
    if (totalFiles > 1) {
        write(format_total_line("Files", currentTest, totalFiles) + "\n");
    }
    if (totalFns > 0) {
        write(format_total_line("Functions", results["testedFns"], totalFns) + "\n");
    }
    if (results["hitLines"] + results["unhitLines"] > 0) {
        write(format_total_line("Lines", results["hitLines"], results["hitLines"] + results["unhitLines"]) + "\n");
    }
    if (totalExpects > 0) {
        write(format_total_line("Expects Passed", results["passingExpects"], totalExpects) + "\n");
    }
    if (totalAsserts > 0) {
        write(format_total_line("Asserts Passed", results["passingAsserts"], totalAsserts) + "\n");
    } else {
        write("No tests were found.\n");
    }

    if (testStartTime > 0) {
        write("\n" + sprintf("%-20s", results["numTests"]+" tests:") + "\e[33m" + sprintf("%7.2f", time/1000000.0) + " ms" + "\e[0m" + "\n\n");

        if (!sizeof(__TestFiles)) {
            if (sizeof(query_unhit_functions()) > 0) {
                write(sprintf("%-20s", "Untested Functions:") + implode(query_unhit_functions(), ", ") + "\n");
            }
            if (sizeof(query_unhit_lines()) > 0) {
                write(sprintf("%-20s", "Uncovered:") + convert_to_ranges(query_unhit_lines()) + "\n\n");
            }
        }
    }

    if (sizeof(results["failLog"]) > 0) {
        write("Failing expects:\n" + (arrayp(results["failLog"]) ? implode(results["failLog"], "\n") : results["failLog"]) + "\n\n");
        results["failLog"] = ({ });
    }

    if (__Options["coverage"] && sizeof(__TotalLines)) {
        mapping tree = ([ "/": ([ ]) ]), treeRef;
        string *keys = sort_array(keys(__TotalLines), 1);
        int n;

        for (int k = 0; k < sizeof(keys); k ++) {
            string *split = explode(keys[k], "/");
            treeRef = tree["/"];

            for (int i = 0; i < sizeof(split) - 1; i ++) {
                string s = split[i] + "/";
                if (!treeRef[s]) {
                    treeRef[s] = ([ ]);
                }
                treeRef = treeRef[s];
            }
            n = sizeof(split[<1]) + (sizeof(treeRef) > 9 ? 1 : 0) + sizeof(split) * 2;
            treeRef[split[<1]] = format_coverage_line(n, __TotalLines[keys[k]]);
        }
        tree["/                     Total   Fns    Lines  Uncovered Lines"] = tree["/"];
        map_delete(tree, "/");
        write(implode(tree(tree), "\n")+"\n\n");
    }

    if (sizeof(__TestsMissing)) {
        foreach (string file in __TestsMissing) {
            write("Missing " + file + "\n");
        }
        write("\n");
    }

    if (__User) {
        __User->input_prompt();
    }
}

void process () {
    if (currentTest < sizeof(__TestFiles)) {
        process_file(__TestFiles[currentTest], 0);
    } else {
        display_results(__Results);

        if (shutdownAfterTests) {
            shutdown(__Results["failingExpects"] > 0 ? -1 : 0);
        }
    }
}

varargs void update_test_data (string path, string ignoreRegex) {
    mixed *dir = get_dir(path, -1); // Assumes path has trailing / for dirs
    string *codeFiles = ({ }), tmp;

    foreach (mixed *file in dir) {
        if (ignoreRegex && regexp(path + file[0], ignoreRegex)) {
            continue;
        } else if (file[1] == -2) {
            update_test_data(path + file[0] + "/", ignoreRegex);
        } else if (regexp(file[0], "\\.test\\.c$") && !regexp(file[0], "\\.(coverage|mock)\\.")) {
            __Tests[path+file[0]] = ([ ]);
        } else if (regexp(file[0], "\\.c$") && !regexp(file[0], "\\.(coverage|mock)\\.")) {
            totalFiles ++;
            codeFiles += ({ file });
        }
    }
    foreach (mixed *file in codeFiles) {
        tmp = path + replace_string(file[0], ".c", ".test.c", 1);
        if (__Tests[tmp]) {
            __Tests[tmp]["code"] = path + file[0];
        } else {
            __TestsMissing += ({ path + file[0][0..<2] + "test.c" });
        }
    }
}

void run (mapping options) {
    remove_call_out();
    reset_data();

    if (mapp(options)) {
        __Options += options;
    }
    __Options["reset"] = 0;
    shutdownAfterTests = __Options["shutdown"];
    coverageAfterTests = 1;

    __User = 0;
    if (!shutdownAfterTests) {
        __User = this_user();
    }

    testStartTime = time_ns();
    write("Scanning for test files...\n");
    update_test_data("/secure/", "/secure/cmd");
    update_test_data("/daemon/");
    update_test_data("/std/");

    __TestFiles = keys(__Tests);
    __TestFiles = sort_array(__TestFiles, function(string a, string b) {
        if (regexp(a, "^\\/std\\/module\\/test")) {
            return -1;
        } else if (regexp(b, "^\\/std\\/module\\/test")) {
            return 1;
        } else if (regexp(a, "^\\/secure\\/daemon\\/master")) {
            return -1;
        } else if (regexp(b, "^\\/secure\\/daemon\\/master")) {
            return 1;
        } else if (regexp(a, "^\\/secure\\/sefun\\/sefun")) {
            return -1;
        } else if (regexp(b, "^\\/secure\\/sefun\\/sefun")) {
            return 1;
        } else if (regexp(a, "^\\/secure\\/sefun\\/")) {
            return -1;
        } else if (regexp(b, "^\\/secure\\/sefun\\/")) {
            return 1;
        } else {
            return strcmp(a, b);
        }
    });

    call_out_walltime((: process :), 0);
}

/* ----- code coverage ----- */

mapping query_lines () {
    return __Lines;
}
void line_hit (int n) {
    if (__Lines[n]) {
        __Lines[n][0] ++;
    }
}
string *query_hit_functions () {
    return values(map(__Lines, (: sizeof($2) > 1 && $2[0] > 0 ? $2[1] : 0 :))) - ({ 0 });
}
string *query_unhit_functions () {
    return values(map(__Lines, (: sizeof($2) > 1 && $2[0] == 0 ? $2[1] : 0 :))) - ({ 0 });
}
int *query_hit_lines () {
    return sort_array(keys(filter(__Lines, (: $2[0] > 0 :))), 1);
}
int *query_unhit_lines () {
    return sort_array(keys(filter(__Lines, (: $2[0] == 0 :))), 1);
}

string create_coverage (string path) {
    string *newLines = ({ }), *reMatches;
    string line, cPath;
    object cOb;
    int i, l;

    __Lines = ([ ]);

    cPath = path[0..<2] + "coverage.c";
    cp(path, cPath);
    cOb = load_object(cPath);
    destruct(cOb);

    __RawLines = explode(read_file(path), "\n");

    l = sizeof(__RawLines);
    for (i = 0; i < l; i ++) {
        if (sizeof(reMatches = pcre_extract(__RawLines[i], "^(?:public |private |protected |nosave |nomask |varargs )*(?:float|int|object|mapping|mixed|string|void) *\\**([a-zA-Z0-9_]{1,}) \\(.*\\) {")) > 0) {
            // Start of Function
            line = __RawLines[i] + " D_TEST->line_hit(" + (i+1) + "); {";
            __Lines[i+1] = ({ 0, reMatches[0] });
        } else if (pcre_match(__RawLines[i], "^}$")) {
            // End of Function
            line = "} " + __RawLines[i];
        } else if (pcre_match(__RawLines[i], "^\\s+\\} else \\{")) {
            int n = strsrch(__RawLines[i], "{");
            // Else Construct
            line = __RawLines[i][0..n] + " D_TEST->line_hit(" + (i+1) + ");" + __RawLines[i][n+1..];
            __Lines[i+1] = ({ 0 });
        } else if (sizeof(reMatches = pcre_extract(__RawLines[i], "^(\\s+\\} else if \\()(.*)(\\) \\{)")) > 0) {
            // Else If Construct
            line = reMatches[0] + "D_TEST->line_hit(" + (i+1) + ") || (" + reMatches[1] + ")" + reMatches[2];
            __Lines[i+1] = ({ 0 });
        } else if (pcre_match(__RawLines[i], "^\\s+(?:case \"?.*\"?:|default:)\\s*return")) {
            // Case / Default return
            line = replace_string(__RawLines[i], " return", " D_TEST->line_hit(" + (i+1) + "); return");
            __Lines[i+1] = ({ 0 });
        } else if (pcre_match(__RawLines[i], "(?:default|case \\\"?.+?\\\"?):$")) {
            // Case / Default
            line = __RawLines[i] + " D_TEST->line_hit(" + (i+1) + ");";
            __Lines[i+1] = ({ 0 });
        } else if (pcre_match(__RawLines[i], "^\\s+(break|for \\(|foreach \\(|if \\(|return|switch \\(|while \\(|continue;|efun::)")) {
            // Construct
            line = "D_TEST->line_hit(" + (i+1) + "); " + __RawLines[i];
            __Lines[i+1] = ({ 0 });
        } else if (pcre_match(__RawLines[i], "^\\s+([^ \\((\\/*|\\/\\/)]+ (=|\\+\\+|--|\\+=|-=|\\*=|\\\\=).*;?)")) {
            // Variable Operator
            line = "D_TEST->line_hit(" + (i+1) + "); " + __RawLines[i];
            __Lines[i+1] = ({ 0 });
        } else if (
            pcre_match(__RawLines[i], "^\\s+([\\:]*[a-zA-Z_]\\w*\\s*\\((.*)\\)\\s*;)") ||
            pcre_match(__RawLines[i], "^\\s+([a-zA-Z_]\\w*\\s*->\\s*([a-zA-Z_]\\w*)\\s*\\((.*)\\)\\s*;)")
        ) {
            // Function Call or Call Other pointer
            line = "D_TEST->line_hit(" + (i+1) + "); " + __RawLines[i];
            __Lines[i+1] = ({ 0 });
        } else {
            line = __RawLines[i];
        }
        newLines += ({ line });
    }
    write_file(cPath, implode(newLines, "\n"), 1);
    return cPath;
}

void analyze_coverage () {
    int l = sizeof(__RawLines);
    string *lines = ({ });

    for (int i = 0; i < l; i ++) {
        string lineNum = ""+(i+1);
        string bColor;
        string lineInfo;

        if (!sizeof(__Lines[i+1]) > 1) { // Function
            if (!__Lines[i+1][0]) {
                bColor = "B_FFC";
            } else {
                bColor = "B_G14";
            }
            lineInfo = __Lines[i+1][0] + "x";
        } else if (!undefinedp(__Lines[i+1])) { // Line
            if (!__Lines[i+1][0]) {
                bColor = "B_FFC";
            } else {
                bColor = "B_G14";
            }
            lineInfo = __Lines[i+1][0] + "x";
        } else {
            bColor = "B_G10";
            lineInfo = "";
        }
        lines += ({ "%^" + bColor + "%^BLACK%^" + lineNum + sprintf("%*s", 8-sizeof(lineNum), lineInfo) + "%^RESET%^  " + __RawLines[i] });
    }
    this_user()->handle_pager(lines);
}
