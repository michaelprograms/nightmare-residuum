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
Assert that testOb contains a public function matching fn. Due to limited ability of the test runner to determine if a function has been called, `expect_function(fn, testOb)` is used in a way that marks off the function as tested. Another way this can be done is by formatting the `message` of `expect_*` statements starts with the name of the function tested, but this is not always ideal.

```c
void expect_true (mixed value, string message)
```
Compare the value to true.

```c
void expect_false (mixed value, string message)
```
Compare the value to false.

```c
void expect_arrays_array_equal (mixed *left, mixed *right, string message)
```
Compare each array in the array left is equal to the array right.

```c
void expect_array_strings_equal (string *left, string right, string message)
```
Compare the string array left to the string right.

```c
void expect_arrays_equal (mixed *left, mixed *right, string message)
```
Compare the array left to the array right.

```c
void expect_strings_equal (string left, string right, string message)
```
Compare the string left to the string right.

```c
void expect_strings_regexp (string left, string right, string message)
```
Compare the string left to the regular expression right, example: `expect_strings_regexp(str, "^regular expression$", msg);`

```c
void expect_integers_equal (int left, int right, string message)
```
Compare the integer left to the integer right.

```c
void expect_catch (mixed expr, string right, string message)
```
Compare expr evaluation for an error matching to right.

```c
void expect_catches (mixed *expr, string right, string message)
```
Compare an array of expr evaluations for any errors matching right.