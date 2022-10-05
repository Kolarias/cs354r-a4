#include <player.h>
#include <playercam.h>
#include <token.h>
#include <spike.h>

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<Player::Player>();
    godot::register_class<PlayerCam::PlayerCam>();
    godot::register_class<Token::Token>();
    godot::register_class<Spike::Spike>();
}