# Mudlib Design

## Account Based Login System

Most MUDs are designed such that each "player character" is a self-contained "account", "character", "terminal preferences", and so on for each included module that contains savable variables / data.

Nightmare Residuum takes a different approach to this, both in terms of an account based system and a separation of data into modules.

The user object is used to represent a user connection, which consists of an account object and a character object. The user object also contains built-in shell functionality which interprets user input and routes it to the appropriate commands, verbs, or other available actions.

A user will create an account, represented by [STD_ACCOUNT](lib/doc/build/STD_ACCOUNT.md), which contains username, password, character list, account settings, last login, creation date, etc.

From there, a user creates player characters, represented by [STD_CHARACTER](lib/doc/build/STD_CHARACTER.md), which contains player stats such as level, stat attributes, species, last environment, last action, creation date, etc.

Due to this design, the standard `this_player()` efun is ambiguous into whether you are referring to a user object or a character object. These different objects may be referenced via sefuns:

`this_user()` - returns the interactive user's USER object

`this_character()` - returns [STD_CHARACTER](lib/doc/build/STD_CHARACTER.md) object of the interactive user

Chaining off of `this_user()` is also possible.

`this_user()->query_character()`

With the separation of account and characters, savefiles are much smaller than a standard MUD since each savefile does not contain a full account's data. A second effect is that the data for each of these user objects can be stored separate from one another, for example, /save/account/ will contain all account data, while /save/character/ will contain all character data, and the savefiles contained will consist of only the minimal amount of data necessary for that object type. This removes ambiguity of which modules include which variables into a savefile.

## Environments

The game world is represented by [STD_ROOM](lib/doc/build/STD_ROOM.md), which consist of a short (brief) description, a long (verbose) description, items in the room that can be looked at for more details, exits that link to other rooms, in addition to possible [STD_ITEM](lib/doc/build/STD_ITEM.md) and [STD_NPC](lib/doc/build/STD_NPC.md) contents.

## Objects

Basic objects that characters can interact with, such as getting, dropping, giving, or putting, are represented by [STD_ITEM](lib/doc/build/STD_ITEM.md).

Basic NPCs that can populate rooms are referenced by [STD_NPC](lib/doc/build/STD_NPC.md).

## Unit Tests

This project was built with testing in mind from the start with the [M_TEST](lib/doc/build/M_TEST.md) module for writing tests and D_TEST for running tests among the first files to be coded. Objects are broken down into small, easily testable modules that are as independent of other components as possible.

## Security

The access control system is implemented in `D_ACCESS` (`/secure/daemon/access.c`) and enforced via the `valid_read` and `valid_write` master applies in `master.c`.

### How access checks work

When any object performs a file read or write, the driver calls `valid_read` or `valid_write` in `master.c`, which delegates to `D_ACCESS->query_allowed(caller, fn, file, mode)`.

`query_allowed` supports three modes: `"read"`, `"write"`, and `"socket"`. Socket access is a special case — it bypasses config lookup entirely and is permitted only for the object at `/secure/daemon/ipc` or any object inheriting the HTTP module. For read and write modes, `query_allowed` performs three checks in order:

1. **Path lookup** — The target file path is matched against `read.cfg` (for reads) or `write.cfg` (for writes) to find the required privilege level. If the path is marked `ALL`, access is immediately granted without inspecting the call stack. If marked `NONE`, the call stack will be inspected but every object will be denied.

2. **Call stack inspection** — Every object in the call stack (the caller plus all `previous_object(-1)` entries) is evaluated by `check_stack_entry`. If *any* object fails, access is denied.

3. **Per-entry privilege check** (`check_stack_entry`) — For each stack entry, checks in order:
   - Internal objects (`D_ACCESS`, `MASTER`, `SEFUN`) are always trusted and skipped.
   - Objects with no `query_privs` result are denied.
   - Objects reading a path with no `read.cfg` entry are permitted (open read).
   - Objects with `ACCESS_SECURE` privilege bypass all path restrictions.
   - Objects whose privilege class matches the target file's class (via `query_file_privs`) are permitted.
   - Objects writing to a path with no `write.cfg` entry are denied.
   - Otherwise, the object's privileges must intersect the path's required privileges.

### Access level tiers

| Constant | Value | Assigned to |
|---|---|---|
| `ACCESS_ALL` | `"ALL"` | Used in cfg files to mean "any caller permitted" |
| `ACCESS_CMD` | `"COMMAND"` | Files in `/cmd/` |
| `ACCESS_ASSIST` | `"ASSIST"` | Files in `/std/` |
| `ACCESS_MUDLIB` | `"MUDLIB"` | Files in `/daemon/` |
| `ACCESS_SECURE` | `"SECURE"` | Files in `/secure/` and `/save/`; bypasses all path restrictions |

Realm files get the lowercased realm name as their privilege (e.g., `"player"`).
Domain files get the capitalized domain name (e.g., `"Midgaard"`).

### Config files

| File | Purpose |
|---|---|
| `lib/secure/etc/read.cfg` | Minimum privilege to *read* a path |
| `lib/secure/etc/write.cfg` | Minimum privilege to *write* a path |
| `lib/secure/etc/group.cfg` | Privilege group memberships |

Format: `(/path/) PRIV` or `(/path/) PRIV:PRIV2` for multiple accepted privileges.

**Common mistake:** Adding a restriction to `write.cfg` for a path that is `ALL` in `read.cfg` is correct — reads and writes use separate config lookups. A file can be publicly readable but write-restricted.

### Debugging access failures

Enable per-entry debug logging at runtime:

```lpc
D_ACCESS->set_debug(1);  // enable
D_ACCESS->set_debug(0);  // disable
```

Each stack entry evaluation will emit a `debug_message` showing the object, its privileges, the required privileges, and the result.