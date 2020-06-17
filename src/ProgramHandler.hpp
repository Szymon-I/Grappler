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
#include "Camera.hpp"
#define FLOAT_THRESHOLD 0.005

extern Camera camera;

using namespace std;
class ProgramHandler
{
private:
	int _id;
	// opengl masks
	GLuint program;
	GLuint vBuffer_coord;
	GLuint vBuffer_normal;
	GLuint vBuffer_uv;
	GLuint vArray;
	GLuint TextureID;
	GLuint TextureID2;
	bool second_texture_applied = false;

	// vectors for storing obj data
	vector<glm::vec3> OBJ_vertices;
	vector<glm::vec2> OBJ_uvs;
	vector<glm::vec3> OBJ_normals;

	// transform data
	glm::vec3 custom_translate;
	glm::vec3 custom_scale;
	glm::vec3 custom_rotate;
	GLfloat translate_factor = 0.0;
	GLfloat translation_animation = 0.0;
	GLfloat rotate_angle;
	GLfloat rotation_animation = 0.0;
	bool rotation_applied = false;
	bool texture_applied = false;

	// global light and material
	Light global_light;
	vector<glm::vec3> global_material;

	// data passed into initialize
	string vertex_shader_path;
	string fragment_shader_path;
	string obj_path;
	string texture;

