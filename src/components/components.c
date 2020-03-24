#include "../../engine.h"
#include <AL/al.h>
#include <AL/alc.h>


void transform_transform_calculate(Transform *const t, mat4 transform)
{
    glm_mat4_identity(transform);
    glm_translate(transform, t->pos);
    glm_rotate_z(transform, t->rot, transform);
}


float transform_distance_get(Transform *restrict t, Transform *restrict t2)
{
    //calculating distance with pythagoreas
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}


bool transform_circle_circle_collision(Transform *restrict t0, Transform *restrict t1, float const r0, float const r1)
{
    return r0 + r1 > sqrtf(pow(t0->pos[0] + r0 - t1->pos[0] - r1, 2) + pow(t0->pos[1] + r0 - t1->pos[1] - r1, 2));
}


bool transform_circle_point_collision(Transform *restrict t0, Transform *restrict t1, float const r0)
{
    return transform_distance_get(t0, t1) < r0;
}


bool transform_rect_rect_collision(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1)
{
    return t0->pos[0] + bb0[0] > t1->pos[0] && t0->pos[0] < t1->pos[0] + bb1[0] && 
           t0->pos[1] - bb0[1] < t1->pos[1] && t0->pos[1] > t1->pos[1] - bb1[1];
}


bool  transform_rect_point_collision(Transform *restrict t0, Transform *restrict t1, vec2 const bb0)
{
    return t1->pos[0] > t0->pos[0] && t1->pos[0] < t0->pos[0] + bb0[0] && t1->pos[1] > t0->pos[1] && t1->pos[1] < t0->pos[1] + bb0[1];
}


void soundSource_construct(SoundSource *const s, char const *const path)
{
	uint8_t channels, bps;
    uint32_t samplerate, size;
    ALvoid *const data = wavFile_load(path, &channels, &samplerate, &bps, &size);

    {
        ALuint buffer;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, wav_format_get(channels, bps), data, size, samplerate);
        alGenSources(1, &s->source);
        alSourcei(s->source, AL_BUFFER, buffer);
        alDeleteBuffers(1, &buffer);
    }

    che_scope_free(data);
}


void soundSource_destruct(SoundSource *const s)
{
	alDeleteSources(1, &s->source);
}


void soundSource_sound_play(SoundSource *const s)
{
	alSourcePlay(s->source);
}


void soundSource_position_set(SoundSource *const s, vec3 const pos)
{
    alSourcefv(s->source, AL_POSITION, pos);
}


void renderable_construct(Renderable *const r)
{
    vertexArray_construct(&r->vao);
}


void renderable_destruct(Renderable *const r)
{
    vertexArray_destruct(&r->vao);
}


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