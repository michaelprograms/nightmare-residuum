nosave private string __Ability = split_path(file_name())[1];

int calculate_damage (object source, object target) {
    int damage;

    // base damage
    damage = secure_random(10);
    damage += (source->query_stat("strength") * 10 / 100);
    damage += secure_random(source->query_stat("strength") * 10 / 100 + 1);
    damage += secure_random(source->query_sp() * 10 / 100 + 1);
    damage += secure_random(source->query_stat("luck") * 5 / 100 + 1);
    damage += secure_random(source->query_skill("melee attack") * 20 / 100 + 1);

    // apply target mitigations
    damage -= (target->query_stat("endurance") * 10 / 100);
    damage -= secure_random(target->query_stat("endurance") * 10 / 100 + 1);
    damage -= secure_random(target->query_hp() * 10 / 100 + 1);
    damage -= secure_random(target->query_stat("luck") * 10 / 100 + 1);
    damage -= secure_random(target->query_skill("melee defense") * 20 / 100 + 1);
    // damage -= target->query_limb_armor(limb); // @TODO limb

    return damage;
}