/*!
 * \file RenderQueuePass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "BindingPass.h"
#include "Job.h"

#include <vector>

class RenderQueuePass : public BindingPass
{
public:
	using BindingPass::BindingPass;
	void Accept( Job job ) noexcept;
	void Execute( Graphics& gfx ) const IFNOEXCEPT override;
	void Reset() IFNOEXCEPT override;

private:
	std::vector<Job> jobs;
};