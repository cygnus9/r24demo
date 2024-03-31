#pragma once

#include "simple.h"

class Accelerator : public Simple {
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
            uniform sampler2D velocityTex;
            uniform sampler2D positionTex;
            uniform highp float dt;
            uniform highp vec3 singularity;

            out highp vec4 f_color;
            in highp vec2 v_texcoor;
            uniform highp float gravity;
            uniform highp float drag;

            void main()
            {
                highp vec3 currentSpeed = textureLod(velocityTex, v_texcoor, 0.0).xyz;
                highp vec3 currentPos = textureLod(positionTex, v_texcoor, 0.0).xyz;
                highp vec3 dist = currentPos - singularity;
                f_color = vec4(currentSpeed * (1.0 - (drag * dt * length(currentSpeed))) - normalize(dist) * pow(length(dist), -2.0) * dt * gravity + vec3(0, -10, 0) * dt, 1.0);
            }
		)sl";

	Accelerator(GLuint positionTex) : Simple(), m_positionTex(positionTex) {
		static const Geometry::attrib_defs vertex_defs[] = { {"position", 2}, {"texcoor", 2 }, { nullptr } };
		static const struct {
			vec2 pos;
			vec2 texcoor;
		} vertices[] = { { {1, -1}, { 1, 0 } }, { { -1, -1 }, { 0, 0 } }, { {1, 1}, { 1, 1 } }, { {-1, 1}, { 0, 1 } } };

		GLuint vertex_buffer = Geometry::create_vertex_buffer((void*)vertices, sizeof(vertices));

		init(vertex_code, shader_code, vertex_buffer, ARRAYSIZE(vertices), vertex_defs,
			0, 1, nullptr);

		m_objcolor = vec4{ 1.0, 1.0, 1.0, 1.0 };
		m_gravity = 50;
		m_drag = 0.01;
		m_singularity = vec3{ 0.0, 0.0, 0.0 };

		m_srcBlend = GL_ONE;
		m_dstBlend = GL_ZERO;
	}

	void setVelocityTex(GLuint tex) {
		m_velocityTex = tex;
	}

	void setDT(float dt) {
		m_dt = dt;
	}

	void uniforms(GLuint program) {
		Geometry::texture(program, "velocityTex", 0, m_velocityTex);
		Geometry::texture(program, "positionTex", 1, m_positionTex);

		Geometry::uniform(program, "objcolor", m_objcolor);
		Geometry::uniform(program, "dt", m_dt);
		Geometry::uniform(program, "gravity", m_gravity);
		Geometry::uniform(program, "drag", m_drag);
		Geometry::uniform(program, "singularity", m_singularity);
	}

private:
	GLuint m_velocityTex;
	GLuint m_positionTex;
	vec3 m_singularity;
	float m_dt;
	float m_gravity;
	float m_drag;
	vec4 m_objcolor;
};

