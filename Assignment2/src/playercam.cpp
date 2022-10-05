#include "playercam.h"
#include "player.h"

#define M_PI 3.14159265358979323846

namespace PlayerCam {

void PlayerCam::_register_methods()
{
    register_method("_ready", &PlayerCam::_ready);
    register_method("_input", &PlayerCam::_input);
    register_method("_process", &PlayerCam::_process);
    register_property<PlayerCam, float>("mouse_sensitivity", &PlayerCam::mouse_sensitivity, float(0.1));
}

void PlayerCam::_init() {
    mouse_sensitivity = float(0.1);
    input = Input::get_singleton();
    input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
}

void PlayerCam::_ready() {
    Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"))->connect("input_event", this, "_input");
}

void PlayerCam::_input(InputEvent* event) {
    // Hard-coded paths! Consider making more dynamic?
    Player::Player* player = Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"));
    Spatial* pivot = Object::cast_to<Spatial>(Node::get_node("/root/Level/Player/Pivot"));

    if (input->is_action_just_pressed("ui_cancel")) {
        input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    }

    InputEventMouseButton *m1 = Object::cast_to<InputEventMouseButton>(event);
    if (m1) {
        if (m1->get_button_index() == 1) // Left click
            input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
    }

    InputEventMouseMotion *m2 = Object::cast_to<InputEventMouseMotion>(event);
    if (m2) {
        if (!player->is_on_ledge() && player->can_mouse_rotate()) {
            player->rotate_y((-m2->get_relative().x * mouse_sensitivity) * (M_PI / 180));
            pivot->rotate_z((-m2->get_relative().y * mouse_sensitivity) * (M_PI / 180));
            // clamp so rotating vertically doesn't loop back over again
            pivot->set_rotation(Vector3(pivot->get_rotation().x, pivot->get_rotation().y,
                (std::max<double>(-80 * (M_PI / 180), std::min<double>(pivot->get_rotation().z,
                20 * (M_PI / 180))))));
        }
        else if (!player->can_mouse_rotate()){
            pivot->rotate_y((-m2->get_relative().x * mouse_sensitivity) * (M_PI / 180));
        }
    }
}

void PlayerCam::_process(float delta) {
    // Hard-coded paths! Consider making more dynamic?
    Spatial* camera = Object::cast_to<Spatial>(Node::get_node("/root/Level/Player/Pivot/Camera"));
    RayCast* camcollider = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/Pivot/CamCollider"));

    if (camcollider->is_colliding()) {
        camera->set_global_transform(Transform(camera->get_global_transform().basis, camcollider->get_collision_point()));
    } else {
        camera->set_translation(camcollider->get_cast_to());
    }
}

}