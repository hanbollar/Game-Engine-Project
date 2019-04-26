#include "rig_solver.h"

RigSolver::RigSolver() : target_(glm::vec3(0.f))
{}

RigSolver::~RigSolver()
{}

void RigSolver::SolveCurrent(Bone* endEffector) {
    // using cyclic order of descent method

    glm::vec3 end_pos = endEffector->GetWorldPosition();
    Bone* on_bone = endEffector->GetParent();

    float error = glm::distance(end_pos, target_);
    int on_bone_iter_away_from_end = 0;

    for (int i = 0; i < NUM_ITER && error > THRESHOLD; ++i) {
        // checking configuration of current bone relative to target
        glm::vec3 pivot_to_affector = glm::normalize(glm::vec3(end_pos - on_bone->GetWorldPosition()));
        glm::vec3 pivot_to_target = glm::normalize(glm::vec3(target_ - on_bone->GetWorldPosition()));
        float projection = glm::dot(pivot_to_affector, pivot_to_target);
        float angle = glm::degrees(glm::acos(projection));
        if (projection >= 1) { break; } // not unitized. bad value.

        // get the rotation direction
        glm::vec3 axis_temp = glm::normalize(glm::cross(pivot_to_affector, pivot_to_target));
        glm::vec3 axis = glm::vec3(glm::mat3(on_bone->GetInverseWorldTransform()) * axis_temp);

        // error check, we done?
        error = glm::distance(end_pos, target_);
        if (error < THRESHOLD) { break; }

        // update for this new rotation
        if (axis != glm::vec3())  {
            on_bone->SetLocalTransform(glm::rotate(glm::mat4(1.f), angle, axis)); // back to world space
        }

        // final update
        end_pos = endEffector->GetWorldPosition();

        // setup for next loop
        if (on_bone->GetParent()->GetParent() == nullptr
            || on_bone_iter_away_from_end > NUM_ITER_BONES) {
            // loop back around and start from end affector again
            on_bone = endEffector->GetParent();
            on_bone_iter_away_from_end = 0;
        } else {
            on_bone = on_bone->GetParent();
            on_bone_iter_away_from_end++;
        }
    }
}