#include "types.h"
#include "geometry.h"

class Simple {
public:
	void init(const char* vertex_code, const char* shader_code, 
			GLuint vertex_buffer, int vertex_count, const Geometry::attrib_defs* vertex_defs,
			GLuint instance_buffer, int instance_count, const Geometry::attrib_defs* instance_defs) 
	{
		m_vertex_buffer = vertex_buffer;
		m_vertex_count = vertex_count;
		m_vertex_defs = vertex_defs;
		m_instance_buffer = instance_buffer;
		m_instance_count = instance_count;
		m_instance_defs = instance_defs;

		m_program = Geometry::compile_program(vertex_code, shader_code);
		m_vao = Geometry::create_vao();
	}

	void render() {
		GLuint err = glGetError();

		Geometry::use_program(m_program);
		err = glGetError();
		glBlendFunc(GL_ONE, GL_ONE);
		err = glGetError();
		Geometry::use_vao(m_vao);
		err = glGetError();
		Geometry::configure_attribs(m_program, m_vertex_defs, m_vertex_buffer, 0);
		err = glGetError();
		Geometry::configure_attribs(m_program, m_instance_defs, m_instance_buffer, 1);
		err = glGetError();
		Geometry::render(GL_QUADS, m_vertex_count, m_instance_count);
		err = glGetError();
		Geometry::use_program(0);
		err = glGetError();
	}

private:
	const char* m_vertex_code;
	const char* m_shader_code;

	GLuint m_vertex_buffer;
	size_t m_vertex_count;
	const Geometry::attrib_defs* m_vertex_defs;

	GLuint m_instance_buffer;
	size_t m_instance_count;
	const Geometry::attrib_defs* m_instance_defs;

	GLuint m_program;
	GLuint m_vao;
	matrix4f m_model_view;
};

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
		static const vec2 vertices[] = { {-1, -1}, {-1, 1}, {0, 1}, {0, -1} };

		GLuint vertex_buffer = Geometry::create_vertex_buffer((void*)vertices, sizeof(vertices));

		static const Geometry::attrib_defs instance_defs[] = { {"instance", 1}, {nullptr} };
		static const float instances[] = { 0, 1 };

		GLuint instance_buffer = Geometry::create_vertex_buffer((void*)instances, sizeof(instances));

		init(vertex_code, shader_code, vertex_buffer, ARRAYSIZE(vertices), vertex_defs,
										instance_buffer, ARRAYSIZE(instances), instance_defs);
	}
};

