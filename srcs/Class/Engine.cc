/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:52:39 by gperez            #+#    #+#             */
/*   Updated: 2021/12/01 23:47:49 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

using namespace std;
extern t_txt_path g_txt_path[];


Engine::Engine()
{
	this->isCursor = false;
	this->sky = false;
	this->firstMouse = true;
	this->lockRay = false;
	this->speed20 = false;
	for (unsigned int i = 0; i < GLFW_KEY_LAST; i++)
		this->keys[i] = KEY_RELEASE;
}

void				Engine::inputKey(unsigned int key)
{
	if (glfwGetKey(this->window, key) == GLFW_PRESS
		&& this->keys[key] == KEY_RELEASE)
	{
		this->keys[key] = KEY_PRESS;
		this->queue.push(key);
	}
	else if (glfwGetKey(this->window, key) == GLFW_RELEASE
		&& this->keys[key] == KEY_DONE)
			this->keys[key] = KEY_RELEASE;
}

void				Engine::getKeys(float deltaFrameTime)
{
	float speed = SPEED;

	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);
	if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !this->speed20)
		speed = SPEED_SPRINT;
	else if (this->speed20)
		speed = SPEED_ACCEL;

	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
		this->camera.translate(E_FRONT, speed * deltaFrameTime);
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
		this->camera.translate(E_FRONT, -speed * deltaFrameTime);
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
		this->camera.translate(E_RIGHT, speed * deltaFrameTime);
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
		this->camera.translate(E_RIGHT, -speed * deltaFrameTime);
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->camera.translate(E_UP, speed * deltaFrameTime);
	if (glfwGetKey(this->window, GLFW_KEY_X) == GLFW_PRESS)
		this->camera.translate(E_UP, -speed * deltaFrameTime);

	this->inputKey(GLFW_KEY_APOSTROPHE);
	this->inputKey(GLFW_KEY_MINUS);
	this->inputKey(GLFW_KEY_EQUAL);
	this->inputKey(GLFW_KEY_C);
}

int					Engine::getWidth(void)
{
	return this->monitorWidth;
}

int					Engine::getHeight(void)
{
	return this->monitorHeight;
}

void				mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Engine		*engine = (Engine*)glfwGetWindowUserPointer(window);
	glm::vec2	offsetMouse;
	glm::vec2	lastMousePos;

	if (!engine)
		return ;
	if (engine->isFirst())
	{
		engine->setMouseLastPos(glm::vec2(xpos, ypos));
		engine->setFirst(false);
	}
	lastMousePos = engine->getMouseLastPos();
	offsetMouse.y = xpos - lastMousePos.x;
	offsetMouse.x = ypos - lastMousePos.y;
	engine->setMouseLastPos(glm::vec2(xpos, ypos));
	offsetMouse *= SENSITIVITY;
	engine->getCam().rotate(glm::vec3(offsetMouse.x, offsetMouse.y, 0.0));
}

void				Engine::checkKeys(World &world)
{
	short	i;

	while (this->queue.size())
	{
		i = this->queue.front();
		if (i == GLFW_KEY_APOSTROPHE)
		{
			this->isCursor = !this->isCursor;
			glfwSetInputMode(window, GLFW_CURSOR, this->isCursor
				? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(this->window, this->isCursor
				? NULL : mouse_callback);
		}
		else if (i == GLFW_KEY_C)
			this->speed20 = !this->speed20;
		else if (i == GLFW_KEY_MINUS)
			world.decreaseDist();
		else if (i == GLFW_KEY_EQUAL)
			world.increaseDist();
		this->keys[(int)i] = KEY_DONE;
		this->queue.pop();
	}
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
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_TRUE);
	this->monitorWidth = mode->width;
	this->monitorHeight = mode->height;
	this->window = glfwCreateWindow(mode->width, mode->height, "ft_vox", monitor, NULL);
	if (this->window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return (-1);
	}
	glfwMakeContextCurrent(this->window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		glfwDestroyWindow(this->window);
		glfwTerminate();
		return (-1);
	}
	glfwSetCursorPosCallback(this->window, mouse_callback);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwSwapInterval(VSYNC_OFF);
	return (0);
}

GLFWwindow	*Engine::getWindow(void)
{
	return (Engine::window);
}

static bool	switchedBlock(int startPos[3], glm::vec3 pos)
{
	return (!(startPos[0] == (int)pos.x
		&& startPos[1] == (int)pos.y
		&& startPos[2] == (int)pos.z));
}

static void	stepLoop(glm::vec3 &pos, glm::vec3 ray)
{
	int			startPos[3] = {(int)pos.x, (int)pos.y, (int)pos.z};
	float		step = 0.0001;

	do
	{
		pos += ray * step;
	}	while (!switchedBlock(startPos, pos));
}

static bool isInAirBlock(Block currentBlock)
{
	return (currentBlock.getInfo().id == AIR);
}

Block		*Engine::getBlockFromPos(Chunk **chunk, glm::vec3 pos, glm::vec4 &bP, World &world)
{
	Block		*block;
	ChunkPos	chunkPos = Camera::getCurrentChunkPos(pos);

	(*chunk) = world.getUnsafe(chunkPos);
	if (!chunk)
		return (NULL);
	bP = glm::vec4(Camera::getCurrentOffset(pos), (int)(pos.y / 16.));
	if (bP.x - PREC < 0.0)
		bP.x = 1.0 + bP.x;
	if (bP.z - PREC < 0.0)
		bP.z = 1.0 + bP.z;

	bP.x *= 16.0;
	bP.y *= 16.0;
	bP.z *= 16.0;
	bP.x = (int)(bP.x);
	bP.y = (int)(bP.y);
	bP.z = (int)(bP.z);

	if ((int)bP.x == 16)
		bP.x = 0;
	else if ((int)bP.y == 16)
		bP.y = 0;
	else if ((int)bP.z == 16)
		bP.z = 0;

	if (!(*chunk))
		return (NULL);
	block = &(*chunk)->getBlock(bP.w, bP.x, bP.y, bP.z);
	return (block);
}

static void	genNeighboor(Direction dir, Chunk *chunk, glm::vec4 posB)
{
	Chunk	*neighboor;

	neighboor = chunk->getNeighboorUnsafe(dir);
	if (neighboor)
		neighboor->generateGraphics((int)posB.w);
}

static void	setGenBlock(glm::vec4 posB, Chunk *chunk, e_BlockType type) // A VOIR
{
	int pos[4] = {(int)posB.w, (int)posB.x, (int)posB.y, (int)posB.z};
	chunk->setBlock(pos,
		(t_block_info){(unsigned char)type, 0, 0, 0});
	chunk->generateGraphics((int)posB.w);
	if ((int)posB.x == 0)
		genNeighboor(WEST, chunk, posB);
	else if ((int)posB.x == 15)
		genNeighboor(EAST, chunk, posB);
	if ((int)posB.z == 0)
		genNeighboor(SOUTH, chunk, posB);
	else if ((int)posB.z == 15)
		genNeighboor(NORTH, chunk, posB);
	if ((int)posB.y == 0 && (int)posB.w > 0)
		chunk->generateGraphics((int)posB.w - 1);
	else if ((int)posB.y == 15 && (int)posB.w < 15)
		chunk->generateGraphics((int)posB.w + 1);
}

void		Engine::rayCasting(World &world)
{
	glm::vec3		ray;
	glm::vec3		pos = this->camera.getTranslate();
	unsigned int	distBlock = 9;
	Block			*currentBlock;
	glm::vec4		currentBP;
	glm::vec4		saveBP;
	unsigned int	i;

	Chunk			*saveChunk = nullptr;
	Chunk			*chunk = nullptr;

	if (this->lockRay || this->isCursor)
		return;
	if (this->getButton(GLFW_MOUSE_BUTTON_1) == false && this->getButton(GLFW_MOUSE_BUTTON_2) == false)
	{
		this->getHud().setCursorColor(WHITE_CURSOR);
		return ;
	}

	currentBlock = getBlockFromPos(&chunk, pos, currentBP, world); // Check si la position de base est dans un cube d'air
	if (!currentBlock || !chunk || !isInAirBlock(*currentBlock) || chunk->getFenced() == UNFENCED) //
	{
		this->getHud().setCursorColor(RED_CURSOR);
		return;
	}
	ray = this->camera.createRay(glm::vec2((float)this->monitorWidth / 2.0, (float)this->monitorHeight / 2.0), this->monitorWidth, this->monitorHeight);
	for (i = 0; currentBlock && i < distBlock && isInAirBlock(*currentBlock); i++)
	{
		saveChunk = chunk;
		saveBP = currentBP;
		stepLoop(pos, ray);
		currentBlock = getBlockFromPos(&chunk, pos, currentBP, world);
	}
	if (i == distBlock || i == 0 || !currentBlock || isInAirBlock(*currentBlock) || !chunk) //  Si le block est inaccessible
	{
		this->getHud().setCursorColor(RED_CURSOR);
		return;
	}

	this->getHud().setCursorColor(GREEN_CURSOR);
	printf(BOLD_MAGENTA "Chunk %d %d\n" NA, chunk->getPos().get(0), chunk->getPos().get(1));
	printf(MAGENTA "Cam %f %f\n" NA, this->camera.getTranslate().x, this->camera.getTranslate().z);

	if (this->getButton(GLFW_MOUSE_BUTTON_1) == true)
		setGenBlock(saveBP, saveChunk, STONE);
	else
		setGenBlock(currentBP, chunk, AIR);
	this->lockRay = true;
}

int			Engine::setButton(unsigned int b, bool value)
{
	this->buttons[b] = value;
	if (!value)
		this->lockRay = false;
	return (0);
}

bool		Engine::getButton(unsigned int k)
{
	if (k >= GLFW_MOUSE_BUTTON_LAST)
		return (false);
	return (this->buttons[k]);
}

Hud&		Engine::getHud()
{
	return (this->hud);
}

static void	fillTempVbo(vector<vbo_type> &tempVbo, t_direction_consts dir_c, unsigned int idxgTxtPath)
{
	int			iPt;
	vbo_type	vboType;
	short		idBitwise;

	for (iPt = 0; iPt < 6; iPt++)
	{
		vboType.pos[0] = dir_c.pts[iPt].get(X);
		vboType.pos[1] = dir_c.pts[iPt].get(Y);
		vboType.pos[2] = dir_c.pts[iPt].get(Z);
		vboType.meta = dir_c.axis < 0 ? dir_c.axis + 7 : dir_c.axis;
		if (vboType.meta == 1 || vboType.meta == 6) // X et -X
		{
			vboType.coords[1] = dir_c.pts[iPt].get(Y);
			if (vboType.meta == 1)
				vboType.coords[0] = 1. - dir_c.pts[iPt].get(Z);
			else
				vboType.coords[0] = dir_c.pts[iPt].get(Z);
		}
		else if (vboType.meta == 2 || vboType.meta == 5) // Y et -Y
		{
			vboType.coords[0] = dir_c.pts[iPt].get(X);
			if (vboType.meta == 2)
				vboType.coords[1] = 1. - dir_c.pts[iPt].get(Z);
			else
			vboType.coords[1] = dir_c.pts[iPt].get(Z);
		}
		else // Z et -Z
		{
			vboType.coords[1] = dir_c.pts[iPt].get(Y);
			if (vboType.meta == 3)
				vboType.coords[0] = dir_c.pts[iPt].get(X);
			else
				vboType.coords[0] = 1. - dir_c.pts[iPt].get(X);
		}
		idBitwise = (int)g_txt_path[idxgTxtPath].type << 8;
		vboType.meta = vboType.meta | idBitwise;
		tempVbo.push_back(vboType);
	}
}

int			Engine::genSkybox(void)
{
	vector<vbo_type>	tempVbo;

	if (sky || this->shaderSky.loadShader((char*)VERTEX_SKY, (char*)FRAGMENT_SKY))
		return (1);
	for (int i = 0; i < 6; i++)
		fillTempVbo(tempVbo, g_dir_c[i], i + SKY_FRONT_T);
	glGenVertexArrays(1, &this->vaoSky);
	glBindVertexArray(this->vaoSky);
	glGenBuffers(1, &this->vboSky);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboSky);
	glBufferData(GL_ARRAY_BUFFER, tempVbo.size() * (sizeof(float) * 9), &tempVbo[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)(sizeof(float) * 6));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)(sizeof(float) * 8));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	this->sky = true;
	return (0);
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
	Shader		&shader = this->shaderSky;
	Camera		centerCam;
	
	this->getCam().look();
	centerCam.setRotation(this->getCam().getRotation());
	centerCam.setTranslate(glm::vec3(0.5, 0.5, 0.5));
	glDepthMask(false);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(this->vaoSky);
	glUseProgram(shader.getProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
		"view"), 1, GL_FALSE, glm::value_ptr(centerCam.getMatrix(true)));
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
		"projection"), 1, GL_FALSE, glm::value_ptr(this->getCam().getProjMatrix()));
	glUniform1i(glGetUniformLocation(shader.getProgram(),
		"basicTexture"), t ? t->getTxt() : 0);
	glUniform1i(glGetUniformLocation(shader.getProgram(),
		"nbTxt"), SKY_BOTTOM_T - END_BLOCK_T);
	glDrawArrays(GL_TRIANGLES, 0, NB_PTS_CUBE);
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
	bool	isPng;

	while (i < end && (!load || this->textures.size() < 16))
	{
		len = strlen(g_txt_path[i].path_txt);
		str.assign(g_txt_path[i].path_txt, len);
		isPng = str.find(".png") == (len - 4);
		if (len && load && isPng)
			this->addTexture((char*)g_txt_path[i].path_txt);
		else if (!load && isPng)
			this->textures.push_back(new Textures((char*)g_txt_path[i].path_txt));
		else if (!isPng)
			this->textures.push_back(new Textures((char*)g_txt_path[TEST_T].path_txt));
		str.clear();
		i++;
	}
}

static int	fillBuffer(std::string &buffer, std::vector<Textures*> &textures, glm::vec2 len, size_t offset)
{
	unsigned long	i;
	unsigned long	iX;
	unsigned long	iY;
	unsigned long	lenTxt = len.x * len.y * sizeof(int);

	for (unsigned long idxTxt = offset; idxTxt < textures.size() && textures[idxTxt]; idxTxt++)
	{
		// printf(RED "Boucle %d Height:%d Width%d\n" NA, idxTxt, textures[idxTxt]->getHeight(), textures[idxTxt]->getWidth());
		if (textures[idxTxt]->getHeight() != (int)len.y || textures[idxTxt]->getWidth() != (int)len.x)
		{
			printf(RED "Wrong size of a texture\n" NA);
			return (1);
		}
		for (iY = 0; iY < (unsigned long)textures[idxTxt]->getHeight(); iY++)
		{
			for (iX = 0; iX < (unsigned long)textures[idxTxt]->getWidth(); iX++)
			{
				for (i = 0; i < 4 && textures[idxTxt]->getTxtData(); i++)
				{
					if (i >= (unsigned int)textures[idxTxt]->getNrChannels())
						(buffer)[(idxTxt - offset) * lenTxt
						+ iY * textures[idxTxt]->getWidth() * sizeof(int)
						+ iX * sizeof(int) + i] = (char)255;
					else
						(buffer)[(idxTxt - offset) * lenTxt
						+ iY * textures[idxTxt]->getWidth() * sizeof(int)
						+ iX * sizeof(int) + i]
							= textures[idxTxt]->getTxtData()
								[iY * textures[idxTxt]->getWidth() * textures[idxTxt]->getNrChannels()
								+ iX * textures[idxTxt]->getNrChannels() + i];
				}
			}
		}
	}
	return (0);
}

int		Engine::genBlocksTextures(glm::vec2 len, e_txt start, e_txt end, size_t offsetInTexture)
{
	std::string		buffer;
	size_t			size;
	size_t			size_y;
	int				nbTxt;

	this->fillTextureVector(start, end, false);
	for (int i = offsetInTexture; i < (int)this->textures.size(); i++)
		printf(BLUE "Height:%d Width%d\n" NA, textures[i]->getHeight(), textures[i]->getWidth());
	nbTxt = this->textures.size() - offsetInTexture;
	if (nbTxt < 1)
		return (1);
	size = len.x * len.y * sizeof(int) * nbTxt;
	buffer.resize(size);
	size_y = nbTxt * len.y;
	if (!buffer.size())
		return (1);
	if (fillBuffer(buffer, this->textures, len, offsetInTexture))
		return (1);
	for (int i = (int)this->textures.size(); i > (int)offsetInTexture; i--)
	{
		delete this->textures[i - 1];
		this->textures.pop_back();
	}
	this->addTexture((char*)buffer.c_str(), len.x, size_y);
	return (0);
}

Textures	*Engine::getTexture(unsigned int t)
{
	if (t < this->textures.size())
		return (this->textures[t]);
	return (NULL);
}

unsigned int Engine::getNbTextures(void)
{
	return (textures.size());
}

int			Engine::genTextures(void)
{
	if (this->genBlocksTextures((glm::vec2){16, 16}, DIRT_T, END_BLOCK_T, 0))
		return (1);
	if (this->genBlocksTextures((glm::vec2){2048, 2048}, SKY_FRONT_T, SKY_T, 1))
		return (1);
	this->fillTextureVector(SKY_T + 1, END_T, true);
	return (0);
}


void	Engine::addTexture(char *buffer, unsigned long width, unsigned long height)
{
	unsigned int	textIdx;
	Textures		*t;

	this->textures.push_back(new Textures(buffer, width, height));
	glGenTextures(1, &textIdx);
	t = this->textures[this->textures.size() - 1];
	t->setTxt(textIdx);
	glActiveTexture(GL_TEXTURE0 + this->textures.size());
	glBindTexture(GL_TEXTURE_2D, t->getTxt());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (this->textures.size() == 1 || this->textures.size() == 2)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->getWidth(), t->getHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, t->getTxtData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void	Engine::addTexture(char *path)
{
	unsigned int	textIdx;
	Textures		*t;

	this->textures.push_back(new Textures(path));
	glGenTextures(1, &textIdx);
	t = this->textures[this->textures.size() - 1];
	t->setTxt(textIdx);
	glActiveTexture(GL_TEXTURE0 + this->textures.size());
	glBindTexture(GL_TEXTURE_2D, t->getTxt());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (this->textures.size() == 1 || this->textures.size() == 2)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->getWidth(), t->getHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, t->getTxtData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

glm::vec2	Engine::getMouseLastPos(void)
{
	return (this->mouseLastPos);
}

void		Engine::setMouseLastPos(glm::vec2 v)
{
	this->mouseLastPos = v;
}

bool		Engine::isFirst(void)
{
	return (this->firstMouse);
}

void		Engine::setFirst(bool f)
{
	this->firstMouse = f;
}

void 		Engine::deleteText()
{
	int				i;
	int				t;

	i = 0;
	while ((unsigned int)i < this->textures.size())
	{
		t = this->textures[i]->getTxt();
		if (t != -1)
			glDeleteTextures(1, (GLuint*)&t);
		delete this->textures[i];
		i++;
	}
	if (this->shader.getProgram())
		this->shader.freeProgram();
	this->hud.deleteHud();
}

Engine::~Engine()
{
}

///////////////Private///////////////