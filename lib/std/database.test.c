inherit M_TEST;
/**
 * @var {"/std/database"} testOb
 */

nosave private object db;
void test_connect () {
    expect("connect returns a handle", (: ({
        // connect
        db = testOb->connect(([ "db": "/save/test/database.db", ])),
        assert_equal(objectp(db), 1),
        assert_equal(testOb->query_handle() > 0, 1),
    }) :));
    expect("handle is queryable", (: ({
        // query non-existant
        assert_equal(testOb->query("SELECT * FROM `Test`"), UNDEFINED),

        // create
        testOb->query("CREATE TABLE `Test` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, `thing` TEXT NOT NULL )"),

        // table is empty
        assert_equal(testOb->query("SELECT * FROM `Test`"), UNDEFINED),

        // insert into table
        testOb->query("INSERT INTO `Test` (thing) VALUES ('some thing')"),
        testOb->query("INSERT INTO `Test` (thing) VALUES ('another thing')"),

        // table has records
        assert_equal(testOb->query("SELECT * FROM `Test`"), ({ ({ 1, "some thing" }), ({ 2, "another thing" }) })),

        // delete table records
        assert_equal(testOb->query("DELETE FROM `Test`"), UNDEFINED),

        // table is empty
        assert_equal(testOb->query("SELECT * FROM `Test`"), UNDEFINED),

        // drop table
        assert_equal(testOb->query("DROP TABLE `Test`"), UNDEFINED),
    }) :));
    expect("close stops a handle", (: ({
        // close
        assert_equal(testOb->close(), 1),
        assert_equal(testOb->query_handle(), 0),
    }) :));

    if (file_size("/save/test/database.db")) {
        rm("/save/test/database.db");
    }
}