inherit M_TEST;

void test_remove_article () {
    expect("articles are trimmed", (: ({
        assert_equal(testOb->remove_article(""), ""),

        assert_equal(testOb->remove_article("thing"), "thing"),

        assert_equal(testOb->remove_article("the thing"), "thing"),
        assert_equal(testOb->remove_article("a thing"), "thing"),
        assert_equal(testOb->remove_article("an item"), "item"),
    }) :));
}
void test_add_article () {
    expect("articles are prepended", (: ({
        assert_equal(testOb->add_article(""), ""),

        assert_equal(testOb->add_article("item"), "an item"),
        assert_equal(testOb->add_article("item", 1), "the item"),

        assert_equal(testOb->add_article("thing"), "a thing"),
        assert_equal(testOb->add_article("thing", 1), "the thing"),
    }) :));
}

void test_conjunction () {
    expect("conjunction handles list", (: ({
        assert_equal(testOb->conjunction(({ "1" })), "1"),
        assert_equal(testOb->conjunction(({ "1", "2" })), "1 and 2"),
        assert_equal(testOb->conjunction(({ "1", "2", "3" })), "1, 2, and 3"),
        assert_equal(testOb->conjunction(({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" })), "1, 2, 3, 4, 5, 6, 7, 8, 9, and 10"),
    }) :));
}

void test_cardinal () {
    expect("cardinal handles numbers", (: ({
        assert_equal(testOb->cardinal(0), "zero"),
        assert_equal(testOb->cardinal(1), "one"),
        assert_equal(testOb->cardinal(-1), "negative one"),
        assert_equal(testOb->cardinal(10), "ten"),
        assert_equal(testOb->cardinal(-10), "negative ten"),
        assert_equal(testOb->cardinal(100), "one hundred"),
        assert_equal(testOb->cardinal(1000), "one thousand"),
        assert_equal(testOb->cardinal(10000), "ten thousand"),
        assert_equal(testOb->cardinal(12345), "twelve thousand, three hundred and forty-five"),
        assert_equal(testOb->cardinal(-12345), "negative twelve thousand, three hundred and forty-five"),
        assert_equal(testOb->cardinal(100000), "many"),
        assert_equal(testOb->cardinal(-100000), "negative many"),
    }) :));
}

void test_ordinal () {
    expect("ordinal handles words for 0..9 range", (: ({
        assert_equal(testOb->ordinal(0), "zeroth"),
        assert_equal(testOb->ordinal(1), "first"),
        assert_equal(testOb->ordinal(2), "second"),
        assert_equal(testOb->ordinal(3), "third"),
        assert_equal(testOb->ordinal(4), "fourth"),
        assert_equal(testOb->ordinal(5), "fifth"),
        assert_equal(testOb->ordinal(6), "sixth"),
        assert_equal(testOb->ordinal(7), "seventh"),
        assert_equal(testOb->ordinal(8), "eighth"),
        assert_equal(testOb->ordinal(9), "ninth"),
    }) :));

    expect("ordinal handles numbers outside 11..14 range", (: ({
        assert_equal(testOb->ordinal(11), "11th"),
        assert_equal(testOb->ordinal(12), "12th"),
        assert_equal(testOb->ordinal(13), "13th"),
        assert_equal(testOb->ordinal(14), "14th"),
    }) :));

    expect("ordinal handles numbers outside 0..14 range", (: ({
        assert_equal(testOb->ordinal(15), "15th"),
        assert_equal(testOb->ordinal(22), "22nd"),
        assert_equal(testOb->ordinal(23), "23rd"),
        assert_equal(testOb->ordinal(24), "24th"),
    }) :));
}

void test_pluralize () {
    object ob;

    expect("pluralize handles strings", (: ({
        assert_equal(testOb->pluralize("elf"), "elves"),
        assert_equal(testOb->pluralize("dwarf"), "dwarves"),

        assert_equal(testOb->pluralize("staff"), "staves"),
        assert_equal(testOb->pluralize("scoff"), "scoffs"),
        assert_equal(testOb->pluralize("bluff"), "bluffs"),
        assert_equal(testOb->pluralize("quaff"), "quaffs"),
        assert_equal(testOb->pluralize("chaff"), "chaffs"),
        assert_equal(testOb->pluralize("graff"), "graffs"),

        assert_equal(testOb->pluralize("gun"), "guns"),
        assert_equal(testOb->pluralize("guns"), "guns"),

        assert_equal(testOb->pluralize("a red house"), "red houses"),
        assert_equal(testOb->pluralize("a sack of rice"), "sacks of rice"),
        assert_equal(testOb->pluralize("plumbus"), "plumbi"),
        assert_equal(testOb->pluralize("gooblebox"), "goobleboxes"),
        assert_equal(testOb->pluralize("flooblecrank"), "flooblecranks"),
        assert_equal(testOb->pluralize("bloobleyank"), "bloobleyanks"),
    }) :));

    ob = new(STD_OBJECT);
    expect("pluralize handles objects", (: ({
        assert_equal($(ob)->set_name("tester"), 0),
        assert_equal(testOb->pluralize($(ob)), "testers"),
        assert_equal($(ob)->set_name("staff"), 0),
        assert_equal(testOb->pluralize($(ob)), "staves"),
    }) :));
    destruct(ob);

    expect("pluralize handles some abnormal overridden strings", (: ({
        // handle entire string is abnormal override
        assert_equal(testOb->pluralize("die"), "dies"),
        assert_equal(testOb->pluralize("were"), "was"),
        assert_equal(testOb->pluralize("boots"), "boots"),
        assert_equal(testOb->pluralize("robes"), "robes"),
        assert_equal(testOb->pluralize("gloves"), "gloves"),
        assert_equal(testOb->pluralize("shoes"), "shoes"),

        // handle only last word is override string
        assert_equal(testOb->pluralize("blue robes"), "blue robes"),
        assert_equal(testOb->pluralize("canvas shoes"), "canvas shoes"),
    }) :));
}

void test_consolidate () {
    expect("consolidate handles basic words", (: ({
        assert_equal(testOb->consolidate(5, "elf"), "five elves"),
        assert_equal(testOb->consolidate(0, "giraffe"), "zero giraffes"),
        assert_equal(testOb->consolidate(25, "cat"), "twenty-five cats"),
        assert_equal(testOb->consolidate(54321, "dog"), "fifty-four thousand, three hundred and twenty-one dogs"),
        assert_equal(testOb->consolidate(-54321, "care"), "negative fifty-four thousand, three hundred and twenty-one cares"),
        assert_equal(testOb->consolidate(2, "Paul"), "two Pauls"),
    }) :));
    expect("consolidate handles parenthesis tags after words", (: ({
        assert_equal(testOb->consolidate(2, "a bronze dagger (wielded)"), "two bronze daggers (wielded)"),
        assert_equal(testOb->consolidate(2, "a bronze ring (worn)"), "two bronze rings (worn)"),
        assert_equal(testOb->consolidate(3, "a monster (injured)"), "three monsters (injured)"),
    }) :));
}

void test_possessive_noun () {
    object ob;

    expect("possessive_noun handles names", (: ({
        assert_equal(testOb->possessive_noun(0), "Its"),
        assert_equal(testOb->possessive_noun("Name"), "Name's"),
        assert_equal(testOb->possessive_noun("Hermes"), "Hermes'"),
        assert_equal(testOb->possessive_noun("Shax"), "Shax's"),
        assert_equal(testOb->possessive_noun("Chaz"), "Chaz'"),
    }) :));

    ob = new(STD_OBJECT);
    expect("possessive_noun handles objects", (: ({
        assert_equal($(ob)->set_name("Name"), 0),
        assert_equal(testOb->possessive_noun($(ob)), "Name's"),
        assert_equal($(ob)->set_name("Hermes"), 0),
        assert_equal(testOb->possessive_noun($(ob)), "Hermes'"),
        assert_equal($(ob)->set_name("Shax"), 0),
        assert_equal(testOb->possessive_noun($(ob)), "Shax's"),
        assert_equal($(ob)->set_name("Chaz"), 0),
        assert_equal(testOb->possessive_noun($(ob)), "Chaz'"),
    }) :));
    destruct(ob);
}

void test_subjective () {
    object ob;

    expect("subjective handles names", (: ({
        assert_equal(testOb->subjective(0), "it"),
        assert_equal(testOb->subjective("male"), "he"),
        assert_equal(testOb->subjective("female"), "she"),
        assert_equal(testOb->subjective("neither"), "they"),
        assert_equal(testOb->subjective("nonexistant"), "it"),
    }) :));

    ob = new(STD_LIVING);
    expect("subjective handles objects", (: ({
        assert_equal($(ob)->set_gender("male"), 0),
        assert_equal(testOb->subjective($(ob)), "he"),
        assert_equal($(ob)->set_gender("female"), 0),
        assert_equal(testOb->subjective($(ob)), "she"),
        assert_equal($(ob)->set_gender("neither"), 0),
        assert_equal(testOb->subjective($(ob)), "they"),
        assert_equal($(ob)->set_gender("nonexistant"), 0),
        assert_equal(testOb->subjective($(ob)), "it"),
    }) :));
    destruct(ob);
}

void test_objective () {
    object ob;

    expect("objective handles names", (: ({
        assert_equal(testOb->objective(0), "it"),
        assert_equal(testOb->objective("male"), "him"),
        assert_equal(testOb->objective("female"), "her"),
        assert_equal(testOb->objective("neither"), "them"),
        assert_equal(testOb->objective("nonexistant"), "it"),
    }) :));

    ob = new(STD_LIVING);
    expect("objective handles objects", (: ({
        assert_equal($(ob)->set_gender("male"), 0),
        assert_equal(testOb->objective($(ob)), "him"),
        assert_equal($(ob)->set_gender("female"), 0),
        assert_equal(testOb->objective($(ob)), "her"),
        assert_equal($(ob)->set_gender("neither"), 0),
        assert_equal(testOb->objective($(ob)), "them"),
        assert_equal($(ob)->set_gender("nonexistant"), 0),
        assert_equal(testOb->objective($(ob)), "it"),
    }) :));
    destruct(ob);
}

void test_possessive () {
    object ob;

    expect("possessive handles names", (: ({
        assert_equal(testOb->possessive(0), "its"),
        assert_equal(testOb->possessive("male"), "his"),
        assert_equal(testOb->possessive("female"), "her"),
        assert_equal(testOb->possessive("neither"), "their"),
        assert_equal(testOb->possessive("nonexistant"), "its"),
    }) :));

    ob = new(STD_LIVING);
    expect("possessive handles objects", (: ({
        assert_equal($(ob)->set_gender("male"), 0),
        assert_equal(testOb->possessive($(ob)), "his"),
        assert_equal($(ob)->set_gender("female"), 0),
        assert_equal(testOb->possessive($(ob)), "her"),
        assert_equal($(ob)->set_gender("neither"), 0),
        assert_equal(testOb->possessive($(ob)), "their"),
        assert_equal($(ob)->set_gender("nonexistant"), 0),
        assert_equal(testOb->possessive($(ob)), "its"),
    }) :));
    destruct(ob);
}

void test_reflexive () {
    object ob;

    expect("reflexive handles names", (: ({
        assert_equal(testOb->reflexive(0), "itself"),
        assert_equal(testOb->reflexive("male"), "himself"),
        assert_equal(testOb->reflexive("female"), "herself"),
        assert_equal(testOb->reflexive("neither"), "themself"),
        assert_equal(testOb->reflexive("nonexistant"), "itself"),
    }) :));

    ob = new(STD_LIVING);
    expect("reflexive handles objects", (: ({
        assert_equal($(ob)->set_gender("male"), 0),
        assert_equal(testOb->reflexive($(ob)), "himself"),
        assert_equal($(ob)->set_gender("female"), 0),
        assert_equal(testOb->reflexive($(ob)), "herself"),
        assert_equal($(ob)->set_gender("neither"), 0),
        assert_equal(testOb->reflexive($(ob)), "themself"),
        assert_equal($(ob)->set_gender("nonexistant"), 0),
        assert_equal(testOb->reflexive($(ob)), "itself"),
    }) :));
    destruct(ob);
}