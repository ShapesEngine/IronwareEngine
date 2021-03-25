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
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw( Graphics & gfx ) const noexcept( !IS_DEBUG );
	virtual void Update( float dt ) noexcept = 0;
	void AddBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG );
	void AddIndexBufferBind( std::unique_ptr<class IndexBuffer> ibuf ) noexcept( !IS_DEBUG );

protected:
	/**
	 * @brief Retrieves specified bindable from binds collection
	 * @tparam T required bindable type
	 * @return pointer to the bindable type, if it was found
	*/
	template<typename T>
	T* QueryBindable();

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};