void command (string input, mapping flags) {
    string left = "%^A bunch of %^UNDERLINE%^text that %^RED%^has some colors%^UNDERLINE_OFF%^ %^BOLD%^and style%^BOLD_OFF%^ set into it that%^RESET%^ will %^GREEN%^wrap around to the next line and retain its color and%^RESET%^ padding.\n\n%^B_RED%^Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.%^RESET%^\n\n01 02 03 04 05 06 07 08 09 10 11 12 %^GREEN%^BOLD%^13 14 15 16 17%^RESET%^ 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 \n\n"+"A %^GREEN%^%^BOLD%^large green field%^RESET%^ stretches out as far as the eye can see. The %^GREEN%^%^BOLD%^green grass%^RESET%^ sways lightly with the %^CYAN%^cool, gentle breeze%^RESET%^.";
    string right = "%^CYAN%^BOLD%^At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident, similique sunt in culpa qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem rerum facilis est et expedita distinctio.%^RESET%^\n\n%^MAGENTA%^BOLD%^Viverra nibh cras pulvinar mattis nunc sed. Ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit. At imperdiet dui accumsan sit amet nulla facilisi morbi. Orci sagittis eu volutpat odio. Scelerisque fermentum dui faucibus in ornare quam viverra. Et malesuada fames ac turpis. Massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin. Tellus in hac habitasse platea dictumst vestibulum rhoncus est pellentesque. Sed vulputate odio ut enim blandit volutpat. Id diam vel quam elementum pulvinar etiam. Tristique senectus et netus et malesuada fames ac turpis egestas. Pellentesque diam volutpat commodo sed. Dui nunc mattis enim ut tellus. Tempor commodo ullamcorper a lacus vestibulum sed arcu darfu fardu non odio.%^RESET%^";
    string *leftLines, *rightLines;
    string line, result = "";
    int l;

    leftLines = explode(wrap_ansi(left, 40), "\n");
    rightLines = explode(wrap_ansi(right, 40), "\n");

    l = max(({ sizeof(leftLines), sizeof(rightLines) }));
    for (int i = 0; i < l; i ++) {
        line = "";
        if (i < sizeof(leftLines)) {
            line += leftLines[i];
        } else {
            line += sprintf("%40' 's", "");
        }
        if (i < sizeof(rightLines)) {
            line += rightLines[i];
        }
        result += line + "\n";
    }
    message("system", result, this_character());
}