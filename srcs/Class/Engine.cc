/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:52:39 by gperez            #+#    #+#             */
/*   Updated: 2020/12/24 00:57:17 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

using namespace std;

Engine::Engine()
{
	this->sky = false;
	this->firstMouse = true;
	this->lockRay = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

	// glfwWindowHint(GLFW_AUTO_ICONIFY, GL_TRUE);
	// this->window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", glfwGetPrimaryMonitor(), NULL);
	this->window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
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
	float		step = 0.001;

	do
	{
		pos += ray * step;
	}	while (!switchedBlock(startPos, pos));
}

static bool isInAirBlock(Block currentBlock)
{
	return (currentBlock.getInfo().id == AIR);
}

Block		*Engine::getBlockFromPos(Chunk **chunk, ChunkPos &prev, glm::vec3 pos, glm::vec4 &bP, std::map<ChunkPos, Chunk*> memory)
{
	Block		*block;
	ChunkPos	chunkPos = Camera::getCurrentChunkPos(pos);

	(*chunk) = memory.at(chunkPos);
	bP = glm::vec4(Camera::getCurrentOffset(pos), (int)(pos.y / 16));
	if (/*prev.get(0) < chunkPos.get(0) && */bP.x + PREC < 0.0)
		bP.x = 1.0 + bP.x;
	if (/*prev.get(1) < chunkPos.get(1) && */bP.z + PREC < 0.0)
		bP.z = 1.0 + bP.z;
	
	bP.x *= 16.0;
	bP.y *= 16.0;
	bP.z *= 16.0;
	prev = chunkPos;
	if (!(*chunk))
		return (NULL);
	block = &(*chunk)->getBlock(bP.w, bP.x, bP.y, bP.z);
	if ((this->getButton(GLFW_MOUSE_BUTTON_1) == true || this->getButton(GLFW_MOUSE_BUTTON_2) == true)
		&& this->lockRay == false)
	{
		ft_printf(RED "ChunkPos %d %d\n" NA, (*chunk)->getPos().get(0), (*chunk)->getPos().get(1));
		ft_printf(ORANGE "Bp %f %f %f\n" NA, bP.x, bP.y, bP.z);
	}
	return (block);
}

static void	genNeighboor(Direction dir, Chunk *chunk, glm::vec4 posB)
{
	Chunk	*neighboor;

	neighboor = chunk->getNeighboor(dir);
	if (neighboor)
		neighboor->generateGraphics((int)posB.w);
}

static void	setGenBlock(glm::vec4 posB, Chunk *chunk, e_BlockType type)
{
	chunk->setBlock((int[4]){(int)posB.w, (int)posB.x, (int)posB.y, (int)posB.z},
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

void		Engine::rayCasting(Chunk *chunk, map<ChunkPos, Chunk*> &memory)
{
	glm::vec3		ray;
	glm::vec3		pos = this->camera.getTranslate();
	unsigned int	distBlock = 9;
	Block			*currentBlock;
	glm::vec4		currentBP;
	glm::vec4		saveBP;
	ChunkPos		prev((int[2]){0, 0});
	unsigned int	i;

	if (!chunk)
		return;
	if (this->getButton(GLFW_MOUSE_BUTTON_1) == false && this->getButton(GLFW_MOUSE_BUTTON_2) == false)
	{
		this->getHud().setCursorColor(WHITE_CURSOR);
		return ;
	}
	currentBlock = getBlockFromPos(&chunk, prev, pos, currentBP, memory);
	if (!currentBlock || !chunk || !isInAirBlock(*currentBlock) || chunk->getFenced() == UNFENCED)
	{
		this->getHud().setCursorColor(RED_CURSOR);
		return;
	}
	ray = this->camera.createRay(glm::vec2(WIDTH / 2.0, HEIGHT / 2.0), WIDTH, HEIGHT);
	for (i = 0; currentBlock && i < distBlock && isInAirBlock(*currentBlock); i++)
	{
		saveBP = currentBP;
		stepLoop(pos, ray);
		currentBlock = getBlockFromPos(&chunk, prev, pos, currentBP, memory);
	}
	if (i == distBlock)
		this->getHud().setCursorColor(RED_CURSOR);
	else
		this->getHud().setCursorColor(GREEN_CURSOR);

	// if (this->getButton(GLFW_MOUSE_BUTTON_1) == false && this->getButton(GLFW_MOUSE_BUTTON_2) == false)
	// 	return ;

	if (!this->lockRay && i != distBlock && currentBlock && !isInAirBlock(*currentBlock) && chunk)
	{
		ft_printf(BOLD_MAGENTA "Chunk %d %d\n" NA, chunk->getPos().get(0), chunk->getPos().get(1));
		ft_printf(MAGENTA "Cam %f %f\n" NA, this->camera.getTranslate().x, this->camera.getTranslate().z);
		if (this->getButton(GLFW_MOUSE_BUTTON_1) == true)
			setGenBlock(saveBP, chunk, STONE);
		else
			setGenBlock(currentBP, chunk, AIR);
		this->lockRay = true;
	}
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
		vboType.tab[0] = dir_c.pts[iPt].get(X);
		vboType.tab[1] = dir_c.pts[iPt].get(Y);
		vboType.tab[2] = dir_c.pts[iPt].get(Z);
		vboType.meta = dir_c.axis < 0 ? dir_c.axis + 7 : dir_c.axis;
		if (vboType.meta == 1 || vboType.meta == 6)
		{
			vboType.coords[0] = dir_c.pts[iPt].get(Z);
			vboType.coords[1] = dir_c.pts[iPt].get(Y);
		}
		else if (vboType.meta == 2 || vboType.meta == 5)
		{
			vboType.coords[0] = dir_c.pts[iPt].get(X);
			vboType.coords[1] = dir_c.pts[iPt].get(Z);
		}
		else
		{
			vboType.coords[0] = dir_c.pts[iPt].get(X);
			vboType.coords[1] = dir_c.pts[iPt].get(Y);
		}
		idBitwise = (int)g_txt_path[idxgTxtPath].type << 8;
		vboType.meta = (int)vboType.meta | idBitwise;
		tempVbo.push_back(vboType);
	}
}

int			Engine::genSkybox(void)
{
	vector<vbo_type>	tempVbo;

	if (sky || this->shaderSky.loadShader((char*)VERTEX_SKY, (char*)FRAGMENT_SKY))
		return (1);
	for (int i = 0; i < 6; i++)
	{
		fillTempVbo(tempVbo, g_dir_c[i], i + SKY_FRONT_T);
	}
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
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
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
		len = ft_strlen(g_txt_path[i].path_txt);
		str.assign(g_txt_path[i].path_txt, len);
		isPng = str.find(".png") == (len - 4);
		if (len && load && isPng)
			this->addTexture((char*)g_txt_path[i].path_txt, 0, 0);
		else if (!load && isPng)
			this->textures.push_back(new Textures((char*)g_txt_path[i].path_txt));
		else if (!isPng)
			this->textures.push_back(new Textures((char*)g_txt_path[TEST_T].path_txt));
		str.clear();
		i++;
	}
}

