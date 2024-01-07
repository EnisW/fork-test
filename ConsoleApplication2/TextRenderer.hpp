#pragma once
#include "Util.hpp"
#include "Render.hpp"



class TextRenderer : public Renderer
{

public:
	TextRenderer();
	void addText(Text& text);
	void removeText(Text& text);
	void setBitmapFont(std::string fontName);
	void render() override;
	void CalculateUV(Character character, float& u1, float& v1, float& u2, float& v2);
	GLuint getProgramID() {
		return programID;
	};


private:
	unsigned int textureWidth, textureHeight;
	GLuint textureID;
	GLuint programID;
	std::vector<charUV> charUVs;
	std::vector<Text> texts;
};

