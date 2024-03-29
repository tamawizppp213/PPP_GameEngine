////////////////////////////////////////////////////////////////////////////////////
////              @file   ResourceManager.cpp
/////             @brief  ResourceManager
/////             @author Toide Yutaro
/////             @date   2022/03/13
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
////                             Include
////////////////////////////////////////////////////////////////////////////////////
//#include "GameCore/Core/Include/ResourceManager.hpp"
//#include "GameCore/Audio/Core/Include/AudioClip.hpp"
//#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Texture.hpp"
//#include <unordered_map>
////////////////////////////////////////////////////////////////////////////////////
////                              Define
////////////////////////////////////////////////////////////////////////////////////
//using namespace gc::core;
//using namespace gc::audio;
////////////////////////////////////////////////////////////////////////////////////
////                          Implement
////////////////////////////////////////////////////////////////////////////////////
//struct ResourceManager::Implementation
//{
//public:
//	/****************************************************************************
//	**                Public Function
//	*****************************************************************************/
//	/*-------------------------------------------------------------------
//	-                        Exists
//	---------------------------------------------------------------------*/
//	bool ExistsAudioClip(const std::wstring& name) { return AudioTable.find(name) != AudioTable.end(); }
//	
//	/*-------------------------------------------------------------------
//	-                        Clear
//	---------------------------------------------------------------------*/
//	inline void ClearAudioTable() { AudioTable.clear(); }
//	inline void ClearTexture() { TextureManager::ClearTextureTable(); }
//	void ClearAllResources();
//	/*-------------------------------------------------------------------
//	-                        Get
//	---------------------------------------------------------------------*/
//	inline AudioClip& GetAudioClip(const std::wstring& name) { return AudioTable[name]; }
//	inline const Texture& GetTexture(const std::wstring& name, TextureType type = TextureType::Texture2D) { return TextureManager::LoadTexture(name, type); }
//	Implementation();
//	~Implementation();
//private:
//	/****************************************************************************
//	**                Private Member Variables
//	*****************************************************************************/
//	std::unordered_map<std::wstring, AudioClip> AudioTable;
//
//};
//ResourceManager::Implementation::Implementation()
//{
//
//}
//ResourceManager::Implementation::~Implementation()
//{
//
//}
//void ResourceManager::Implementation::ClearAllResources()
//{
//	ClearAudioTable();
//	ClearTexture();
//}
//
//#pragma region Public Function
//ResourceManager::ResourceManager()
//{
//	_implementation = std::make_unique<Implementation>();
//}
//ResourceManager::~ResourceManager()
//{
//
//}
//const Texture& ResourceManager::LoadTexture(const std::wstring& name, TextureType type)
//{
//	return TextureManager::LoadTexture(name, type);
//}
//
//#pragma region Exists
///****************************************************************************
//*                       ExistsAudioClip
//*************************************************************************//**
//*  @fn        bool ResourceManager::ExistsAudioClip(const std::wstring& name)
//*  @brief     Exists audio clip data
//*  @param[in] std::wstring name
//*  @return 　　bool
//*****************************************************************************/
//bool ResourceManager::ExistsAudioClip(const std::wstring& name)
//{
//	return _implementation->ExistsAudioClip(name);
//}
//#pragma endregion Exists
//#pragma region Clear
///****************************************************************************
//*                       ClearAllResources
//*************************************************************************//**
//*  @fn        void ResourceManager::ClearAllResources()
//*  @brief     Clear all resources
//*  @param[in] void
//*  @return 　　void
//*****************************************************************************/
//void ResourceManager::ClearAllResources()
//{
//	_implementation->ClearAllResources();
//}
///****************************************************************************
//*                       ClearAudioTable
//*************************************************************************//**
//*  @fn        void ResourceManager::ClearAudioTable()
//*  @brief     Clear audio table
//*  @param[in] void
//*  @return 　　void
//*****************************************************************************/
//void ResourceManager::ClearAudioTable()
//{
//	_implementation->ClearAudioTable();
//}
//#pragma endregion Clear
//#pragma region Property
///****************************************************************************
//*                       GetAudioClip
//*************************************************************************//**
//*  @fn        AudioClip& ResourceManager::GetAudioClip(const std::wstring& name)
//*  @brief     Get audio clip
//*  @param[in] void
//*  @return 　　void
//*****************************************************************************/
//AudioClip& ResourceManager::GetAudioClip(const std::wstring& name)
//{
//	return _implementation->GetAudioClip(name);
//}
//#pragma endregion Property
//
//#pragma endregion Public Function