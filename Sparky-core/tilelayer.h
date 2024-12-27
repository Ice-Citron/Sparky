#pragma once

#include "src/graphics/layers/layer.h"
#include "src/graphics/batch2drenderer.h"


class TileLayer : public sparky::graphics::Layer{
public:
	TileLayer(sparky::graphics::Shader* shader);
	~TileLayer();
};


