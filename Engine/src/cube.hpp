#pragma once
#include <vector>
#include "triangle.hpp"
#include "vertex.hpp"

struct Cube
{
	std::vector<engine::Vertex> cubeVertices;
	std::vector<uint32_t> cubeIndices;
	std::vector<XMVECTOR> cubeNormals;
	
public:
	Cube()
	: cubeVertices{
			{{-1.0f, -1.0f, -1.0f }, {0.0f, 1.0f}},
			{{-1.0f,  1.0f, -1.0f }, {0.0f, 0.0f}},
			{{1.0f,  1.0f, -1.0f  }, {1.0f, 0.0f}},
			{{1.0f, -1.0f, -1.0f  }, {1.0f, 1.0f}},
			{{-1.0f, -1.0f, 1.0f  }, {1.0f, 1.0f}},
			{{1.0f, -1.0f, 1.0f   }, {0.0f, 1.0f}},
			{{1.0f,  1.0f, 1.0f   }, {0.0f, 0.0f}},
			{{-1.0f,  1.0f, 1.0f  },{1.0f, 0.0f}},
			{{-1.0f, 1.0f, -1.0f  },{0.0f, 1.0f}},
			{{-1.0f, 1.0f,  1.0f  },{0.0f, 0.0f}},
			{{1.0f, 1.0f,  1.0f   }, {1.0f, 0.0f}},
			{{1.0f, 1.0f, -1.0f   }, {1.0f, 1.0f}},
			{{-1.0f, -1.0f, -1.0f },{1.0f, 1.0f}},
			{{1.0f, -1.0f, -1.0f  },  {0.0f, 1.0f}},
			{{1.0f, -1.0f,  1.0f  },{0.0f, 0.0f}},
			{{-1.0f, -1.0f,  1.0f },{1.0f, 0.0f}},
			{{-1.0f, -1.0f,  1.0f },{0.0f, 1.0f}},
			{{-1.0f,  1.0f,  1.0f },{0.0f, 0.0f}},
			{{-1.0f,  1.0f, -1.0f },{1.0f, 0.0f}},
			{{-1.0f, -1.0f, -1.0f },{1.0f, 1.0f}},
			{{1.0f, -1.0f, -1.0f  }, {0.0f, 1.0f}},
			{{1.0f,  1.0f, -1.0f  }, {0.0f, 0.0f}},
			{{1.0f,  1.0f,  1.0f  }, {1.0f, 0.0f}},
			{{1.0f, -1.0f,  1.0f  }, {1.0f, 1.0f}}
	}, cubeIndices{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	}

	{}
};