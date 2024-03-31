#pragma once

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
		glBlendFunc(m_srcBlend, m_dstBlend);
		err = glGetError();
		Geometry::use_vao(m_vao);
		err = glGetError();
		Geometry::configure_attribs(m_program, m_vertex_defs, m_vertex_buffer, 0);
		err = glGetError();
		Geometry::configure_attribs(m_program, m_instance_defs, m_instance_buffer, 1);
		err = glGetError();
		uniforms(m_program);
		Geometry::render(GL_TRIANGLE_STRIP, m_vertex_count, m_instance_count);
		err = glGetError();
		Geometry::use_program(0);
		err = glGetError();
	}

	virtual void uniforms(GLuint program) { }

	typedef struct {
		vec2 p;
		vec2 t;
	} POS_TEX;

	static const Geometry::attrib_defs quad_defs[];
	static const vec2 quad_vertices[];
	static const Geometry::attrib_defs texquad_defs[];
	static const POS_TEX texquad_vertices[];



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

protected:
	GLuint m_srcBlend = GL_SRC_ALPHA;
	GLuint m_dstBlend = GL_ONE_MINUS_SRC_ALPHA;
};

const Geometry::attrib_defs Simple::quad_defs[] = { {"position", 2}, { nullptr } };
const vec2 Simple::quad_vertices[4] = { {-1, 1}, {-1, -1}, {1, 1}, {1, -1} };

const Geometry::attrib_defs Simple::texquad_defs[] = { {"position", 2}, {"texcoor", 2 }, { nullptr } };
const Simple::POS_TEX Simple::texquad_vertices[] = { { {1, -1}, { 1, 0 } }, { { -1, -1 }, { 0, 0 } }, { {1, 1}, { 1, 1 } }, { {-1, 1}, { 0, 1 } } };