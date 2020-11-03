#include "camera.h"


void camera_construct(Camera *const c)
{
    c->zoom = 1;
}


void camera_zoom(Camera *const c, float const zoom)
{
    c->zoom += zoom;
}


void camera_resize(Camera *const c, uint16_t const width, uint16_t const height)
{
    glViewport(0, 0, width, height);
    c->aspectRatio = width / (float)height;
}


void camera_target_set(Camera *const c, Transform *const t)
{
    c->target = t;
}


void camera_vp_recalculate(EntityId const id, mat4 vp)
{
    mat4 view = GLM_MAT4_IDENTITY_INIT;

    checs_component_get_once(Transform, t, id);
    checs_component_get_once(Camera, c, id);

    //if the camera has a target, interpolate their positions which makes the camera smoothly follow the target
    if (c->target)
    {
        glm_vec2_lerp(t->pos, c->target->pos, 0.08f, t->pos);
    }

    /* by setting the projection always according to the aspect ratio if the window gets resized everything still gets rendered with the correct aspect ratio*/
    glm_scale_uni(view, c->zoom);
    glm_translate(view, t->pos); 
    glm_rotate_z(view, c->rot, view); 
    glm_mat4_inv(view, view); //if camera moves left, everything else should move right. because of this we have to inverse the direction
    glm_ortho(-c->zoom * c->aspectRatio, c->zoom * c->aspectRatio, -c->zoom, c->zoom, -1.f, 1.f, vp); //calulating and saving proj in vp
    glm_mat4_mul(vp, view, vp);
}