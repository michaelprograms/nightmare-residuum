# STD_CHARACTER - /std/character.c

## Inherits

STD_LIVING - /std/living.c
M_PARENT - /std/module/parent.c
M_SAVE - /secure/module/save.c

### Common Functionality

```c
int is_character ()
```
verify an object inherits STD_CHARACTER.

```c
void receive_message (string type, string message)
```
proxy for `this_user()->receive_message(type, message)`

```c
object query_user ()
```
returns character's user object

```c
int query_created ()
```
returns character's creation date

```c
int query_last_action ()
```
returns character's last action

```c
void describe_environment ()
```
describe a character's environment as a response from the 'look' verb

```c
mapping query_settings ()
```
return account settings