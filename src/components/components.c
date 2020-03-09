#include "../../engine.h"
#include <AL/al.h>
#include <AL/alc.h>


void transform_construct(Transform *const t)
{
    memset(t, 0, sizeof(Transform));
}


void transform_transform_calculate(Transform *const t, mat4 transform)
{
    glm_mat4_identity(transform);
    glm_translate(transform, t->position);
    glm_rotate_z(transform, t->rotation, transform);
}


float transform_distance_get(Transform *restrict t, Transform *restrict t2)
{
    //calculating distance with pythagoreas
    return sqrt(pow(t->position[0] - t2->position[0], 2) + pow(t->position[0] - t2->position[1], 2));
}


void soundSource_construct(SoundSource *const s, char const *const path)
{
	uint8_t channels, bps;
    uint32_t samplerate, size;
    ALvoid *data;

    data = wavFile_load(path, &channels, &samplerate, &bps, &size);

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, wav_format_get(channels, bps), data, size, samplerate);

    alGenSources(1, &s->source);
    alSourcei(s->source, AL_BUFFER, buffer);

    alDeleteBuffers(1, &buffer);
    che_free(data);
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
    
}


void renderable_destruct(Renderable *const r)
{

}


void camera_construct(Camera *const c)
{
    c->zoom = 1;
    c->aspectRatio = 1;
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


void camera_default_vp_recalculate(Camera *const c)
{
    mat4 view, proj; 
    checs_component_get_once(Transform, t, checs_entity_get_by_tag(CameraTag));

    glm_mat4_identity(view); //reset the view matrix
    glm_ortho(-c->zoom * c->aspectRatio, c->zoom * c->aspectRatio, -c->zoom, c->zoom, -1.f, 1.f, proj); 
    /* by setting the projection always according to the aspect ratio if the window gets resized everything still gets rendered correctly*/

    glm_translate(view, t->position); //moving the camera
    glm_scale_uni(view, c->zoom);
    glm_rotate_z(view, t->rotation, view);  //rotate it
    glm_mat4_inv(view, view);
    glm_mat4_mul(proj, view, c->vp);
}