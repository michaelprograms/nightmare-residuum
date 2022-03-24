# M_TEST - /std/module/test.c

This module is inherited by test files to provide

## Lifecycle Functionality

```c
void before_all_tests ()
```
Run some initialization code which runs once, before all tests are executed.

```c
void before_each_test ()
```
Run some initialization code which runs before each test is executed.

```c
void after_each_test ()
```
Run some cleanup code which runs after each test is executed.

```c
void after_all_tests ()
```
Run some cleanup code which runs once, after all tests are executed.

```
string *test_order ()
```
If defined, can be used to instruct D_TEST runner to execute tests in this order.

```c
string *test_ignore () {
    return ::test_ignore() + ({ "local_public_function" });
}
```
If overridden, can be used instruct D_TEST runner to ignore additional local public functions.

## Expect Functionality

```c
void expect_function (string fn, object testOb)
```
Assert that testOb contains a public function matching fn. Due to limited ability of the test runner to determine if a function has been called, `expect_function(fn, testOb)` is used in a way that marks off the function as tested. Another way this can be done is by formatting the `message` of `expect` statements starts with the name of the function tested, but this is not always ideal.

```c
void expect (string message, function fn)
```
Start an `expect`ation of `assert`ions in function `fn`, while string `message` contains a human-readable description of what is being tested. `fn` should consist of a function that returns an array of `assert` evaluations.

```c
void assert (mixed left, string condition, mixed right)
```
Evaluate an `assert`ion inside of an `expect`ation. `left` should contain a queried value or result while `right` should contain the expected answer. `condition` changes how the comparison should evaluate.

Examples of conditions:
```c
    expect("conditions all pass", (: ({
        assert(1, "==", 1),
        assert(0, "!=", 1),
        assert(1, ">", 0),
        assert(1, ">=", 0),
        assert(0, "<", 1),
        assert(0, "<=", 1),
        assert("cat", "regex", "^.+at"),
        assert((: error("Error") :), "catch", "*Error\n"),
    }) :));
```