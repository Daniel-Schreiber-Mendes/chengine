#include "render.h"

static RectBatch rectBatch;

static void rectBatch_render(void);
static void rectBatch_setup(void);

static uint32_t ambient_program;
static void ambientShader_setup(void);
static void ambientShader_render(void);


void render_import(void)
{
	checs_component_register(5, sizeof(Renderable), NULL, NULL, 1 << 1);
	checs_tasks_register(CHECS_ON_DRAW, render_task, 1);
}


void render_task_init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); //is needed so a pixel with its z coordinate at -1 appers behind one with 0
	glClearColor(0.8, 0.4, 1, 1);

	int max_vertex_attrib_bindings;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &max_vertex_attrib_bindings);
	assert(max_vertex_attrib_bindings >= 16);

	rectBatch_setup();
	ambientShader_setup();
}


void render_task(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rectBatch_render();
	ambientShader_render();

	window_buffers_swap();
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("OpenGl Error Code: %x\n", error);
		assert(false);
	}
}


static void rectBatch_render(void)
{
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(rectBatch.vao.id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectBatch.ebo);
	glBindBuffer(GL_ARRAY_BUFFER, rectBatch.pi_vbo);
	glUseProgram(rectBatch.program);
	glBindTexture(GL_TEXTURE_2D, rectBatch.image->id);


	glBindBuffer(GL_UNIFORM_BUFFER, rectBatch.camera_ubo);
	camera_vp_recalculate(rectBatch.camera, glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
	glUnmapBuffer(GL_UNIFORM_BUFFER);


	//change instance buffer size if it is to small
	if (checs_component_count(5) > rectBatch.renderableCount)
	{
		glBufferData(GL_ARRAY_BUFFER, checs_component_count(5) * rectBatch.vao.instance_size, NULL, GL_STREAM_DRAW); //update instance buffer size
		rectBatch.renderableCount = checs_component_count(5);
	}

	void *p = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); //start of memory location for updating instance buffer
	assert(p);


	Renderable *r;
	uintEC i = 0;
	for (EntityId entity; r = checs_components_foreach(5, &entity, i); ++i)
	{
    	assert(r->texture);

		float *const transform = p + (i * rectBatch.vao.instance_size);	
		float *const textureSize = transform + 16; 
		float *const texture_pos = textureSize + 2;

		glm_mat4_identity(transform);
    	glm_translate(transform, checs_component_get(1, entity));
    	glm_rotate_z(transform, r->rot, transform);
    	glm_translate(transform, (vec3){r->offset[0], r->offset[1], 0});

    	glm_scale(transform, (vec3){r->scale[0], r->scale[1], 1});

    	glm_vec2_copy(r->texture->size, textureSize);
    	glm_vec2_copy(r->texture->pos, texture_pos);
    }

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glDrawElementsInstanced(rectBatch.mode, rectBatch.ebo_size, GL_UNSIGNED_INT, NULL, i);

}


static void rectBatch_setup(void)
{
	glGenVertexArrays(1, &rectBatch.vao.id);
	glBindVertexArray(rectBatch.vao.id);

    rectBatch.mode = GL_TRIANGLES;

    GLuint elements[] = 
    { //indices of the triangle
        0, 1, 2, //triangle 1
        2, 3, 0 // triangle 2
    };
    rectBatch.ebo_size = 6;
	glGenBuffers(1, &rectBatch.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectBatch.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    //per vertex
	GLfloat positions[] = 
    {//  3 x position  2 x tex
        -0.5,  0.5, 1, 0, 0, //ecke links oben
         0.5,  0.5, 1, 1, 0, //ecke rechts oben
         0.5, -0.5, 1, 1, 1, //ecke rechts unten
        -0.5, -0.5, 1, 0, 1 //ecke links unten
    };

	glGenBuffers(1, &rectBatch.pv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rectBatch.pv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); //buffer data wont be changed

	glBindVertexArray(rectBatch.vao.id);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)12);



	glGenBuffers(1, &rectBatch.pi_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rectBatch.pi_vbo);
	rectBatch.vao.instance_size = 80;

	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, rectBatch.vao.instance_size, (void*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, rectBatch.vao.instance_size, (void*)16);

	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, rectBatch.vao.instance_size, (void*)32);

	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, rectBatch.vao.instance_size, (void*)48);

	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, rectBatch.vao.instance_size, (void*)64);

	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, rectBatch.vao.instance_size, (void*)72);

	rectBatch.vao.attributeCount += 6;
	glBindVertexArray(0);


    program_construct(&rectBatch.program, "modules/render/shader/vertex.glsl", "modules/render/shader/fragment.glsl");
    ubo_construct(&rectBatch.camera_ubo, sizeof(float) * 16, GL_DYNAMIC_DRAW, rectBatch.program, "Camera", 0);
}


void render_task_terminate(void)
{
	glDeleteProgram(rectBatch.program);
	glDeleteBuffers(1, &rectBatch.pv_vbo);
	glDeleteBuffers(1, &rectBatch.pi_vbo);
	glDeleteBuffers(1, &rectBatch.ebo);
	glDeleteBuffers(1, &rectBatch.camera_ubo);
	glDeleteVertexArrays(1, &rectBatch.vao.id);
}


void render_rectBatch_image_set(Image* image)
{
	rectBatch.image = image;
}


static void ambientShader_setup(void)
{
	program_construct(&ambient_program, "modules/render/shader/ambient_vertex.glsl", "modules/render/shader/ambient_fragment.glsl");
}


static void ambientShader_render(void)
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(ambient_program);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


void render_rectBatch_camera_set(EntityId camera)
{
	rectBatch.camera = camera;
}
