#pragma once

#include "Cyclope.h"

#include "SceneViewCamera.h"

using namespace Cyclope;

namespace CyclopeEditor {

	class Grid {

	public:
		Grid();
		void Render(const SceneViewCamera& svc);

	private:
		Shared<VertexArray> m_VA;
		Shared<Shader> m_shader;

	};

}