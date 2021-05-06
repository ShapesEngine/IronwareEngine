/*!
 * \file Bindable.h
 * \date 2020/09/23 4:31
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that contains a bindable class
 *
 * \note The reason that static functions exists is because Bindable is friend of Graphics class
 * * and we would like to give access only to these members for child classes.
 *
*/
#pragma once

#include "GraphicsResource.h"
#include "DxgiInfoManager.h"

#include <memory>

/*!
 * \class Bindable
 *
 * \ingroup Bindables
 *
 * \brief An abstract class that will be inherited from other bindable types.
 * * All the child classes will get bound with the Bind function.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Bindable : public GraphicsResource
{
public:
	/**
	 * @brief Binds the bindable type to the pipeline
	 * @param gfx Graphics object where the context and device are stored
	*/
	virtual void Bind( Graphics& gfx ) noexcept = 0;
	virtual ~Bindable() = default;
	virtual std::wstring GetUID() const noexcept = 0;
	virtual void InitializeParentReference( const class Drawable& ) noexcept {}
	virtual void Accept( class TechniqueProbe& ) {}
};

class CloningBindable : public Bindable
{
public:
	virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
};