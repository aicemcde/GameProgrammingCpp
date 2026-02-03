#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <SDL.h>
#include "Math.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Game.h"
#include "ResourceManager.h"
#include <json.hpp>
#include "Log.h"
#include "GameSystem.h"

const size_t VERTICS_SIZE = 8;
const size_t INDICES_SIZE = 3;

using json = nlohmann::json;

Mesh::Mesh()
	:mVertexArray(nullptr)
	,mRadius(0.0f)
	,mSpecPower(0.0f)
{
	
}

Mesh::~Mesh() = default;

bool Mesh::Load(const std::string& fileName, Renderer* renderer, GameContext* context)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		LOG_WARN("File not found: {}", fileName);
		return false;
	}

	json doc;
	try
	{
		file >> doc;
	}
	catch (const json::parse_error& e)
	{
		LOG_ERROR("Json parse Error in {}: {}", fileName, e.what());
		return false;
	}

	if (!doc.is_object())
	{
		LOG_ERROR("Mesh {} is not vailed", fileName);
		return false;
	}

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	try
	{
		if (!doc.contains("version") || doc["version"].get<int>() != 1)
		{
			LOG_ERROR("Mesh {} is not version 1", fileName);
			return false;
		}

		mShaderName = doc["shader"].get<std::string>();

		if (!doc.contains("textures") || !doc["textures"].is_array() || doc["textures"].size() < 1)
		{
			LOG_ERROR("Mesh {} has no textures, there should be at least one", fileName);
			return false;
		}
		
		const auto& texJson = doc["textures"];

		mSpecPower = doc["specularPower"].get<float>();

		for (const auto& tex : texJson)
		{
			std::string texName = tex.get<std::string>();
			Texture* t = context->resource->GetTexture(texName);
			if (t == nullptr)
			{
				t = context->resource->GetTexture(texName);
			}
			mTextures.emplace_back(t);
		}

		if (!doc.contains("vertices") || !doc["vertices"].is_array() || doc["vertices"].size() < 1)
		{
			LOG_ERROR("Mesh {} has no vertices", fileName);
			return false;
		}

		const auto& vertsJson = doc["vertices"];
		vertices.reserve(vertsJson.size() * VERTICS_SIZE);
		mRadius = 0.0f;

		for (const auto& vert : vertsJson)
		{
			if (!vert.is_array() || vert.size() != VERTICS_SIZE)
			{
				LOG_ERROR("Unexpected vertex format for {}", fileName);
				return false;
			}

			Vector3 pos(vert[0].get<float>(), vert[1].get<float>(), vert[2].get<float>());
			mObjectBox.UpdateMinMax(pos);
			//mRadius is squared
			mRadius = Math::Max(mRadius, pos.LengthSq());

			for (const auto& val : vert)
			{
				vertices.emplace_back(val.get<float>());
			}
		}

		mRadius = Math::Sqrt(mRadius);

		if (!doc.contains("indices") || !doc["indices"].is_array() || doc["indices"].size() < 1)
		{
			LOG_ERROR("Mesh {} has no indices", fileName);
			return false;
		}

		const auto& indJson = doc["indices"];
		
		indices.reserve(indJson.size() * INDICES_SIZE);

		for (const auto& ind : indJson)
		{
			indices.emplace_back(ind[0].get<UINT>());
			indices.emplace_back(ind[1].get<UINT>());
			indices.emplace_back(ind[2].get<UINT>());
		}
	}
	catch (const json::exception& e)
	{
		LOG_ERROR("Mesh {} failed to serialize: {}", fileName, e.what());
		return false;
	}

	mVertexArray = std::make_unique<VertexArray>(vertices.data(), static_cast<unsigned>(vertices.size()) / VERTICS_SIZE,
		indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload()
{
	mVertexArray.reset();
	mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}

