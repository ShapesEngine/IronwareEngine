/*!
 * \file CommonMacros.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 * 
 */
#pragma once

// conditional noexception qualifier 
#define IFNOEXCEPT noexcept( !IS_DEBUG )

// parameter pack
#define TPACK typename...