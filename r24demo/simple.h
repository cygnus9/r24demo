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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		err = glGetError();
		Geometry::use_vao(m_vao);
		err = glGetError();
		Geometry::configure_attribs(m_program, m_vertex_defs, m_vertex_buffer, 0);
		err = glGetError();
		Geometry::configure_attribs(m_program, m_instance_defs, m_instance_buffer, 1);
		err = glGetError();
		uniforms(m_program);
		Geometry::render(GL_QUADS, m_vertex_count, m_instance_count);
		err = glGetError();
		Geometry::use_program(0);
		err = glGetError();
	}

	virtual void uniforms(GLuint program) { }

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

