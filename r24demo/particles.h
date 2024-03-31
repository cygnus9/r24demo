#pragma once

#pragma once

#include "simple.h"
#include "transformations.h"

#define MAXPARTICLES 1048576
vec2 instances[MAXPARTICLES];

class Particles : public Simple {
public:
	const char* vertex_code = R"sl(#version 430
            layout(location = 0) uniform mat4 modelview;
            layout(location = 1) uniform mat4 projection;
            layout(location = 2) uniform mat4 aspect;

            layout(location = 3) uniform vec4 objcolor;
            layout(location = 4) uniform float time;
            layout(location = 5) uniform float lifetime;
            layout(location = 6) uniform vec3 autoFocus;

            layout(location = 7) uniform sampler2D positionTex;
            layout(location = 8) uniform sampler2D colorTex;

            in highp vec2 position;
            in highp vec2 texcoor;

            out highp vec4 v_color;
            out highp vec2 v_texcoor;
            void main()
            {
                highp vec4 postex = texelFetch(positionTex, ivec2(texcoor), 0);
                highp vec4 colortex = texelFetch(colorTex, ivec2(texcoor), 0);
                highp float birth = colortex.w;
                highp vec3 center = postex.xyz;

                highp vec4 color = vec4(colortex.rgb, 1.0);

                highp vec4 projectedCenter = projection * aspect * modelview * vec4(center, 1.0);
                highp vec4 worldCenter = modelview * vec4(center, 1.0);
                highp vec4 worldAutofocus = vec4(autoFocus, 1.0);

                highp float scale = abs((worldCenter.z - worldAutofocus.z) * 0.002);
                scale = clamp(scale, 0.0025, 0.8);

                gl_Position = (vec4(position, 0.0, 1.0) * scale * aspect + normalize(projectedCenter));
                highp float brightness = 0.0001/pow(scale, 2.0);
                highp float sparkle = (1.0 + sin((time - birth) * 10.0)) / 2.0;
                highp float fade = clamp(1.0 - ((time - birth) / lifetime), 0.0, 1.0);

                v_color =  objcolor * color * vec4(vec3(brightness * sparkle * fade), 1.0);

                v_texcoor = position;
            } 
		)sl";

	const char* shader_code = R"sl(#version 430
            in highp vec4 v_color;
            in highp vec2 v_texcoor;
            out highp vec4 f_color;

            void main()
            {
                f_color = vec4(v_color.rgb * v_color.a * clamp(pow((1.0 - length(v_texcoor)),0.5), 0.0, 1.0), 1.0);
            } 
		)sl";

	Particles(int tsize, GLuint positionTex, GLuint colorTex) : Simple() {
        identity(m_modelview);
        identity(m_projection);

        matrix4f i;
        identity(i);
        scale(i, m_aspect, 9.0 / 16.0, 1.0, 1.0);

        m_objcolor = vec4 { 1.0, 1.0, 1.0, 1.0 };
        m_autoFocus = vec3{ 0.0, 0.0, -30.0 };
        m_positionTex = positionTex;
        m_colorTex = colorTex;

        m_srcBlend = GL_SRC_ALPHA;
        m_dstBlend = GL_ONE;

		GLuint vertex_buffer = Geometry::create_vertex_buffer((void*)Simple::quad_vertices, sizeof(Simple::quad_vertices));

		static const Geometry::attrib_defs instance_defs[] = { {"texcoor", 2}, {nullptr} };

        for (int i = 0; i < tsize; i++) {
            for (int j = 0; j < tsize; j++) {
                instances[i * tsize + j].x = float(i);
                instances[i * tsize + j].y = float(j);
            }
        }

		GLuint instance_buffer = Geometry::create_vertex_buffer((void*)instances, sizeof(instances));

		init(vertex_code, shader_code, vertex_buffer, ARRAYSIZE(Simple::quad_vertices), Simple::quad_defs,
			instance_buffer, tsize * tsize, instance_defs);
	}

    void setTime(float t) {
        m_t = t;
    }

    void uniforms(GLuint program) {
        glUniformMatrix4fv(0, 1, false, &m_modelview.m[0].x);
        glUniformMatrix4fv(1, 1, false, &m_projection.m[0].x);
        glUniformMatrix4fv(2, 1, false, &m_aspect.m[0].x);

        glUniform4fv(3, 1, &m_objcolor.x);
        glUniform1fv(4, 1, &m_t);
        static float lifetime = 10.0;
        glUniform1fv(5, 1, &lifetime);
        glUniform3fv(6, 1, &m_autoFocus.x);

        Geometry::texture(program, "positionTex", 0, m_positionTex);
        Geometry::texture(program, "colorTex", 1, m_colorTex);
    }

public:
    matrix4f m_modelview;
    matrix4f m_projection;
    matrix4f m_aspect;

    vec4 m_objcolor;
    vec3 m_autoFocus;

    GLuint m_positionTex;
    GLuint m_colorTex;

    float m_t;
};

