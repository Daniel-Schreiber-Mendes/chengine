#include "render.h"

void camera_import(void)
{
    checs_component_register(<Camera>, sizeof(Camera), NULL, NULL, 1 << <Transform>);
}


void camera_resize(Camera *const c, uint16_t const width, uint16_t const height)
{
    glViewport(0, 0, width, height);
    c->aspectRatio = width / (float)height;
}


void camera_vp_recalculate(EntityId const id, mat4 vp)
{
    mat4 view = GLM_MAT4_IDENTITY_INIT;

    Transform *t = checs_component_get(<Transform>, id);
    Camera *c = checs_component_get(<Camera>, id);

    if (c->target)
    {
        Transform *target_t = checs_component_get(<Transform>, c->target);
        glm_vec2_lerp(t->pos, target_t->pos, 0.2f, t->pos);
    }

    /* by setting the projection always according to the aspect ratio if the window gets resized everything still gets rendered with the correct aspect ratio*/
    glm_translate(view, t->pos); 
    glm_rotate_z(view, c->rot, view); 
    glm_mat4_inv(view, view); //if camera moves left, everything else should move right. because of this we have to inverse the direction
    glm_ortho(-c->zoom * c->aspectRatio, c->zoom * c->aspectRatio, -c->zoom, c->zoom, -1.f, 1.f, vp); //calulating and saving proj in vp
    glm_mat4_mul(vp, view, vp);
}