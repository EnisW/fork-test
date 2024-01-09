#include "TextRenderer.hpp"


TextRenderer::TextRenderer()
{
	updateFlag = false;
	scale = 1.0f;
	vertices = std::vector<Vertex>(0);
	indices = std::vector<unsigned int>(0);
	this->programID = LoadShaders("shaderText.vert", "shaderText.frag");
	glUseProgram(this->programID);
	screenHeight = HEIGHT;
	screenWidth = WIDTH;

	textureEnabled = false;

	modelMatrices.resize(MAX_OBJECT);
	modelMatrixUsed.resize(MAX_OBJECT);
	for (int i = 0; i < MAX_OBJECT; i++) {
		modelMatrixUsed[i] = false;
	}


	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, color))
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, normal))
	);


	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, texCoord))
	);


	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, modelIndex))
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
void TextRenderer::addText(Text* text)
{

	texts.push_back(text);
	text->index = texts.size() - 1;
	text->vertexOffset = vertices.size();
	text->indexOffset = indices.size();

	std::cout << vertices.size() << std::endl;
	std::cout << indices.size() << std::endl;

	unsigned int _vertexCount = 0;
	unsigned int _indexCount = 0;


	float currentXBase = text->x;
	float currentYBase = text->y;

	int textLength = text->text.length();
	int skip = 0;
	for (int i = 0; i < textLength; ++i) {

		char currentChar = text->text[i];
		Character currentCharacter = font_Arial.characters[currentChar - 32];
		if (currentChar == ' ') {
			currentXBase += (float)font_Arial.characters[16].width / scale / 2;
			skip++;
			continue;
		}

		if (currentChar == '|') {
			currentXBase = text->x;
			currentYBase += (float)currentCharacter.height/ (float)2;
			skip++;
			continue;
		}

		float u1, v1, u2, v2;
		CalculateUV(currentCharacter, u1, v2, u2, v1);
		float currentX = ((currentXBase) / WIDTH)*2.0f - 1.0f;
		float currentY = -(((currentYBase) / HEIGHT)*2.0f - 1.0f);

		Vertex vertex1 = { glm::vec3(currentX , currentY , 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u1, v1), 0 };
		Vertex vertex2 = { glm::vec3(currentX + (float)currentCharacter.width / WIDTH/ scale, currentY , 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u2, v1), 0};
		Vertex vertex3 = { glm::vec3(currentX , currentY + (float)currentCharacter.height / HEIGHT / scale, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u1, v2), 0};
		Vertex vertex4 = { glm::vec3(currentX + (float)currentCharacter.width / WIDTH / scale, currentY + (float)currentCharacter.height / HEIGHT / scale, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u2, v2), 0};

		vertices.push_back(vertex1);
		vertices.push_back(vertex2);
		vertices.push_back(vertex3);
		vertices.push_back(vertex4);
		_vertexCount += 4;

		indices.push_back(2 + (i-skip) * 4);
		indices.push_back(0 + (i - skip) * 4);
		indices.push_back(1 + (i - skip) * 4);
		indices.push_back(2 + (i - skip) * 4);
		indices.push_back(1 + (i - skip) * 4);
		indices.push_back(3 + (i - skip) * 4);
		_indexCount += 6;
		currentXBase += (float)currentCharacter.width/ scale/2;
	}

	text->indexCount = _indexCount;
	text->vertexCount = _vertexCount;

	updateFlag = true;

}

void TextRenderer::removeText(Text* text)
{
	textRemoveQueue.push_back(text);
}

void TextRenderer::removeAllText()
{
	for (Text* t : texts) {
		textRemoveQueue.push_back(t);
	}
}

void TextRenderer::setBitmapFont(std::string fontName)
{
	glUseProgram(this->programID);
	charUVs.resize(95);
	int nrChannels;
	unsigned char* data = stbi_load(fontName.c_str(), &textureWidth, &textureHeight, &nrChannels, 0);
	
	if (data) {
		glActiveTexture(GL_TEXTURE0 );
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
			std::cout << "Texture format supported" << std::endl;
		}
		else {
			std::cout << "Texture format not supported" << std::endl;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	for (int i = 0; i < font_Arial.characterCount; ++i) {
		Character currentCharacter = font_Arial.characters[i];

		// Calculate UV coordinates for the current character
		float u1, v1, u2, v2;
		CalculateUV(currentCharacter, u1, v1, u2, v2);
		charUVs[i].bottomLeftX = u1;
		charUVs[i].bottomLeftY = v1;

		charUVs[i].bottomRightX = u2;
		charUVs[i].bottomRightY = v1;

		charUVs[i].topLeftX = u1;
		charUVs[i].topLeftY = v2;

		charUVs[i].topRightX = u2;
		charUVs[i].topRightY = v2;
	}


}

void TextRenderer::render()
{
	if (updateFlag) {
		updateBuffers();
		updateFlag = false;
	}

	for(Text* t: textRemoveQueue)
		removeTextFromQueue(t);

	

	textRemoveQueue.clear();

	glUseProgram(this->programID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void TextRenderer::CalculateUV(Character character, float& u1, float& v1, float& u2, float& v2) {
    // Calculate normalized texture coordinates
    u1 = static_cast<float>(character.x) / (float)textureWidth;
    v1 = static_cast<float>(character.y) / (float)textureHeight;
    u2 = static_cast<float>(character.x + character.width) / (float)textureWidth;
    v2 = static_cast<float>(character.y + character.height) / (float)textureHeight;
}

void TextRenderer::setData(std::vector<Vertex>& data, std::vector<unsigned int>& indices)
{
	
		this->vertices = data;
		this->indices = indices;
		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

}

void TextRenderer::updateBuffers()
{
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

}

void TextRenderer::removeTextFromQueue(Text* text)
{


	this->vertices.erase(this->vertices.begin() + text->vertexOffset, this->vertices.begin() + text->vertexOffset + text->vertexCount);

	this->indices.erase(this->indices.begin() + text->indexOffset, this->indices.begin() + text->indexOffset + text->indexCount);
	texts.erase(texts.begin() + text->index);
	
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	if (vertices.size() > 0) {
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
	}
	else {
				glInvalidateBufferData(vertexbuffer);
				glInvalidateBufferData(elementBuffer);
	}
	
	
}
