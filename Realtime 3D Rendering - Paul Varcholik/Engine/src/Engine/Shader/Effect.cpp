#include "pch.h"
#include "Effect.h"

#include "Engine/Core/Application.h"

#include <fstream>
#include <iterator>

#include <d3dcompiler.h>

namespace Engine
{
	static HRESULT hr;

	namespace Utility
	{
		void loadBinaryFile(const std::wstring& fileName, std::vector<char>& compiledShader)
		{
			std::ifstream is(fileName, std::ios::binary);
			is.unsetf(std::ios::skipws);

			std::streampos fileSize;

			is.seekg(0, std::ios::end);
			fileSize = is.tellg();
			is.seekg(0, std::ios::beg);

			compiledShader.reserve((UINT)fileSize);

			std::istream_iterator<char> beg(is);
			std::istream_iterator<char> end;

			while(beg != end) 
			{
				compiledShader.emplace_back(*beg);
				beg++;
			}
		}
	}

	Effect::~Effect()
	{
		for(auto& technique : m_Techniques)
			DeleteObject(technique);

		for(auto& variable : m_Variables)
			DeleteObject(variable);

		ReleaseObject(m_Effect);
	}

	void Effect::compileEffectFromFile(ID3D11Device* device, ID3DX11Effect** effect, const std::wstring& fileName)
	{
		UINT shaderFlags = 0;

#ifdef DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* errorMessage = nullptr;

		hr = D3DCompileFromFile
		(
			fileName.c_str(),
			nullptr,
			nullptr,
			nullptr,
			"fx_5_0",
			shaderFlags,
			0,
			&compiledShader,
			&errorMessage
		);

		if (errorMessage != nullptr)
		{
			ApplicationException* err = new ApplicationException((char*)errorMessage->GetBufferPointer(), hr);
			ReleaseObject(errorMessage);
			throw err;
		} else if(FAILED(hr)) throw new ApplicationException("D3DCompileFromFile() error", hr);

		if(FAILED(hr = D3DX11CreateEffectFromMemory
		(
			compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			device,
			effect
		))) throw new ApplicationException("D3DX11CreateEffectFromMemory() error", hr);

		ReleaseObject(compiledShader);
	}

	void Effect::loadCompiledEffect(ID3D11Device* device, ID3DX11Effect** effect, const std::wstring& fileName)
	{
		std::vector<char> compiledShader;
		Utility::loadBinaryFile(fileName, compiledShader);

		if(FAILED(hr = D3DX11CreateEffectFromMemory
		(
			&compiledShader.front(),
			compiledShader.size(),
			NULL,
			device,
			effect
		))) throw new ApplicationException("D3DX11CreateEffectFromMemory", hr);
	}

	void Effect::compileFromFile(const std::wstring& fileName)
	{
		compileEffectFromFile(m_Application->getDirect3DDevice(), &m_Effect, fileName);
		initialize();
	}

	void Effect::loadCompiledEffect(const std::wstring& fileName)
	{
		loadCompiledEffect(m_Application->getDirect3DDevice(), &m_Effect, fileName);
		initialize();
	}

	void Effect::initialize()
	{
		if(FAILED(hr = m_Effect->GetDesc(&m_EffectDesc)))
			throw new ApplicationException();

		for(UINT i = 0; i < m_EffectDesc.Techniques; i++)
		{
			Technique* tech = new Technique(m_Application, this, m_Effect->GetTechniqueByIndex(i));
			m_Techniques.push_back(tech);
			m_TechniquesByName.insert(std::pair<std::string, Technique*>(tech->getName(), tech));
		}

		for(UINT i = 0; i < m_EffectDesc.GlobalVariables; i++)
		{
			Variable* var = new Variable(this, m_Effect->GetVariableByIndex(i));
			m_Variables.push_back(var);
			m_VariablesByName.insert(std::pair<std::string, Variable*>(var->getName(), var));
		}
	}
}