	// shadows
	GLuint DepthMap_Program;
	GLuint DepthMap_FrameBuffer;
	GLuint DepthMap_Texture;
	unsigned int DepthMap_Width = 2024 * 2, DepthMap_Height = 2024 * 2;
	glm::vec3 Light_Direction = glm::normalize(glm::vec3(0.2, -0.8f, 1.1f));
	glm::vec3 Light_Position = glm::vec3(0.0f, 10.0f, -5.0f);
	glm::mat4 lightProj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 30.5f);
	glm::mat4 lightView = glm::lookAt(Light_Position,
									  Light_Position + Light_Direction,
									  glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 radius_vector;
	bool collidable;
	// add shaders paths
	void assign_shaders(string vertex_shader, string fragment_shader)
	{
		glAttachShader(this->program, LoadShader(GL_VERTEX_SHADER, vertex_shader.c_str()));
		glAttachShader(this->program, LoadShader(GL_FRAGMENT_SHADER, fragment_shader.c_str()));
		LinkAndValidateProgram(this->program);
	}

	// load texture from png file into object
	void load_texture(string texture, bool second_tex = false)
	{
		int tex_width;
		int tex_height;
		unsigned char *tex_data;

		if (second_tex)
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &TextureID2);
			glBindTexture(GL_TEXTURE_2D, TextureID2);
			this->second_texture_applied = true;
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
		}

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

		// object verticles
		glGenBuffers(1, &vBuffer_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_coord);
		glBufferData(GL_ARRAY_BUFFER, OBJ_vertices.size() * sizeof(glm::vec3), &OBJ_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		// object normals
		glGenBuffers(1, &vBuffer_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_normal);
		glBufferData(GL_ARRAY_BUFFER, OBJ_normals.size() * sizeof(glm::vec3), &OBJ_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// object uvs
		glGenBuffers(1, &vBuffer_uv);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_uv);
		glBufferData(GL_ARRAY_BUFFER, OBJ_uvs.size() * sizeof(glm::vec2), &OBJ_uvs[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		// flags for options
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void shadow_init(void)
	{
		glGenTextures(1, &DepthMap_Texture);
		glBindTexture(GL_TEXTURE_2D, DepthMap_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DepthMap_Width, DepthMap_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// 2. Frame buffer object
		glGenFramebuffers(1, &DepthMap_FrameBuffer);

		// 3. Attaching texture to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FrameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap_Texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 4. OpenGL program for shadow map
		// Tworzenie potoku OpenGL
		DepthMap_Program = glCreateProgram();
		glAttachShader(DepthMap_Program, LoadShader(GL_VERTEX_SHADER, "shaders/depthmapvertex.glsl"));
		glAttachShader(DepthMap_Program, LoadShader(GL_FRAGMENT_SHADER, "shaders/depthmapfragment.glsl"));
		LinkAndValidateProgram(DepthMap_Program);
	}

	// pass unform data to shader
	void uniform_matrix_send(glm::mat4x4 Matrix_proj_mv)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "Matrix_proj_mv"), 1, GL_FALSE, glm::value_ptr(Matrix_proj_mv));

		int size = global_light.get_position().size();
		glUniform1i(glGetUniformLocation(program, "Number_Of_Lights"), size);
		glUniform3fv(glGetUniformLocation(program, "Light_Ambient"), size, &(global_light.get_ambient()[0])[0]);
		glUniform3fv(glGetUniformLocation(program, "Light_Diffuse"), size, &(global_light.get_diffuse()[0])[0]);
		glUniform3fv(glGetUniformLocation(program, "Light_Position"), size, &(global_light.get_position()[0])[0]);

		glUniform3fv(glGetUniformLocation(program, "myMaterial.Ambient"), 1, &(global_material[0])[0]);
		glUniform3fv(glGetUniformLocation(program, "myMaterial.Diffuse"), 1, &(global_material[1])[0]);
		glUniform3fv(glGetUniformLocation(program, "myMaterial.Specular"), 1, &(global_material[2])[0]);
		glUniform1f(glGetUniformLocation(program, "myMaterial.Shininess"), global_material[3][0]);

		glUniformMatrix4fv(glGetUniformLocation(program, "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
		glUniformMatrix4fv(glGetUniformLocation(program, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));

		glm::vec3 Camera_Position = camera.GetCameraPos();
		glUniform3fv(glGetUniformLocation(program, "Camera_Position"), 1, &Camera_Position[0]);
		glUniform3fv(glGetUniformLocation(program, "Light_Direction"), 1, glm::value_ptr(Light_Direction));
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

	void animation_util(void)
	{
		this->rotate_angle += this->rotation_animation;
		if (translation_animation < -FLOAT_THRESHOLD || translation_animation > FLOAT_THRESHOLD)
		{
			this->translate_factor += this->translation_animation;
			GLfloat r = sqrt(custom_translate.x * custom_translate.x + custom_translate.z * custom_translate.z);
			this->custom_translate.x = cos(this->translate_factor) * r;
			this->custom_translate.z = sin(this->translate_factor) * r;
		}
	}
	// shadows util
	void DrawShadowMap()
	{
		glViewport(0, 0, DepthMap_Width, DepthMap_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		// AKTYWUJEMY program
		glUseProgram(DepthMap_Program);

		glUniformMatrix4fv(glGetUniformLocation(DepthMap_Program, "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
		glUniformMatrix4fv(glGetUniformLocation(DepthMap_Program, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));

		glBindVertexArray(vArray);
		glDrawArrays(GL_TRIANGLES, 0, OBJ_vertices.size());
		glBindVertexArray(0);

		// WYLACZAMY program
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	// display final matrix
	void display_util(glm::mat4x4 &Matrix_proj_mv, bool second_texture = false)
	{
		animation_util();
		global_light.move_light(1);

		DrawShadowMap();

		glUseProgram(this->program);

		uniform_matrix_send(Matrix_proj_mv);

		if (second_texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID2);
			glUniform1i(glGetUniformLocation(program, "tex0"), 0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glUniform1i(glGetUniformLocation(program, "tex0"), 0);
		}
		// Shadow map texture slot 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, DepthMap_Texture);
		glUniform1i(glGetUniformLocation(program, "shadowMap"), 1);

		glBindVertexArray(vArray);
		glDrawArrays(GL_TRIANGLES, 0, OBJ_vertices.size());
		glBindVertexArray(0);
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

	glm::vec3 get_colision_vector()
	{
		float radius = 0.0f;
		glm::vec3 radius_vector;
		for (int i = 0; i < OBJ_vertices.size(); i++)
		{
			float len = glm::length(OBJ_vertices[i]);
			if (len > radius)
			{
				radius_vector = OBJ_vertices[i];
				radius = len;
			}
		}
		return radius_vector;
	}

public:
	ProgramHandler()
	{
		static int id = 0;
		_id = id++;
	}
	// create program with given parameters
	void init(string obj_path, string vertex_shader, string fragment_shader, string texture, Light light, std::vector<glm::vec3> material, bool collidable = true, bool reset_mods = true, bool reload_obj = true)
	{
		this->program = glCreateProgram();
		this->obj_path = obj_path;
		this->texture = texture;
		this->global_light = light;
		this->global_material = material;
		this->vertex_shader_path = vertex_shader;
		this->fragment_shader_path = fragment_shader;
		this->collidable = collidable;
		assign_shaders(vertex_shader, fragment_shader);
		load_texture(texture);
		// if object reload is needed
		if (reload_obj)
		{
			loadOBJ(obj_path.c_str(), this->OBJ_vertices, this->OBJ_uvs, this->OBJ_normals);
			this->radius_vector = get_colision_vector();
			//printf("r=%f\n", collision_radius);
			printf("id=%d\n", _id);
		}
		// if reset modifications is needed
		if (reset_mods)
		{
			reset_mod();
		}
		// bind obj data to buffers used in shaders
		bind_buffers();

		shadow_init();
	}

	// change fragment shader for program
	void update_fragment_shader(string fragment_shader_path)
	{
		clean();
		init(this->obj_path, this->vertex_shader_path, fragment_shader_path, this->texture, this->global_light, this->global_material, false, false);
	}
	// set translate for object
	void set_translate(glm::vec3 translate)
	{
		this->custom_translate = translate;
	}
	// set object scale
	void set_scale(glm::vec3 scale)
	{
		this->custom_scale = scale;
	}
	glm::vec3 get_scale()
	{
		return this->custom_scale;
	}
	// set object rotation
	void set_rotation(glm::vec3 rotation_matrix, GLfloat rotation)
	{
		this->custom_rotate = rotation_matrix;
		this->rotation_applied = true;
	}
	// set translation animation for object
	void set_translation_animation(GLfloat translation_animation)
	{
		this->translation_animation += translation_animation;
	}
	// set rotation animation for object
	void set_rotation_animation(GLfloat rotation_animation)
	{
		this->rotation_animation += rotation_animation;
	}
	// get object translate
	glm::vec3 get_translate()
	{
		return this->custom_translate;
	}
	// display function with mods
	void display(glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv, bool second_texture = false)
	{
		glm::mat4x4 Matrix_proj_mv = apply_mods(Matrix_proj, Matrix_mv);
		if (second_texture_applied && second_texture)
		{
			display_util(Matrix_proj_mv, true);
		}
		else
		{
			display_util(Matrix_proj_mv, false);
		}
	}
	// clean all allocated data for program
	void clean(void)
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vBuffer_coord);
		glDeleteBuffers(1, &vBuffer_uv);
		glDeleteBuffers(1, &vBuffer_normal);
		glDeleteVertexArrays(1, &vArray);
	}
	int get_id()
	{
		return this->_id;
	}
	float get_collision_radius()
	{
		return glm::length(this->radius_vector * get_scale());
	}
	bool get_collidable()
	{
		return this->collidable;
	}
	void load_second_tex(string name)
	{
		this->load_texture(name, true);
	}
};
