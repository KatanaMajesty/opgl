#pragma once

enum class TextureType
{
	DIFFUSE,
	SPECULAR,
	UNDEFINED,
};

struct Texture2D
{
private:
	GLuint m_id;
	TextureType m_type;
	std::string m_path;

public:
	Texture2D(const std::string& path, TextureType type, bool flipv, GLint wrapParam = GL_REPEAT);
	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;
	Texture2D(Texture2D&& other);
	Texture2D& operator=(Texture2D&& other);

	inline const std::string& GetPath() const { return m_path; }

	static inline void Bind(const Texture2D& texture, size_t slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot); 
		glBindTexture(GL_TEXTURE_2D, texture.m_id); 
	}

	static inline void Unbind(size_t slot = 0) 
	{ 
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0); 
	}

	inline void Delete() { glDeleteTextures(1, &m_id); };

private:
	void Load(bool flipV, GLint wrapParam);
};