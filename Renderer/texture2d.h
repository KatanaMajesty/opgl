#pragma once

enum class TextureType
{
	Diffuse,
	Specular,
};

struct Texture2D
{
public:
	GLuint m_id;
	TextureType m_type;
	std::string m_path;
};