static int	fillBuffer(char **buffer, std::vector<Textures*> &textures, glm::vec2 len, size_t offset)
{
	unsigned long	i;
	unsigned long	iX;
	unsigned long	iY;
	unsigned long	lenTxt = len.x * len.y * sizeof(int);

	for (unsigned long idxTxt = offset; idxTxt < textures.size() && textures[idxTxt]; idxTxt++)
	{
		// ft_printf(RED "Boucle %d Height:%d Width%d\n" NA, idxTxt, textures[idxTxt]->getHeight(), textures[idxTxt]->getWidth());
		if (textures[idxTxt]->getHeight() != (int)len.y || textures[idxTxt]->getWidth() != (int)len.x)
		{
			ft_printf(RED "Wrong size of a texture\n" NA);
			return (1);
		}
		for (iY = 0; iY < (unsigned long)textures[idxTxt]->getHeight(); iY++)
		{
			for (iX = 0; iX < (unsigned long)textures[idxTxt]->getWidth(); iX++)
			{
				for (i = 0; i < 4 && textures[idxTxt]->getTxtData(); i++)
				{
					if (i >= (unsigned int)textures[idxTxt]->getNrChannels())
						(*buffer)[(idxTxt - offset) * lenTxt
						+ iY * textures[idxTxt]->getWidth() * sizeof(int)
						+ iX * sizeof(int) + i] = (char)255;
					else
						(*buffer)[(idxTxt - offset) * lenTxt
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
	char			*buffer;
	size_t			size;
	size_t			size_y;
	int				nbTxt;

	// ft_printf(RED "Avant fillTexture\n" NA);
	this->fillTextureVector(start, end, false);
	for (int i = 0; i < (int)this->textures.size(); i++)
	{
		ft_printf(BLUE "Height:%d Width%d\n" NA, textures[i]->getHeight(), textures[i]->getWidth());
	}
	// ft_printf(RED "Apres fillTexture\n" NA);
	nbTxt = this->textures.size() - offsetInTexture;
	if (nbTxt < 1)
		return (1);
	size = len.x * len.y * sizeof(int) * nbTxt;
	buffer = (char*)ft_memalloc(size);
	size_y = nbTxt * len.y;
	if (!buffer)
		return (1);
	// ft_printf(RED "Avant fillBuffer\n" NA);
	if (fillBuffer(&buffer, this->textures, len, offsetInTexture))
		return (1);
	// ft_printf(RED "AVANT\n" NA);
	for (int i = (int)this->textures.size(); i > (int)offsetInTexture; i--)
	{
		// ft_printf(YELLOW "Idx:%d Size:%d\n" NA, i - 1, this->textures.size());
		delete this->textures[i - 1];
		this->textures.pop_back();
	}
	// ft_printf(RED "APRES\n" NA);
	this->addTexture(buffer, len.x, size_y);
	// ft_printf(RED "Apres addTexture\n" NA);
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
	if (this->genBlocksTextures((glm::vec2){512, 512}, SKY_FRONT_T, (e_txt)(SKY_T), 1))
		return (1);
	this->fillTextureVector(SKY_T + 1, END_T, true);
	return (0);
}

void	Engine::addTexture(char *pathOrBuffer, unsigned long width, unsigned long height)
{
	unsigned int	textIdx;
	Textures		*t;

	if (width == 0 && height == 0)
		this->textures.push_back(new Textures(pathOrBuffer));
	else
		this->textures.push_back(new Textures(pathOrBuffer, width, height));
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

Engine::~Engine()
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
}

///////////////Private///////////////