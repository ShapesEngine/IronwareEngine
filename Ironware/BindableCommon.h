/*!
 * \file BindableCommon.h
 * \date 2020/09/23 6:18
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that includes all the bindable classes, as they are needed for pipeline
 *
*/
#pragma once

#include "ConstantBuffers.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "PrimitiveTopology.h"
#include "TransformCBufferEx.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "RasterizerState.h"

#ifdef IR_INCLUDE_TEXTURE
#include "Texture.h"
#include "Sampler.h"
// needed to import textures
#include "Surface.h"
#include "BlendState.h"
#endif

#include "BindableCollection.h"