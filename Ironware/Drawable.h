/*!
 * \file Drawable.h
 * \date 2020/09/23 5:49
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a drawable class
 *
 * \note It contains: pIndexBuffer, as it's needed to get its count.
 * *				  binds collection that stores various bindable types in it.
*/
#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "CommonMacros.h"
#include "RenderTechnique.h"

#include <DirectXMath.h>

#include <memory>

/*!
 * \class Drawable
 *
 * \ingroup Drawables
 *
 * \brief A drawable abstract class that controls(partly) the graphics pipeline and provides
 * * an interface for the other geometry object types
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Drawable
{
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void AddTechnique( RenderTechnique tech_in ) noexcept;
	void Submit( class FrameExecutor& frame ) const noexcept;
	void Bind( Graphics& gfx ) const noexcept;
	void Accept( class TechniqueProbe& probe );
	UINT GetIndexCount() const IFNOEXCEPT;

protected:
	std::shared_ptr<class IndexBuffer> pIndices;
	std::shared_ptr<class VertexBuffer> pVertices;
	std::shared_ptr<class PrimitiveTopology> pTopology;

private:
	std::vector<RenderTechnique> techniques;
};