#pragma once

#include "Core.h"

#include "../Maths/Maths.h"
#include <vector>

namespace Cyclope {

	struct CYCLOPE_API Vertex {

		Vector3 position;
		Vector2 uv;

	};

	struct CYCLOPE_API Mesh {

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	};

}