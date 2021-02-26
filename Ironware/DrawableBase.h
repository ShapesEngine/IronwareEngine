/*!
 * \file DrawableBase.h
 * \date 2020/11/25 14:34
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A CRTP class for managing static binds in the graph pipeline
 *
 * TODO:
 *
 * \note Index Buffer needs to be set separately for every geometry object type
*/
#pragma once

#include "Drawable.h"
#include "IndexBuffer.h"

template<typename T>
class DrawableBase : public Drawable
{
protected:
	static __forceinline bool IsStaticInitialized() { return !staticBinds.empty(); }

	static void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG );
	void AddStaticIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf ) noexcept;
	void SetIndexFromStatic() noexcept( !IS_DEBUG );

private:
	__forceinline const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override { return staticBinds; }

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

#include "DrawableBase.tpp"