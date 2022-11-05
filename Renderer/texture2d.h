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

	// Texture2D() = default;
	Texture2D(const std::string& path, TextureType type, bool flipv);

	inline const std::string& GetPath() const { return m_path; }

	static inline void Bind(Texture2D& texture, size_t slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot); 
		glBindTexture(GL_TEXTURE_2D, texture.m_id); 
	}

	static inline void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

private:
	void Load(bool flipV);
};