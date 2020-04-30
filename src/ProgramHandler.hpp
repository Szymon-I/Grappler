#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "libs/texture_loader.hpp"
#include "libs/obj_loader.hpp"
#include "libs/shader_stuff.h"

#include "Light.hpp"
#include "Material.hpp"

using namespace std;
class ProgramHandler
{
private:
	// opengl masks
	GLuint program;
	GLuint vBuffer_coord;
	GLuint vBuffer_normal;
	GLuint vBuffer_uv;
	GLuint vArray;
	GLuint TextureID;

	// vectors for storing obj data
	vector<glm::vec3> OBJ_vertices;
	vector<glm::vec2> OBJ_uvs;
	vector<glm::vec3> OBJ_normals;

	// transform data
	glm::vec3 custom_translate;
	glm::vec3 custom_scale;
	glm::vec3 custom_rotate;
	GLfloat translation_animation = 0.0;
	GLfloat rotate_angle;
	GLfloat rotation_animation = 0.0;
	bool rotation_applied = false;
	bool texture_applied = false;

	Light global_light;
	vector<glm::vec3> global_material;

	// add shader path
	void assign_shaders(string vertex_shader, string fragment_shader)
	{
		glAttachShader(this->program, LoadShader(GL_VERTEX_SHADER, vertex_shader.c_str()));
		glAttachShader(this->program, LoadShader(GL_FRAGMENT_SHADER, fragment_shader.c_str()));
		LinkAndValidateProgram(this->program);
	}

	// load texture from png file into object
	void load_texture(string texture)
	{
		int tex_width;
		int tex_height;
		unsigned char *tex_data;

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		tex_data = SOIL_load_image(texture.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		SOIL_free_image_data(tex_data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// bind matrix to buffers
	void bind_buffers()
	{
		glGenVertexArrays(1, &vArray);
		glBindVertexArray(vArray);

		glGenBuffers(1, &vBuffer_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_coord);
		glBufferData(GL_ARRAY_BUFFER, OBJ_vertices.size() * sizeof(glm::vec3), &OBJ_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		// Wektory normalne
		glGenBuffers(1, &vBuffer_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_normal);
		glBufferData(GL_ARRAY_BUFFER, OBJ_normals.size() * sizeof(glm::vec3), &OBJ_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// Wspolrzedne textury UV
		glGenBuffers(1, &vBuffer_uv);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_uv);
		glBufferData(GL_ARRAY_BUFFER, OBJ_uvs.size() * sizeof(glm::vec2), &OBJ_uvs[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// pass matrix to shader
	void uniform_matrix_send(glm::mat4x4 Matrix_proj_mv)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "Matrix_proj_mv"), 1, GL_FALSE, glm::value_ptr(Matrix_proj_mv));

		int size = global_light.get_position().size();
		glUniform1i(glGetUniformLocation(program, "Number_Of_Lights"), size);
		glUniform3fv(glGetUniformLocation(program, "Light_Ambient"), size, &(global_light.get_ambient()[0])[0]);
		glUniform3fv(glGetUniformLocation(program, "Light_Diffuse"), size, &(global_light.get_diffuse()[0])[0]);
		glUniform3fv(glGetUniformLocation(program, "Light_Position"), size, &(global_light.get_position()[0])[0]);

		glUniform3fv( glGetUniformLocation(program, "myMaterial.Ambient"), 1, &(global_material[0])[0]);
		glUniform3fv( glGetUniformLocation(program, "myMaterial.Diffuse"), 1, &(global_material[1])[0]);
		glUniform3fv( glGetUniformLocation(program, "myMaterial.Specular"), 1, &(global_material[2])[0]);
		glUniform1f( glGetUniformLocation(program, "myMaterial.Shininess"), global_material[3][0]);
	}

	// reset all mods for object
	void reset_mod()
	{
		this->custom_translate = glm::vec3(0.0f, 0.0f, 0.0f);
		this->custom_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		this->custom_rotate = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotate_angle = 0.0f;
		this->rotation_applied = false;
	}

	// display final matrix
	void display_util(glm::mat4x4 &Matrix_proj_mv)
	{
		global_light.move_light(1);

		glBindVertexArray(vArray);
		glUseProgram(this->program);

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, TextureID);
		glUniform1i(glGetUniformLocation(program, "tex0"), 0);
		uniform_matrix_send(Matrix_proj_mv);

		glDrawArrays(GL_TRIANGLES, 0, OBJ_vertices.size());
	}

	// apply all modifications (scale/rotate/translate)
	glm::mat4x4 apply_mods(glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv)
	{
		Matrix_mv = glm::translate(Matrix_mv, this->custom_translate);
		Matrix_mv = glm::scale(Matrix_mv, this->custom_scale);
		if (this->rotation_applied)
			Matrix_mv = glm::rotate(Matrix_mv, this->rotate_angle, this->custom_rotate);
		return Matrix_proj * Matrix_mv;
	}

public:
	// create program
	void init(string obj_path, string vertex_shader, string fragment_shader, string texture, Light light, std::vector <glm::vec3> material)
	{
		this->program = glCreateProgram();
		loadOBJ(obj_path.c_str(), this->OBJ_vertices, this->OBJ_uvs, this->OBJ_normals);
		assign_shaders(vertex_shader, fragment_shader);
		load_texture(texture);
		this->global_light = light;
		this->global_material = material;
		reset_mod();
		bind_buffers();
	}

	void set_translation_animation(GLfloat translation_animation)
	{
		this->translation_animation += translation_animation;
	}

	// translate object
	void set_translate(glm::vec3 translate)
	{
		this->custom_translate = translate;
		if (translation_animation < -0.01 || translation_animation > 0.01)
		{
			GLfloat r = sqrt(custom_translate.x*custom_translate.x+custom_translate.z*custom_translate.z);
			this->custom_translate.x = cos(this->translation_animation) * r;
			this->custom_translate.z = sin(this->translation_animation) * r;
		}
	}

	// scale object
	void set_scale(glm::vec3 scale)
	{
		this->custom_scale = scale;
	}

	// animate rotation
	void set_rotation_animation(GLfloat rotation_animation)
	{
		this->rotation_animation += rotation_animation;
	}

	// rotate object
	void set_rotation(glm::vec3 rotation_matrix, GLfloat rotation)
	{
		this->custom_rotate = rotation_matrix;
		this->rotate_angle = rotation + this->rotation_animation;
		this->rotation_applied = true;
	}

	// display function with mods
	void display(glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv)
	{
		glm::mat4x4 Matrix_proj_mv = apply_mods(Matrix_proj, Matrix_mv);
		display_util(Matrix_proj_mv);
	}

	void clean(void)
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vBuffer_coord);
		glDeleteBuffers(1, &vBuffer_uv);
		glDeleteBuffers(1, &vBuffer_normal);
		glDeleteVertexArrays(1, &vArray);
	}
};
