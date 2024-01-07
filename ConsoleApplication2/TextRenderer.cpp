#include "TextRenderer.hpp"


TextRenderer::TextRenderer()
{
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
void TextRenderer::addText(Text& text)
{
	
	texts.push_back(text);
	float currentXBase = text.x;
	float currentYBase = text.y;

	int textLength = text.text.length();
	for (int i = 0; i < textLength; ++i) {
		char currentChar = text.text[i];
		if (currentChar == '\n') {
			currentXBase = text.x;
			currentYBase -= font_Arial.height;
			continue;
		}
		Character currentCharacter = font_Arial.characters[currentChar - 32];
		float u1, v1, u2, v2;
		CalculateUV(currentCharacter, u1, v1, u2, v2);
		float currentX = (currentXBase + currentCharacter.originX) ;
		float currentY = (currentYBase + currentCharacter.originY) ;
		Vertex vertex1 = { glm::vec3(currentX , currentY , 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u1, v1), 0 };
		Vertex vertex2 = { glm::vec3(currentX + currentCharacter.width , currentY , 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u2, v1), 0};
		Vertex vertex3 = { glm::vec3(currentX , currentY + currentCharacter.height , 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u1, v2), 0};
		Vertex vertex4 = { glm::vec3(currentX + currentCharacter.width , currentY + currentCharacter.height , 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(u2, v2), 0};

		vertices.push_back(vertex1);
		vertices.push_back(vertex2);
		vertices.push_back(vertex3);
		vertices.push_back(vertex4);

		indices.push_back(0 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(3 + i * 4);
		currentXBase += currentCharacter.width;
	}
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	


}

void TextRenderer::removeText(Text& text)
{
}

void TextRenderer::setBitmapFont(std::string fontName)
{
	glUseProgram(this->programID);
	charUVs.resize(95);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fontName.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glActiveTexture(GL_TEXTURE0 );
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
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
    u1 = static_cast<float>(character.x) / textureWidth;
    v1 = static_cast<float>(character.y) / textureHeight;
    u2 = static_cast<float>(character.x + character.width) / textureWidth;
    v2 = static_cast<float>(character.y + character.height) / textureHeight;
}
