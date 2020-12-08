#pragma once

#include "renderer_base.h"
#include "texture/texture.h"
#include "log/log.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <exception>


namespace Bubble
{
	struct Shader
	{
		std::string mName;
        uint32_t mShaderID;
		mutable std::unordered_map<std::string, int> mUniformCache;

    public:
        Shader() = default;

        Shader(const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;

		Shader(Shader&&);
		Shader& operator= (Shader&&);

		int GetUni(const std::string& name) const;

        void Bind() const;
        void Unbind() const;
        
        // Lone int 
        void SetUni1i(const std::string& name, const int& val) const;

        // Float vec
        void SetUni1f(const std::string& name, const float& val) const;
        void SetUni2f(const std::string& name, const glm::vec2& val) const;
        void SetUni3f(const std::string& name, const glm::vec3& val) const;
        void SetUni4f(const std::string& name, const glm::vec4& val) const;

        // Float matrices
        void SetUniMat3(const std::string& name, const glm::mat3& val) const;
        void SetUniMat4(const std::string& name, const glm::mat4& val) const;

        // Texture
        void SetTexture2D(const std::string& name, int tex_id, int slot = 0) const;
        void SetTexture2D(const std::string& name, const Texture2D& texture, int slot = 0) const;
    };
}