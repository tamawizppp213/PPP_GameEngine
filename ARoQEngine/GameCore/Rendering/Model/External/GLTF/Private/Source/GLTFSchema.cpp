//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFSchema.cpp
///             @brief  GLTF Schema
///             @author // Copyright (c) Microsoft Corporation. All rights reserved. Licensed under the MIT License.
///             @date   2022_05_22
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchema.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchemaValidation.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExceptions.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchemaJson.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::define;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
    const std::unordered_map<std::string, SchemaFlags> schemaFlagMap = {
        { SCHEMA_URI_GLTF, SchemaFlags::DisableSchemaRoot },
        { SCHEMA_URI_GLTFID, SchemaFlags::DisableSchemaId },
        { SCHEMA_URI_GLTFCHILDOFROOTPROPERTY, SchemaFlags::DisableSchemaChildOfRoot },
        { SCHEMA_URI_GLTFPROPERTY, SchemaFlags::DisableSchemaProperty },
        { SCHEMA_URI_BUFFER, SchemaFlags::DisableSchemaBuffer },
        { SCHEMA_URI_BUFFERVIEW, SchemaFlags::DisableSchemaBufferView },
        { SCHEMA_URI_ACCESSOR, SchemaFlags::DisableSchemaAccessor },
        { SCHEMA_URI_ACCESSORSPARSE, SchemaFlags::DisableSchemaAccessorSparse },
        { SCHEMA_URI_ACCESSORSPARSEVALUES, SchemaFlags::DisableSchemaAccessorSparseValues },
        { SCHEMA_URI_ACCESSORSPARSEINDICES, SchemaFlags::DisableSchemaAccessorSparseIndices },
        { SCHEMA_URI_ASSET, SchemaFlags::DisableSchemaAsset },
        { SCHEMA_URI_SCENE, SchemaFlags::DisableSchemaScene },
        { SCHEMA_URI_NODE, SchemaFlags::DisableSchemaNode },
        { SCHEMA_URI_MESH, SchemaFlags::DisableSchemaMesh },
        { SCHEMA_URI_MESHPRIMITIVE, SchemaFlags::DisableSchemaMeshPrimitive },
        { SCHEMA_URI_SKIN, SchemaFlags::DisableSchemaSkin },
        { SCHEMA_URI_CAMERA, SchemaFlags::DisableSchemaCamera },
        { SCHEMA_URI_CAMERAORTHOGRAPHIC, SchemaFlags::DisableSchemaCameraOrthographic },
        { SCHEMA_URI_CAMERAPERSPECTIVE, SchemaFlags::DisableSchemaCameraPerspective },
        { SCHEMA_URI_MATERIAL, SchemaFlags::DisableSchemaMaterial },
        { SCHEMA_URI_MATERIALNORMALTEXTUREINFO, SchemaFlags::DisableSchemaMaterialNormalTextureInfo },
        { SCHEMA_URI_MATERIALOCCLUSIONTEXTUREINFO, SchemaFlags::DisableSchemaMaterialOcclusionTextureInfo },
        { SCHEMA_URI_MATERIALPBRMETALLICROUGHNESS, SchemaFlags::DisableSchemaMaterialPBRMetallicRoughness },
        { SCHEMA_URI_TEXTURE, SchemaFlags::DisableSchemaTexture },
        { SCHEMA_URI_TEXTUREINFO, SchemaFlags::DisableSchemaTextureInfo },
        { SCHEMA_URI_IMAGE, SchemaFlags::DisableSchemaImage },
        { SCHEMA_URI_SAMPLER, SchemaFlags::DisableSchemaSampler },
        { SCHEMA_URI_ANIMATION, SchemaFlags::DisableSchemaAnimation },
        { SCHEMA_URI_ANIMATIONSAMPLER, SchemaFlags::DisableSchemaAnimationSampler },
        { SCHEMA_URI_ANIMATIONCHANNEL, SchemaFlags::DisableSchemaAnimationChannel },
        { SCHEMA_URI_ANIMATIONCHANNELTARGET, SchemaFlags::DisableSchemaAnimationChannelTarget },
        { SCHEMA_URI_EXTENSION, SchemaFlags::DisableSchemaExtension },
        { SCHEMA_URI_EXTRAS, SchemaFlags::DisableSchemaExtras }
    };

    bool HasSchemaFlag(SchemaFlags flag, SchemaFlags flags)
    {
        return (flags & flag) == flag;
    }

    class DefaultSchemaLocator : public detail::ISchemaLocator
    {
    public:
        DefaultSchemaLocator(SchemaFlags schemaFlags) : schemaFlags(schemaFlags)
        {
        }

        const char* GetSchemaContent(const std::string& uri) const override
        {
            auto itFlag = schemaFlagMap.find(uri);

            // If the schema for this uri is disabled via the relevant SchemaFlags enum value then return an
            // empty object as empty objects are a completely valid schema that will accept any valid JSON.
            if (itFlag != schemaFlagMap.end() && HasSchemaFlag(itFlag->second, schemaFlags))
            {
                return "{}";
            }

            auto itSchema = detail::define::SchemaJson::GLTF_SCHEMA_MAP.find(uri);

            if (itSchema == SchemaJson::GLTF_SCHEMA_MAP.end())
            {
                throw detail::error::GLTFException("Unknown Schema uri " + uri);
            }

            return itSchema->second.c_str();
        }

    private:
        const SchemaFlags schemaFlags;
    };
}

// Microsoft::glTF namespace function definitions

const std::unordered_map<std::string, std::string>& gltf::detail::define::GetDefaultSchemaUriMap()
{
    return SchemaJson::GLTF_SCHEMA_MAP;
}

std::unique_ptr < const class gltf::detail::ISchemaLocator> gltf::detail::define::GetDefaultSchemaLocator(SchemaFlags schemaFlags)
{
    return std::make_unique<const DefaultSchemaLocator>(schemaFlags);
}

// SchemaFlags operator definitions

SchemaFlags gltf::detail::define::operator|(SchemaFlags lhs, SchemaFlags rhs)
{
    const auto result =
        static_cast<std::underlying_type_t<SchemaFlags>>(lhs) |
        static_cast<std::underlying_type_t<SchemaFlags>>(rhs);

    return static_cast<SchemaFlags>(result);
}

SchemaFlags& gltf::detail::define::operator|=(SchemaFlags& lhs, SchemaFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

SchemaFlags gltf::detail::define::operator&(SchemaFlags lhs, SchemaFlags rhs)
{
    const auto result =
        static_cast<std::underlying_type_t<SchemaFlags>>(lhs) &
        static_cast<std::underlying_type_t<SchemaFlags>>(rhs);

    return static_cast<SchemaFlags>(result);
}

SchemaFlags& gltf::detail::define::operator&=(SchemaFlags& lhs, SchemaFlags rhs)
{
    lhs = lhs & rhs;
    return lhs;
}