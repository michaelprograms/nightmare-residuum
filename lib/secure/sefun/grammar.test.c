#include <time.h>

inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/grammar");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_remove_article () {
    string *values = ({}), *results = ({});

    expect_function("remove_article", testOb);

    values += ({ testOb->remove_article("thing") });
    results += ({ "thing" });
    values += ({ testOb->remove_article("the thing") });
    results += ({ "thing" });
    values += ({ testOb->remove_article("a thing") });
    results += ({ "thing" });
    values += ({ testOb->remove_article("an item") });
    results += ({ "item" });

    expect_arrays_equal(values, results, "remove_article handled articles");
}

void test_conjunction () {
    string *values = ({}), *results = ({});

    expect_function("conjunction", testOb);

    values += ({ testOb->conjunction(({ "1" })) });
    results += ({ "1" });
    values += ({ testOb->conjunction(({ "1", "2" })) });
    results += ({ "1 and 2" });
    values += ({ testOb->conjunction(({ "1", "2", "3" })) });
    results += ({ "1, 2, and 3" });
    values += ({ testOb->conjunction(({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" })) });
    results += ({ "1, 2, 3, 4, 5, 6, 7, 8, 9, and 10" });

    expect_arrays_equal(values, results, "conjunction handled list");

    expect_catches (({
        (: testOb->conjunction(0) :),
        (: testOb->conjunction(0.0) :),
        (: testOb->conjunction(({})) :),
        (: testOb->conjunction(({ 1, 2, 3 })) :),
        (: testOb->conjunction(({ "", "", "" })) :),
        (: testOb->conjunction(([])) :),
        (: testOb->conjunction((: 1 :)) :),
    }), "*Bad argument 1 to grammar->conjunction\n", "conjunction handled invalid argument 1");
}

void test_cardinal () {
    string *values = ({}), *results = ({});

    expect_function("cardinal", testOb);

    values += ({ testOb->cardinal(0) });
    results += ({ "zero" });
    values += ({ testOb->cardinal(1) });
    results += ({ "one" });
    values += ({ testOb->cardinal(-1) });
    results += ({ "negative one" });
    values += ({ testOb->cardinal(10) });
    results += ({ "ten" });
    values += ({ testOb->cardinal(-10) });
    results += ({ "negative ten" });
    values += ({ testOb->cardinal(100) });
    results += ({ "one hundred" });
    values += ({ testOb->cardinal(1000) });
    results += ({ "one thousand" });
    values += ({ testOb->cardinal(10000) });
    results += ({ "ten thousand" });
    values += ({ testOb->cardinal(12345) });
    results += ({ "twelve thousand, three hundred and forty-five" });
    values += ({ testOb->cardinal(-12345) });
    results += ({ "negative twelve thousand, three hundred and forty-five" });
    values += ({ testOb->cardinal(100000) });
    results += ({ "many" });
    values += ({ testOb->cardinal(-100000) });
    results += ({ "negative many" });

    expect_arrays_equal(values, results, "cardinal handled numbers");
}

void test_pluralize () {
    string *values = ({}), *results = ({});
    object ob;

    expect_function("pluralize", testOb);

    // test parameter as string
    values = ({});
    results = ({});
    values += ({ testOb->pluralize("elf") });
    results += ({ "elves" });
    values += ({ testOb->pluralize("staff") });
    results += ({ "staves" });
    values += ({ testOb->pluralize("a red house") });
    results += ({ "red houses" });
    values += ({ testOb->pluralize("a sack of rice") });
    results += ({ "sacks of rice" });
    values += ({ testOb->pluralize("plumbus") });
    results += ({ "plumbi" });
    values += ({ testOb->pluralize("gooblebox") });
    results += ({ "goobleboxes" });
    values += ({ testOb->pluralize("flooblecrank") });
    results += ({ "flooblecranks" });
    values += ({ testOb->pluralize("bloobleyank") });
    results += ({ "bloobleyanks" });
    expect_arrays_equal(values, results, "pluralize handled strings");

    // test parameter as object
    values = ({});
    results = ({});
    ob = new(STD_OBJECT);
    ob->set_name("tester");
    values += ({ testOb->pluralize(ob) });
    results += ({ "testers" });
    expect_arrays_equal(values, results, "pluralize handled objects");
    destruct(ob);
}

void test_consolidate () {
    string *values = ({}), *results = ({});

    expect_function("consolidate", testOb);

    values += ({ testOb->consolidate(5, "elf") });
    results += ({ "five elves" });

    values += ({ testOb->consolidate(0, "giraffe") });
    results += ({ "zero giraffes" });


    values += ({ testOb->consolidate(25, "cat") });
    results += ({ "twenty-five cats" });

    values += ({ testOb->consolidate(54321, "dog") });
    results += ({ "fifty-four thousand, three hundred and twenty-one dogs" });

    values += ({ testOb->consolidate(-54321, "care") });
    results += ({ "negative fifty-four thousand, three hundred and twenty-one cares" });

    values += ({ testOb->consolidate(2, "Paul") });
    results += ({ "two Pauls" });

    expect_arrays_equal(values, results, "consolidate handled words");
}

void test_possessive_noun () {
    string *values, *results;
    object ob;

    expect_function("possessive_noun", testOb);

    // test parameter as a string
    values = ({});
    results = ({});
    values += ({ testOb->possessive_noun() });
    results += ({ "Its" });
    values += ({ testOb->possessive_noun("Name") });
    results += ({ "Name's" });
    values += ({ testOb->possessive_noun("Hermes") });
    results += ({ "Hermes'" });
    values += ({ testOb->possessive_noun("Shax") });
    results += ({ "Shax'" });
    values += ({ testOb->possessive_noun("Chaz") });
    results += ({ "Chaz'" });
    expect_arrays_equal(values, results, "possessive_noun handled names");

    // test parameter as an object
    values = ({});
    results = ({});
    ob = new(STD_OBJECT);
    values += ({ testOb->possessive_noun() });
    results += ({ "Its" });
    ob->set_name("Name");
    values += ({ testOb->possessive_noun(ob) });
    results += ({ "Name's" });
    ob->set_name("Hermes");
    values += ({ testOb->possessive_noun(ob) });
    results += ({ "Hermes'" });
    ob->set_name("Shax");
    values += ({ testOb->possessive_noun(ob) });
    results += ({ "Shax'" });
    ob->set_name("Chaz");
    values += ({ testOb->possessive_noun(ob) });
    results += ({ "Chaz'" });
    expect_arrays_equal(values, results, "possessive_noun handled objects");
    destruct(ob);
}

void test_subjective () {
    string *values, *results;
    object ob;

    expect_function("subjective", testOb);

    // test parameter as a string
    values = ({});
    results = ({});
    values += ({ testOb->subjective() });
    results += ({ "it" });
    values += ({ testOb->subjective("male") });
    results += ({ "he" });
    values += ({ testOb->subjective("female") });
    results += ({ "she" });
    values += ({ testOb->subjective("neither") });
    results += ({ "they" });
    values += ({ testOb->subjective("nonexistant") });
    results += ({ "it" });
    expect_arrays_equal(values, results, "subjective handled names");

    // test parameter as an object
    values = ({});
    results = ({});
    ob = new(STD_LIVING);
    values += ({ testOb->subjective() });
    results += ({ "it" });
    ob->set_gender("male");
    values += ({ testOb->subjective(ob) });
    results += ({ "he" });
    ob->set_gender("female");
    values += ({ testOb->subjective(ob) });
    results += ({ "she" });
    ob->set_gender("neither");
    values += ({ testOb->subjective(ob) });
    results += ({ "they" });
    ob->set_gender("nonexistant");
    values += ({ testOb->subjective(ob) });
    results += ({ "it" });
    expect_arrays_equal(values, results, "subjective handled objects");
    destruct(ob);
}

void test_objective () {
    string *values, *results;
    object ob;

    expect_function("objective", testOb);

    // test parameter as a string
    values = ({});
    results = ({});
    values += ({ testOb->objective() });
    results += ({ "it" });
    values += ({ testOb->objective("male") });
    results += ({ "him" });
    values += ({ testOb->objective("female") });
    results += ({ "her" });
    values += ({ testOb->objective("neither") });
    results += ({ "them" });
    values += ({ testOb->objective("nonexistant") });
    results += ({ "it" });
    expect_arrays_equal(values, results, "objective handled names");

    // test parameter as an object
    values = ({});
    results = ({});
    ob = new(STD_LIVING);
    values += ({ testOb->objective() });
    results += ({ "it" });
    ob->set_gender("male");
    values += ({ testOb->objective(ob) });
    results += ({ "him" });
    ob->set_gender("female");
    values += ({ testOb->objective(ob) });
    results += ({ "her" });
    ob->set_gender("neither");
    values += ({ testOb->objective(ob) });
    results += ({ "them" });
    ob->set_gender("nonexistant");
    values += ({ testOb->objective(ob) });
    results += ({ "it" });
    expect_arrays_equal(values, results, "objective handled objects");
    destruct(ob);
}

void test_possessive () {
    string *values, *results;
    object ob;

    expect_function("possessive", testOb);

    // test parameter as a string
    values = ({});
    results = ({});
    values += ({ testOb->possessive() });
    results += ({ "its" });
    values += ({ testOb->possessive("male") });
    results += ({ "his" });
    values += ({ testOb->possessive("female") });
    results += ({ "hers" });
    values += ({ testOb->possessive("neither") });
    results += ({ "theirs" });
    values += ({ testOb->possessive("nonexistant") });
    results += ({ "its" });
    expect_arrays_equal(values, results, "possessive handled names");

    // test parameter as an object
    values = ({});
    results = ({});
    ob = new(STD_LIVING);
    values += ({ testOb->possessive() });
    results += ({ "its" });
    ob->set_gender("male");
    values += ({ testOb->possessive(ob) });
    results += ({ "his" });
    ob->set_gender("female");
    values += ({ testOb->possessive(ob) });
    results += ({ "hers" });
    ob->set_gender("neither");
    values += ({ testOb->possessive(ob) });
    results += ({ "theirs" });
    ob->set_gender("nonexistant");
    values += ({ testOb->possessive(ob) });
    results += ({ "its" });
    expect_arrays_equal(values, results, "possessive handled objects");
    destruct(ob);
}

void test_reflexive () {
    string *values, *results;
    object ob;

    expect_function("reflexive", testOb);

    // test parameter as a string
    values = ({});
    results = ({});
    values += ({ testOb->reflexive() });
    results += ({ "itself" });
    values += ({ testOb->reflexive("male") });
    results += ({ "himself" });
    values += ({ testOb->reflexive("female") });
    results += ({ "herself" });
    values += ({ testOb->reflexive("neither") });
    results += ({ "themself" });
    values += ({ testOb->reflexive("nonexistant") });
    results += ({ "itself" });
    expect_arrays_equal(values, results, "reflexive handled names");

    // test parameter as an object
    values = ({});
    results = ({});
    ob = new(STD_LIVING);
    values += ({ testOb->reflexive() });
    results += ({ "itself" });
    ob->set_gender("male");
    values += ({ testOb->reflexive(ob) });
    results += ({ "himself" });
    ob->set_gender("female");
    values += ({ testOb->reflexive(ob) });
    results += ({ "herself" });
    ob->set_gender("neither");
    values += ({ testOb->reflexive(ob) });
    results += ({ "themself" });
    ob->set_gender("nonexistant");
    values += ({ testOb->reflexive(ob) });
    results += ({ "itself" });
    expect_arrays_equal(values, results, "reflexive handled objects");
    destruct(ob);
}