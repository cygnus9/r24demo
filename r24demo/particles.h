#pragma once

#pragma once

#include "simple.h"
#include "transformations.h"

class Particles : public Simple {
private:
    static const int TSIZE = 4;

public:
	const char* vertex_code = R"sl(#version 300 es
            uniform mat4 modelview;
            uniform mat4 projection;
            uniform mat4 aspect;
            uniform mat4 rotation;

            uniform vec4 objcolor;
            uniform float time;
            uniform float lifetime;
            uniform vec3 autoFocus;

            uniform sampler2D positionTex;
            uniform sampler2D colorTex;

            in highp vec2 position;
            in highp vec2 texcoor;

            out highp vec4 v_color;
            out highp vec2 v_texcoor;
            void main()
            {
                highp vec4 postex = texelFetch(positionTex, ivec2(texcoor), 0);
                highp vec3 center = postex.xyz;

                highp vec4 color = vec4(texelFetch(colorTex, ivec2(texcoor), 0).rgb, 1.0);

                color = vec4(1.0, 1.0, 1.0, 1.0);

                highp vec4 projectedCenter = projection * aspect * modelview * vec4(center, 1.0);
                highp vec4 worldCenter = modelview * vec4(center, 1.0);
                highp vec4 worldAutofocus = vec4(autoFocus, 1.0);

                highp float scale = abs((worldCenter.z - worldAutofocus.z) * 0.01);
                scale = clamp(scale, 0.01, 0.08);

                gl_Position = (vec4(position, 0.0, 1.0) * scale * aspect + normalize(projectedCenter));
                highp float brightness = 0.001/pow(scale, 2.0);
                //brightness *= 0.001;
                highp float sparkle = (1.0 + sin((time - postex.w) * 10.0)) / 2.0;
                highp float fade = clamp(1.0 - ((time - postex.w) / lifetime), 0.0, 1.0);

                v_color =  objcolor * color * vec4(vec3(brightness * sparkle * fade), 1.0);

                v_texcoor = position;
            } 
		)sl";

	const char* shader_code = R"sl(#version 300 es
            in highp vec4 v_color;
            in highp vec2 v_texcoor;
            out highp vec4 f_color;

            void main()
            {
                f_color = vec4(v_color.rgb * v_color.a * clamp(pow((1.0 - length(v_texcoor)),0.5), 0.0, 1.0), 1.0);
            } 
		)sl";

	Particles(int tsize, GLuint positionTex, GLuint colorTex) : Simple() {
        identity((float(*)[4]) & m_modelview);
        identity((float(*)[4]) & m_projection);

        matrix4f i;
        identity((float(*)[4]) & i);
        scale((float(*)[4]) & i, (float(*)[4]) & m_aspect, 9.0 / 16.0, 1.0, 1.0);

        m_objcolor = vec4 { 1.0, 1.0, 1.0, 1.0 };
        m_autoFocus = vec3{ 0.0, 0.0, -10.0 };
        m_positionTex = positionTex;
        m_colorTex = colorTex;

		static const Geometry::attrib_defs vertex_defs[] = { {"position", 2}, {nullptr} };
		static const vec2 vertices[] = { {-1, -1}, {-1, 1}, {1, 1}, {1, -1} };

		GLuint vertex_buffer = Geometry::create_vertex_buffer((void*)vertices, sizeof(vertices));

		static const Geometry::attrib_defs instance_defs[] = { {"texcoor", 2}, {nullptr} };

#define MAXPARTICLES 65536
        vec2 instances[MAXPARTICLES];

        for (int i = 0; i < tsize; i++) {
            for (int j = 0; j < tsize; j++) {
                instances[i * tsize + j].x = i;
                instances[i * tsize + j].y = j;
            }
        }

		GLuint instance_buffer = Geometry::create_vertex_buffer((void*)instances, sizeof(instances));

		init(vertex_code, shader_code, vertex_buffer, ARRAYSIZE(vertices), vertex_defs,
			instance_buffer, tsize * tsize, instance_defs);
	}

    void uniforms(GLuint program) {
        Geometry::uniform(program, "modelview", m_modelview);
        Geometry::uniform(program, "projection", m_projection);
        Geometry::uniform(program, "aspect", m_aspect);

        Geometry::uniform(program, "objcolor", m_objcolor);
        Geometry::uniform(program, "time", (float)0.0);
        Geometry::uniform(program, "lifetime", (float)10.0);
        Geometry::uniform(program, "autoFocus", m_autoFocus);

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


};

