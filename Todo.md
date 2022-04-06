- [ ] cleanup local_options (can prune extraneous?)
- [ ] cleanup config (can prune extraneous?)
- [ ] lib/doc and autodoc
- [ ] ansi.h to simplify daemon/ansi
- [ ] master()->log_error("/realm/name/test.c",...) logs to /log/name, should be /log/realm/name?
Todo!

consolidate /std/living/location.c into /std/module/move.c

tail, more
wild_card updates
D_ACCESS valid checks?

STD_CHARACTER->query_account_name for whois
test/catch bad arguments to sefun/grammar
fix cmd update/test vs driver run-tests
/secure/daemon/master parsing - cleanup parser_error_message and command_users update
compare driver/compat/parser_error.c to master->parser_error
grammar->pluralize ABNORMAL_WORDS mapping
container->query_contents?
set_name vs set_cap_name
check objects cleaning up
master()->valid_read|write log_file on fail
todo fix error messages
tests before_each_test vs before_all_tests
tests after_all_tests cleanup testOb
tests should fail on CAUGHT DURING TEST
showtree - lima
Human to Terran?
use file_length vs file_size
helpfiles (commands/verbs)
/realm/name/bin command path

cleanup message types and receive_message for wrap/terminal_colour LARGEST_PRINTABLE_STRING
12345678901234567890123456789012345678901234567890123456789012345678901234567890



currencies, physical wallet to print physical/resource?
- credit, digital
- physical, off the books
- resource



OBJ_USER rename?

eval -include [path] -inherit [path|NAME] [code]

ed notes
    -merentha /doc/help/creator/ed





weird colors
    188;104;185
    202;104;225
    103;100;135
    172;101;184
    67;101;209
    216;202;107
    235;216;104
    134;101;213
    124;151;107
    252;86;103
    212;238;100
    67;101;148
    189;106;124
    198;220;102

#845Ec2 -   https://mycolor.space/?hex=%23845EC2&sub=1

# Color Palette
https://blog.depositphotos.com/15-cyberpunk-color-palettes-for-dystopian-designs.html
https://blog.depositphotos.com/wp-content/uploads/2020/01/Cyberpunk-color-palettes_10.jpg.webp


Primary:    05D9E8 (teal)
Secondary:  005678 (blue)
            01012B (too dark blue)
Accent      FF2A6B (bright pink)
Accent      D1F7FF (light blue)


https://material.io/design/color/the-color-system.html#tools-for-picking-colors
Primary         EEEE99  #e7e771
Complementary   9999EE  #7171e7
Analogous 1     EEC399
Analogous 2     C3EE99
Triadic 1       99EE99
Triadic 2       99EEEE

Emphasis High   FFFFFF + BOLD (white)
Emphasis Medium FFFFFF (white)
Emphasis Low    grayscale



# Master Applies
- [x] compile_object        master              done
- [x] connect               master              done
- [x] crash                 master              done
- [x] epilog                master/startup      done
- [x] error_handler         master              done
- [x] flag                  master/startup      done
- [x] get_include_path      master              done
- [ ] get_save_file_name    master/ed           todo
- [x] log_error             master              revisit messaging user
- [x] make_path_absolute    master/ed           todo
- [x] object_name           master              done
- [x] preload               master/startup      done
- [x] privs_file            master              done
- [ ] retrieve_ed_setup     master/ed           todo
- [ ] save_ed_setup         master/ed           todo
- [ ] valid_object          master/valid
- [x] valid_override        master/valid        done
- [x] valid_read            master/valid        done
- [x] valid_socket          master/valid        revisit D_ACCESS->query_allowed(...,"socket")
- [x] valid_write           master/valid        done
- [x] get_mud_stats         master              done

- [?] valid_bind            master/valid

# Applies not implimenting
- [ ] author_file           master/stats
- [ ] creator_file          master/stats
- [ ] domain_file           master/stats
- [ ] get_bb_uid            master/uid
- [ ] get_root_uid          master/uid
- [ ] valid_save_binary     master/valid
- [ ] valid_database        master/valid
- [ ] valid_hide            master/valid
- [ ] valid_link            master/valid
- [ ] valid_seteuid         master/valid
- [ ] valid_shadow          master/valid
- [ ] view_errors           master

# Sefun


# Test
- [ ] expand validate fail to mark position that was different with ^
- [ ] fix test daemon aborting when a test fails
- [ ] verify can handle async tests
- [ ] time_expression properly
- [ ] async properly
- [ ] dynamically find .tests and create runners
- [ ] make empty runners/suites fail
- [ ] shouldFail / passedfailed
- [ ] assertion counts
- [ ] swap between write() and debug_message(strip_colours())


Notes:
https://ringbreak.dnd.utwente.nl/~krimud/Docs/NMAdmin/


https://ringbreak.dnd.utwente.nl/~krimud/build/Introduction.txt
```
Writing an event in LPC is nothing more that providing step by step
instructions for what happens when a given event occurs.  The LPC term
for an event is a function (sometimes you will hear people refer to
functions as methods).  The Nightmare Object Library uses four types of
events:

#1 MudOS initiated events, sometimes called an "apply"
The names of these events are all lower case.  The most common apply
is the create() event.  It is caused by the creation of the object.

#2 Attribute manipulation events
These are the SetXXX() and GetXXX() events.  They exist basically to
allow other objects to find out about an object's attributes, or to do
initial setup for an object's attributes.

#3 Modal events
These are the CanXXX() events.  They get called often just prior to a
behaviour type of an event.  For example, I am trying to leave a room,
so my leave behaviour asks the room if I can leave.  This would be
CanLeave() in the room.  If CanLeave() says I can leave, then the
leave event is triggered in the room.

#4 True events
A true event is identified by eventXXX().  These type of events are
the meat of what is happening in the game.  Any given true event is
generally in turn triggered by some other true event.  The chain
generally can be traced as starting with a player or NPC command at
some point.
```

A coalition of Type II civilizations, (stellar civilization—can use and control
energy at the scale of its stellar system) which failed to surmount the
obstacles to becoming a Type III civilization. - https://en.wikipedia.org/wiki/Kardashev_scale
