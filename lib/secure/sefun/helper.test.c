inherit M_TEST;

/**
 * @var {"/secure/sefun/helper"} testOb
 */

void test_helpers () {
    object living = new(STD_LIVING);
    object npc = new(STD_NPC);
    object character = new(STD_CHARACTER);
    object item = new(STD_ITEM);
    object armor = new(STD_ARMOR);
    object weapon = new(STD_WEAPON);
    object food = new(STD_FOOD);
    object drink = new(STD_DRINK);
    object room = new(STD_ROOM);

    expect("undefined handled appropriately", (: ({
        assert_equal(testOb->livingp(UNDEFINED), 0),
        assert_equal(testOb->npcp(UNDEFINED), 0),
        assert_equal(testOb->characterp(UNDEFINED), 0),
        assert_equal(testOb->immortalp(UNDEFINED), 0),
        assert_equal(testOb->itemp(UNDEFINED), 0),
        assert_equal(testOb->armorp(UNDEFINED), 0),
        assert_equal(testOb->weaponp(UNDEFINED), 0),
        assert_equal(testOb->foodp(UNDEFINED), 0),
        assert_equal(testOb->drinkp(UNDEFINED), 0),
        assert_equal(testOb->roomp(UNDEFINED), 0),
    }) :));
    expect("living handled appropriately", (: ({
        assert_equal(testOb->livingp($(living)), 1),
        assert_equal(testOb->npcp($(living)), 0),
        assert_equal(testOb->characterp($(living)), 0),
        assert_equal(testOb->immortalp($(living)), 0),
        assert_equal(testOb->itemp($(living)), 0),
        assert_equal(testOb->armorp($(living)), 0),
        assert_equal(testOb->weaponp($(living)), 0),
        assert_equal(testOb->foodp($(living)), 0),
        assert_equal(testOb->drinkp($(living)), 0),
        assert_equal(testOb->roomp($(living)), 0),
    }) :));
    expect("npc handled appropriately", (: ({
        assert_equal(testOb->livingp($(npc)), 1),
        assert_equal(testOb->npcp($(npc)), 1),
        assert_equal(testOb->characterp($(npc)), 0),
        assert_equal(testOb->immortalp($(npc)), 0),
        assert_equal(testOb->itemp($(npc)), 0),
        assert_equal(testOb->armorp($(npc)), 0),
        assert_equal(testOb->weaponp($(npc)), 0),
        assert_equal(testOb->foodp($(npc)), 0),
        assert_equal(testOb->drinkp($(npc)), 0),
        assert_equal(testOb->roomp($(npc)), 0),
    }) :));
    expect("character handled appropriately", (: ({
        assert_equal(testOb->livingp($(character)), 1),
        assert_equal(testOb->npcp($(character)), 0),
        assert_equal(testOb->characterp($(character)), 1),
        assert_equal(testOb->immortalp($(character)), 0),
        $(character)->set_immortal(1),
        assert_equal(testOb->immortalp($(character)), 1),
        assert_equal(testOb->itemp($(character)), 0),
        assert_equal(testOb->armorp($(character)), 0),
        assert_equal(testOb->weaponp($(character)), 0),
        assert_equal(testOb->foodp($(character)), 0),
        assert_equal(testOb->drinkp($(character)), 0),
        assert_equal(testOb->roomp($(character)), 0),
    }) :));
    expect("item handled appropriately", (: ({
        assert_equal(testOb->livingp($(item)), 0),
        assert_equal(testOb->npcp($(item)), 0),
        assert_equal(testOb->characterp($(item)), 0),
        assert_equal(testOb->immortalp($(item)), 0),
        assert_equal(testOb->itemp($(item)), 1),
        assert_equal(testOb->armorp($(item)), 0),
        assert_equal(testOb->weaponp($(item)), 0),
        assert_equal(testOb->foodp($(item)), 0),
        assert_equal(testOb->drinkp($(item)), 0),
        assert_equal(testOb->roomp($(item)), 0),
    }) :));
    expect("armor handled appropriately", (: ({
        assert_equal(testOb->livingp($(armor)), 0),
        assert_equal(testOb->npcp($(armor)), 0),
        assert_equal(testOb->characterp($(armor)), 0),
        assert_equal(testOb->immortalp($(armor)), 0),
        assert_equal(testOb->itemp($(armor)), 1),
        assert_equal(testOb->armorp($(armor)), 1),
        assert_equal(testOb->weaponp($(armor)), 0),
        assert_equal(testOb->foodp($(armor)), 0),
        assert_equal(testOb->drinkp($(armor)), 0),
        assert_equal(testOb->roomp($(armor)), 0),
    }) :));
    expect("weapon handled appropriately", (: ({
        assert_equal(testOb->livingp($(weapon)), 0),
        assert_equal(testOb->npcp($(weapon)), 0),
        assert_equal(testOb->characterp($(weapon)), 0),
        assert_equal(testOb->immortalp($(weapon)), 0),
        assert_equal(testOb->itemp($(weapon)), 1),
        assert_equal(testOb->armorp($(weapon)), 0),
        assert_equal(testOb->weaponp($(weapon)), 1),
        assert_equal(testOb->foodp($(weapon)), 0),
        assert_equal(testOb->drinkp($(weapon)), 0),
        assert_equal(testOb->roomp($(weapon)), 0),
    }) :));
    expect("food handled appropriately", (: ({
        assert_equal(testOb->livingp($(food)), 0),
        assert_equal(testOb->npcp($(food)), 0),
        assert_equal(testOb->characterp($(food)), 0),
        assert_equal(testOb->immortalp($(food)), 0),
        assert_equal(testOb->itemp($(food)), 1),
        assert_equal(testOb->armorp($(food)), 0),
        assert_equal(testOb->weaponp($(food)), 0),
        assert_equal(testOb->foodp($(food)), 1),
        assert_equal(testOb->drinkp($(food)), 0),
        assert_equal(testOb->roomp($(food)), 0),
    }) :));
    expect("drink handled appropriately", (: ({
        assert_equal(testOb->livingp($(drink)), 0),
        assert_equal(testOb->npcp($(drink)), 0),
        assert_equal(testOb->characterp($(drink)), 0),
        assert_equal(testOb->immortalp($(drink)), 0),
        assert_equal(testOb->itemp($(drink)), 1),
        assert_equal(testOb->armorp($(drink)), 0),
        assert_equal(testOb->weaponp($(drink)), 0),
        assert_equal(testOb->foodp($(drink)), 0),
        assert_equal(testOb->drinkp($(drink)), 1),
        assert_equal(testOb->roomp($(drink)), 0),
    }) :));
    expect("room handled appropriately", (: ({
        assert_equal(testOb->livingp($(room)), 0),
        assert_equal(testOb->npcp($(room)), 0),
        assert_equal(testOb->characterp($(room)), 0),
        assert_equal(testOb->immortalp($(room)), 0),
        assert_equal(testOb->itemp($(room)), 0),
        assert_equal(testOb->armorp($(room)), 0),
        assert_equal(testOb->weaponp($(room)), 0),
        assert_equal(testOb->foodp($(room)), 0),
        assert_equal(testOb->drinkp($(room)), 0),
        assert_equal(testOb->roomp($(room)), 1),
    }) :));

    destruct(living);
    destruct(npc);
    destruct(character);
    destruct(item);
    destruct(armor);
    destruct(weapon);
    destruct(food);
    destruct(drink);
    destruct(room);
}