/*!
 * \file BufferResource.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

class Graphics;

class BufferResource
{
public:
	virtual ~BufferResource() = default;
	virtual void BindAsBuffer( Graphics& ) IFNOEXCEPT = 0;
	virtual void BindAsBuffer( Graphics&, BufferResource* ) IFNOEXCEPT = 0;
	virtual void Clear( Graphics& ) IFNOEXCEPT = 0;
};