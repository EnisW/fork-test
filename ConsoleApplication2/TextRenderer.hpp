#pragma once
#include "Util.hpp"
#include "Render.hpp"



class TextRenderer : public Renderer
{

public:
	TextRenderer();
	void addText(Text* text);

	void removeText(Text* text);
	void removeAllText();
	void setBitmapFont(std::string fontName);
	void render() override;
	void CalculateUV(Character character, float& u1, float& v1, float& u2, float& v2);
	void setData(std::vector<Vertex>& data, std::vector<unsigned int>& indices);
	void setScale(float scale) {
		this->scale = scale;
	};	
	GLuint getProgramID() {
		return programID;
	};
	GLuint getVertexArrayID() {
		return VertexArrayID;
	};
	GLuint getVertexBuffer() {
		return vertexbuffer;
	};
	GLuint getElementBuffer() {
		return elementBuffer;
	};

private:
	bool updateFlag;
	void updateBuffers();
	void removeTextFromQueue(Text* text);
	float scale;
	int textureWidth, textureHeight;
	GLuint textureID;
	GLuint programID;
	std::vector<charUV> charUVs;
	std::vector<Text*> texts;
	std::vector<Text*> textRemoveQueue;
};

