//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFExceptions.hpp
///             @brief  Exception 
///             @author // Copyright (c) Microsoft Corporation. All rights reserved.
//                       Licensed under the MIT License.
///             @date   2022_05_14  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_EXCEPTIONS_HPP
#define GLTF_EXCEPTIONS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <stdexcept>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		namespace error
		{
            // Base class for all glTF SDK exceptions
            class GLTFException : public std::runtime_error
            {
            public:
                GLTFException(const std::string& msg) : std::runtime_error(msg) {}
            };

            // An operation was attempted that would cause a spec violation
            class InvalidGLTFException : public GLTFException
            {
            public:
                InvalidGLTFException(const std::string& msg) : GLTFException(msg) {}
            };

            // The glTF::Document is in a bad state
            class DocumentException : public GLTFException
            {
            public:
                DocumentException(const std::string& msg) : GLTFException(msg) {}
            };

            // The glTF failed validation
            class ValidationException : public GLTFException
            {
            public:
                ValidationException(const std::string& msg) : GLTFException(msg) {}
            };
		}
	}
}
#endif