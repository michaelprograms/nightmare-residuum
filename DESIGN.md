# Mudlib Design

## Account Based Login System

Most MUDs are designed such that each "player character" is a self-contained "account", "character", "terminal preferences", and so on for each included module that contains savable variables / data.

Nightmare Residuum takes a different approach to this, both in terms of an account based system and a separation of data into modules.

The user object is used to represent a user connection, which consists of an account object, a character object, and a shell object.

A user will create an account, represented by [STD_ACCOUNT](lib/doc/build/STD_ACCOUNT.md), which contains username, password, character list, account settings, last login, creation date, etc.

From there, a user creates player characters, represented by [STD_CHARACTER](lib/doc/build/STD_CHARACTER.md), which contains player stats such as level, stat attributes, species, last environment, last action, creation date, etc.

As a user enters the world with a character, a shell is created for the user which interprets user input and routes it to the appropriate commands, verbs, or other available actions.

Due to this design, the standard `this_player()` efun is ambigious into whether you are referring to a user object, a character object, or a shell object. These different objects may be referenced via sefuns:

`this_user()` - returns the interactive user's USER object
`this_account()` - returns [STD_ACCOUNT](lib/doc/build/STD_ACCOUNT.md) of the interactive user
`this_character()` - returns [STD_CHARACTER](lib/doc/build/STD_CHARACTER.md) object of the interactive user

Chaining off of `this_user()` is also possible.
`this_user()->query_account()`
`this_user()->query_character()`
`this_user()->query_shell()`

With the separation of account and characters, savefiles are much smaller than a standard MUD since each savefile does not contain a full account's data. A second effect is that the data for each of these user objects can be stored separate from one another, for example, /save/account/ will contain all account data, while /save/character/ will contain all character data, and the savefiles contained will consist of only the minimal amount of data necessary for that object type. This removes ambiguity of which modules include which variables into a savefile.

## Environments

The game world is represented by [STD_ROOM](lib/doc/build/STD_ROOM.md), which consist of a short (brief) description, a long (verbose) description, items in the room that can be looked at for more details, exits that link to other rooms, in addition to possible [STD_ITEM](lib/doc/build/STD_ITEM.md) and [STD_NPC](lib/doc/build/STD_NPC.md) contents.

## Objects

Basic objects that characters can interact with, such as getting, dropping, giving, or putting, are represented by [STD_ITEM](lib/doc/build/STD_ITEM.md).

Basic NPCs that can populate rooms are referenced by [STD_NPC](lib/doc/build/STD_NPC.md).