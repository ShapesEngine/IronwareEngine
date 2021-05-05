/*!
 * \file ModelProbe.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

class ModelProbe
{
public:
	virtual bool PushNode( class Node& node ) = 0;
	virtual void PopNode( class Node& node ) = 0;
};