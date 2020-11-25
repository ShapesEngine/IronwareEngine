/*!
 * \file DrawableBase.h
 * \date 2020/11/25 14:34
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Drawable.h"
#include "IndexBuffer.h"

template<typename T>
class DrawableBase : public Drawable
{
public:
	inline bool IsStaticInitialized() const { return !staticBinds.empty(); }

	void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG );
	void AddStaticIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf ) noexcept;
	void SetIndexFromStatic() noexcept( !IS_DEBUG );

private:
	inline const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override { return staticBinds; }

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

template<typename T>
void DrawableBase<T>::AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG )
{
	assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid( *bind ) != typeid( IndexBuffer ) );
	staticBinds.push_back( std::move( bind ) );
}

template<typename T>
void DrawableBase<T>::AddStaticIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf ) noexcept
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	pIndexBuffer = ibuf.get();
	staticBinds.push_back( std::move( ibuf ) );
}

template<typename T>
void DrawableBase<T>::SetIndexFromStatic() noexcept( !IS_DEBUG )
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	for( const auto& b : staticBinds )
	{
		if( const auto p = dynamic_cast<IndexBuffer*>( b.get() ) )
		{
			pIndexBuffer = p;
			return;
		}
	}
	assert( "Failed to find index buffer in static binds" && pIndexBuffer != nullptr );
}