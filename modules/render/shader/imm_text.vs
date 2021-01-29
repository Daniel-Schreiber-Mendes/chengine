#version 430


//per instance
in float scale;
in vec2 position;
in vec4 color;
in float currentFont;
in float stringSize; 
in float string[64];

out vec2 v_texCoord;
flat out vec4 f_color;
flat out float layer;


struct Character
{					//base alignment //aligned offset //total size 20
	vec2 size;		// 8				0
	vec2 bearing;   // 8				8
	uint advance; 	// 4				16			// Offset to advance to next glyph
};

//TODO: make all members const


struct Font
{								//base alignment  //aligned offset // total size 2568
	vec2 textureSize;			//8				    0
	Character characters[128]; // 20 * 128			8 + 20 * 128
};


layout (std140, binding = 0) uniform Camera
{
					//base alignment //aligned offset
	mat4 u_camera_vp; //64              0
};


layout (std140, binding = 0) buffer Fonts //total size 5144
{					//base alignment  //aligned offset
	Font fonts[2]; // 5136				0
	bool valids[2]; // 8					51236
};
//currently supports only two fonts loaded at the same time. changing fonts is very expensive

vec2 positions[4] = vec2[4]
(
	vec2(-0.5f, -0.5f),
	vec2(0.5f, -0.5f),
	vec2(0.5f, 0.5f),
	vec2(-0.5f, 0.5f)
);


vec2 texturePositions[4] = vec2[4]
(
	vec2(0, 0),
	vec2(1, 0),
	vec2(1, 1),
	vec2(0, 1)
);


void main()
{
	gl_Position = vec4(positions[gl_VertexID % 4], 0, 1) + vec4(1, 1, 1, 0); //* fonts[uint(currentFont)].characters[uint(string[gl_InstanceID])].size * vec2(scale, scale) + position + vec2(fonts[uint(currentFont)].characters[uint(string[gl_InstanceID])].bearing.x, 0), 0, 1);
	v_texCoord = texturePositions[gl_VertexID % 4] * fonts[uint(currentFont)].characters[uint(string[gl_InstanceID])].size / fonts[uint(currentFont)].textureSize;
	f_color = color;
	layer = string[gl_InstanceID];
	
}