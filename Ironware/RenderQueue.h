#pragma once

#include "Graphics.h"
#include "RenderJob.h"

#include <vector>

/**
 * @brief Container class that holds appropriate rendering jobs
 * * It can execute all the rendering jobs
*/
class RenderQueue
{
public:
	void Accept( RenderJob job ) noexcept { jobs.push_back( job ); }
	void Reset() noexcept { jobs.clear(); }
	void Execute( Graphics& gfx ) const IFNOEXCEPT;	

private:
	std::vector<RenderJob> jobs;
};

