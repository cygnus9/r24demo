#pragma once

#include "simple.h"

class Quad : public Simple {
public:
	const char* vertex_code = R"sl(#version 300 es
			in highp float instance;
			in highp vec2 position;
			out highp vec4 v_color;
			void main()
			{
				gl_Position = vec4(position + vec2(instance, 0), 0.0, 1.0);
				v_color = vec4(position, 0.0, 1.0);
			} 
		)sl";

	const char* shader_code = R"sl(#version 300 es
			in highp vec4 v_color;
			out highp vec4 f_color;

			void main()
			{
				f_color = v_color;
			} 
		)sl";

	Quad() : Simple() {
		static const Geometry::attrib_defs vertex_defs[] = { {"position", 2}, {nullptr} };
		static const vec2 vertices[] = { {-1, 1}, {-1, -1}, {0, 1}, {0, -1} };

		GLuint vertex_buffer = Geometry::create_vertex_buffer((void*)vertices, sizeof(vertices));

		static const Geometry::attrib_defs instance_defs[] = { {"instance", 1}, {nullptr} };
		static const float instances[] = { 0, 1 };

		GLuint instance_buffer = Geometry::create_vertex_buffer((void*)instances, sizeof(instances));

		init(vertex_code, shader_code, vertex_buffer, ARRAYSIZE(vertices), vertex_defs,
			instance_buffer, ARRAYSIZE(instances), instance_defs);
	}
};

