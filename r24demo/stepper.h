#pragma once

#include "simple.h"

class Stepper : public Simple {
public:
	const char* vertex_code = R"sl(#version 300 es
			uniform vec4 objcolor;

			in highp vec2 position;
			in highp vec2 texcoor;
			out highp vec4 v_color;
			out highp vec2 v_texcoor;
			void main()
			{
				gl_Position = vec4(position, 0.0, 1.0);
				v_color = objcolor;
				v_texcoor = texcoor;
			} 
		)sl";

	const char* shader_code = R"sl(#version 300 es
			uniform sampler2D tex;
			out highp vec4 f_color;
			in highp vec2 v_texcoor;
			in highp vec4 v_color;
        
			void main()
			{
				f_color = textureLod(tex, v_texcoor, 0.0) * v_color;
			}		)sl";

	Stepper() : Simple() {
		static const Geometry::attrib_defs vertex_defs[] = { {"position", 2}, {"texcoor", 2 }, { nullptr } };
		static const struct {
			vec2 pos;
			vec2 texcoor;
		} vertices[] = { { {-1, -1}, { 0, 0 } }, { { 1, -1 }, { 1, 0 } }, { {1, 1}, { 1, 1 } }, { {-1, 1}, { 0, 1 } } };

		GLuint vertex_buffer = Geometry::create_vertex_buffer((void*)vertices, sizeof(vertices));

		init(vertex_code, shader_code, vertex_buffer, ARRAYSIZE(vertices), vertex_defs,
			0, 1, nullptr);

		m_objcolor = vec4{ 1.0, 1.0, 1.0, 1.0 };

		m_srcBlend = GL_SRC_ALPHA;
		m_dstBlend = GL_ONE;
	}

	void setVelocityTex(GLuint tex) {
		m_tex = tex;
	}

	void setColor(vec4 color) {
		m_objcolor = color;
	}

	void uniforms(GLuint program) {
		Geometry::texture(program, "tex", 0, m_tex);
		Geometry::uniform(program, "objcolor", m_objcolor);
	}

private:
	GLuint m_tex;
	vec4 m_objcolor;
};

