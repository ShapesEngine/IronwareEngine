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
protected:
	static inline bool IsStaticInitialized() { return !staticBinds.empty(); }

	static void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG );
	void AddStaticIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf ) noexcept;
	void SetIndexFromStatic() noexcept( !IS_DEBUG );

private:
	inline const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override { return staticBinds; }

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

#include "DrawableBase.tpp"