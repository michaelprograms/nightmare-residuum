# STD_ROOM - /std/room.c

## Inherits

STD_CONTAINER - /std/container.c
M_RESET - /std/module/reset.c
/std/module/exit.c
/std/module/look.c

### Common Functionality

```c
int is_room ()
```
verify an object inherits STD_ROOM.

```c
object *query_living_contents ()
```
returns a list of all objects inheriting STD_LIVING within this object

```c
object *query_living_contents ()
```
returns a list of all objects inheriting STD_LIVING within this object

```c
int clean_u p()
```
overrides M_CLEAN->clean_up() to prevent clean up in certain situations.