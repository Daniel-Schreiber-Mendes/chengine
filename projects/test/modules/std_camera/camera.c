#include "camera.h"


void camera_construct(Camera *const c)
{
    c->zoom = 1;
}


void camera_default_zoom(Camera *const c, float const yoffset)
{
    /*the offset has to be between -1 and 1*/
    c->zoom += yoffset * 0.05f;
    c->zoom = fmaxf(c->zoom, 0.05f);
}


void camera_default_resize(Camera *const c, uint16_t const width, uint16_t const height)
{
    glViewport(0, 0, width, height);
    c->aspectRatio = width / (float)height;
}


void camera_target_set(Camera *const c, Transform *const t)
{
    c->target = t;
}


void camera_default_vp_recalculate(Camera *const c)
{
    mat4 view, proj; 
    checs_component_get_once(Transform, t, checs_entity_get_by_tag(CameraTag));

    //if the camera has a target, interpolate their poss which makes the camera smoothly follow the target
    if (c->target)
    {
        glm_vec3_lerp(t->pos, c->target->pos, 0.1f, t->pos);
    }

    glm_mat4_identity(view); //reset the view matrix
    glm_ortho(-c->zoom * c->aspectRatio, c->zoom * c->aspectRatio, -c->zoom, c->zoom, -1.f, 1.f, proj); 
    /* by setting the projection always according to the aspect ratio if the window gets resized everything still gets rendered with the correct aspect ratio*/
    glm_translate(view, t->pos); //moving the camera
    glm_scale_uni(view, c->zoom);
    glm_rotate_z(view, t->rot, view);  //rotate it
    glm_mat4_inv(view, view); //if camera moves left, everything else should move right. because of this we have to inverse the direction
    glm_mat4_mul(proj, view, c->vp);
}