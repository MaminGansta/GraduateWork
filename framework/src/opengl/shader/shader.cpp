
#include "shader.h"


namespace Bubble
{
	
	Shader::Shader(Shader&& other)
		: mShaderID(other.mShaderID),
          mName(std::move(other.mName)),
          mUniformCache(std::move(mUniformCache))
	{
        other.mShaderID = 0;
	}


	Shader& Shader::operator=(Shader&& other)
	{
		mShaderID = other.mShaderID;
        mName = std::move(other.mName);
        mUniformCache = std::move(mUniformCache);
        other.mShaderID = 0;
        return *this;
	}


	void Shader::Bind() const
	{
        glcall(glUseProgram(mShaderID));
	}


	void Shader::Unbind() const
	{
        glcall(glUseProgram(0));
	}


	int Shader::GetUni(const std::string& uniformname) const
	{
		glcall(glUseProgram(mShaderID));
        if (mUniformCache.find(uniformname) != mUniformCache.end())
            return mUniformCache[uniformname];

		int unifromid = glGetUniformLocation(mShaderID, uniformname.c_str());
		if (unifromid == -1)
			LOG_CORE_WARN("Shader: {} doesn't have uniform: {}", mName, uniformname);
		
        mUniformCache[uniformname] = unifromid;
		return unifromid;
	}


	// Lone int 
    void Shader::SetUni1i(const std::string& name, const int& val) const
    {
        glcall(glUniform1i(GetUni(name), val));
    }

    // Float vec
    void Shader::SetUni1f(const std::string& name, const float& val) const
    {
        glcall(glUniform1f(GetUni(name), val));
    }

    void Shader::SetUni2f(const std::string& name, const glm::vec2& val) const
    {
        glcall(glUniform2f(GetUni(name), val.x, val.y));
    }

    void Shader::SetUni3f(const std::string& name, const glm::vec3& val) const
    {
        glcall(glUniform3f(GetUni(name), val.x, val.y, val.z));
    }

    void Shader::SetUni4f(const std::string& name, const glm::vec4& val) const
    {
        glcall(glUniform4f(GetUni(name), val.x, val.y, val.z, val.w));
    }

    // Float matrices
    void Shader::SetUniMat3(const std::string& name, const glm::mat3& val) const
    {
        glcall(glUniformMatrix3fv(GetUni(name), 1, GL_FALSE, glm::value_ptr(val)));
    }

    void Shader::SetUniMat4(const std::string& name, const glm::mat4& val) const
    {
        glcall(glUniformMatrix4fv(GetUni(name), 1, GL_FALSE, glm::value_ptr(val)));
    }

	// Texture
	void Shader::SetTexture2D(const std::string& name, int tex_id, int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		SetUni1i(name, slot);
	}

	void Shader::SetTexture2D(const std::string& name, const Texture2D& texture, int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture.GetRendererID());
		SetUni1i(name, slot);
	}

}