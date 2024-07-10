inherit M_TEST;

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
        assert_equal(db->query("SELECT * FROM `Test`"), UNDEFINED),

        // create
        db->query("CREATE TABLE `Test` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, `thing` TEXT NOT NULL )"),

        // table is empty
        assert_equal(db->query("SELECT * FROM `Test`"), UNDEFINED),

        // insert into table
        db->query("INSERT INTO `Test` (thing) VALUES ('some thing')"),
        db->query("INSERT INTO `Test` (thing) VALUES ('another thing')"),

        // table has records
        assert_equal(db->query("SELECT * FROM `Test`"), ({ ({ 1, "some thing" }), ({ 2, "another thing" }) })),

        // delete table records
        assert_equal(db->query("DELETE FROM `Test`"), UNDEFINED),

        // table is empty
        assert_equal(db->query("SELECT * FROM `Test`"), UNDEFINED),

        // drop table
        assert_equal(db->query("DROP TABLE `Test`"), UNDEFINED),
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