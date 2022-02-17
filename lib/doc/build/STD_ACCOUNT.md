# STD_ACCOUNT - /std/account.c

## Inherits

M_PARENT - /std/module/parent.c
M_SAVE - /secure/module/save.c
M_PROPERTY - /std/module/property.c

### Common Functionality

```c
int is_account ()
```
verify an object inherits STD_ACCOUNT.

```c
string query_created ()
string query_last_on ()
```
Return dates for account creation and last on.

```c
mapping query_character (string name)
```
returns character data about an account's character

```c
string *query_character_names ()
```
returns list of character names for an account

```c
void set_setting (string key, mixed value)
```
change an account setting

```c
mixed query_setting (string key)
```
returns an account setting

```c
mapping query_settings ()
```
return account settings