/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:52:39 by gperez            #+#    #+#             */
/*   Updated: 2020/08/11 23:27:31 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

using namespace std;

Engine::Engine()
{
	this->sky = false;
}

static void	framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

int			Engine::initWindow(void)
{
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return (-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	Engine::window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
	if (Engine::window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return (-1);
	}
	glfwMakeContextCurrent(Engine::window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return (-1);
	}
	glfwSetFramebufferSizeCallback(Engine::window, framebuffer_size_callback);
	glfwSwapInterval(1);
	return (0);
}

GLFWwindow	*Engine::getWindow(void)
{
	return (Engine::window);
}

static void	fillTempVbo(vector<vbo_type> &tempVbo, t_direction_consts dir_c)
{
	int			iPt;
	vbo_type	vboType;

	for (iPt = 0; iPt < 6; iPt++)
	{
		vboType.tab[0] = dir_c.pts[iPt].get(X);
		vboType.tab[1] = dir_c.pts[iPt].get(Y);
		vboType.tab[2] = dir_c.pts[iPt].get(Z);
		vboType.meta = dir_c.axis < 0 ? dir_c.axis + 7 : dir_c.axis;
		tempVbo.push_back(vboType);
	}
}

void		Engine::genSkybox(void)
{
	vector<vbo_type>	tempVbo;

	if (sky || this->shaderSky.loadShader((char*)VERTEX_SKY, (char*)FRAGMENT))
		return;
	for (int i = 0; i < 6; i++)
	{
		fillTempVbo(tempVbo, g_dir_c[i]);
	}
	glGenVertexArrays(1, &this->vaoSky);
	glBindVertexArray(this->vaoSky);
	glGenBuffers(1, &this->vboSky);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboSky);
	glBufferData(GL_ARRAY_BUFFER, tempVbo.size() * sizeof(vbo_type), &tempVbo[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	this->sky = true;
}

bool		Engine::isSkybox(void)
{
	return (this->sky);
}

Shader&		Engine::getShaderSky(void)
{
	return (this->shaderSky);
}

void		Engine::displaySky(Textures *t)
{
	Shader	&shader = this->shaderSky;

	glDepthMask(false);
	glBindVertexArray(this->vaoSky);
	glUseProgram(shader.getProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
		"view"), 1, GL_FALSE, glm::value_ptr(this->getCam().getMatrix(true)));
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
		"projection"), 1, GL_FALSE, glm::value_ptr(this->getCam().getProjMatrix()));
	glUniform1i(glGetUniformLocation(shader.getProgram(),
		"basicTexture"), t ? t->getTxt() : 0);
	glDrawArrays(GL_TRIANGLES, 0, NB_TRIANGLES_CUBE);
	glDepthMask(true);
}

Camera&		Engine::getCam(void)
{
	return (Engine::camera);
}
void		Engine::setCam(Camera cam)
{
	Engine::camera = cam;
}

Shader&		Engine::getShader(void)
{
	return (Engine::shader);
}

void 		Engine::fillTextureVector(size_t start, size_t end, bool load)
{
	size_t	i = start;
	string	str;
	size_t	len;

	while (i < end && (!load || textures.size() < 16))
	{
		len = ft_strlen(g_txt_path[i].path_txt);
		if (len && load)
		{
			str.assign(g_txt_path[i].path_txt, len);
			this->addTexture((char*)g_txt_path[i].path_txt, 0,
			str.find(".png") == (len - 4) ? true : false);
		}
		else if (!load)
			this->textures.push_back(new Textures((char*)g_txt_path[i].path_txt));
		str.clear();
		i++;
	}
}

void	Engine::genBlocksTextures(void)
{
	ContextOpenCL	cl;

	this->fillTextureVector(0, END_BLOCK_T, false);

	// Recuperer les datas de chaques txt et en faire une et une seule // A FAIRE AVEC CL

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];
	this->textures.clear();

	// Rajouter la texture generer avec openCL dans le vector textures

}

void		Engine::genTextures(void)
{
	this->genBlocksTextures();
	this->fillTextureVector(END_BLOCK_T + 1, END_T, true);
}

Textures	*Engine::getTexture(unsigned int t)
{
	if (t < this->textures.size())
		return (this->textures[t]);
	return (NULL);
}

Engine::~Engine()
{
	int	i;

	i = 0;
	while ((unsigned int)i < this->textures.size())
	{
		delete this->textures[i];
		i++;
	}
}

///////////////Private///////////////

void	Engine::addTexture(char *pathOrBuffer, size_t len, bool alpha)
{
	unsigned int	textIdx;
	Textures		*t;

	(void)len;
	// if (len == 0)
		this->textures.push_back(new Textures(pathOrBuffer));
	// else
	// 	this->textures.push_back(new Textures(pathOrBuffer, len));
	glGenTextures(1, &textIdx);
	t = this->textures[this->textures.size() - 1];
	t->setTxt(textIdx);
	glActiveTexture(GL_TEXTURE0 + this->textures.size());
	glBindTexture(GL_TEXTURE_2D, t->getTxt());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (alpha)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->getWidth(), t->getHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, t->getTxtData());
	}
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->getWidth(), t->getHeight(), 0,
			GL_RGB, GL_UNSIGNED_BYTE, t->getTxtData());
	glGenerateMipmap(GL_TEXTURE_2D);
}