string describe_living_item (object ob) {
    string tag = "";
    int pct, maxhp;
    if (ob->query_defeated()) {
        tag += " (defeated)";
    } else {
        maxhp = ob->query_max_hp();
        if (maxhp > 0) {
            pct = ob->query_hp() * 100 / maxhp;
            if (pct < 0) {
                pct = 0;
            }
            switch (pct) {
                case 72..95: tag = " (bruised)"; break;
                case 48..71: tag = " (injured)"; break;
                case 24..47: tag = " (bleeding)"; break;
                case 0..23:  tag = " (dying)"; break;
            }
        }
        switch (ob->query_posture()) {
            case "flying": case "laying": case "meditating": case "sitting":
                tag += " ("+ob->query_posture()+")"; break;
        }
    }
    return ob->query_short("%^I_RED%^BOLD%^") + tag;
}