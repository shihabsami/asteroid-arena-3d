#include "object_register.h"
#include "object_loader.h"
#include "../main/config.h"

void object_register::init() {

}

void object_register::init_arena() {
    arena = make_shared<game_arena>(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
}

void object_register::delete_arena() {
    arena.reset();
}

void object_register::init_spaceship() {

}

void object_register::delete_spaceship() {

}
