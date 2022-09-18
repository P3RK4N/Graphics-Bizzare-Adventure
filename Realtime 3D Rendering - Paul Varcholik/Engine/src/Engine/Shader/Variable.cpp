#include "pch.h"
#include "Variable.h"

namespace Engine
{
    static HRESULT hr;

    Variable::Variable(Effect* effect, ID3DX11EffectVariable* variable)
		: m_Effect(effect), m_Variable(variable) 
	{
        if(FAILED(hr = m_Variable->GetDesc(&m_VariableDesc)))
		    throw new ApplicationException("GetDesc() error", hr);

        m_Type = m_Variable->GetType();

        if(FAILED(hr = m_Type->GetDesc(&m_TypeDesc)))
		    throw new ApplicationException("GetDesc() error", hr);

        m_Name = m_VariableDesc.Name;
	}

    Variable::~Variable()
    {
        ReleaseObject(m_Variable);
        ReleaseObject(m_Type);
    }

    Variable& Variable::operator<<(CXMMATRIX value)
    {
        ID3DX11EffectMatrixVariable* variable = m_Variable->AsMatrix();
        if(!variable->IsValid()) throw new ApplicationException();
        variable->SetMatrix(reinterpret_cast<const float*>(&value));

        return *this;
    }

    Variable& Variable::operator<<(ID3D11ShaderResourceView* value)
    {
        ID3DX11EffectShaderResourceVariable* variable = m_Variable->AsShaderResource();
        if(!variable->IsValid()) throw new ApplicationException();
        variable->SetResource(value);

        return *this;
    }

    Variable& Variable::operator<<(FXMVECTOR value)
    {
        ID3DX11EffectVectorVariable* variable = m_Variable->AsVector();
        if(!variable->IsValid()) throw new ApplicationException();
        variable->SetFloatVector(reinterpret_cast<const float*>(&value));

        return *this;
    }

    Variable& Variable::operator<<(float value)
    {
        ID3DX11EffectScalarVariable* variable = m_Variable->AsScalar();
        if(!variable->IsValid()) throw new ApplicationException();
        variable->SetFloat(value);
      
        return *this;
    }

    Variable& Variable::operator<<(const std::vector<float>& values)
	{
		ID3DX11EffectScalarVariable* variable = m_Variable->AsScalar();
		if (variable->IsValid() == false)
		{
			throw ApplicationException("Invalid effect variable cast.");
		}

		variable->SetFloatArray(&values[0], 0, values.size());
	
		return *this;
	}

	Variable& Variable::operator<<(const std::vector<XMFLOAT2>& values)
	{
		ID3DX11EffectVectorVariable* variable = m_Variable->AsVector();
		if (variable->IsValid() == false)
		{
			throw ApplicationException("Invalid effect variable cast.");
		}

		variable->SetFloatVectorArray(reinterpret_cast<const float*>(&values[0]), 0, values.size());
	
		return *this;
	}

	Variable& Variable::operator<<(const std::vector<XMFLOAT4X4>& values)
	{
		ID3DX11EffectMatrixVariable* variable = m_Variable->AsMatrix();
		if (variable->IsValid() == false)
		{
			throw ApplicationException("Invalid effect variable cast.");
		}

		variable->SetMatrixArray(reinterpret_cast<const float*>(&values[0]), 0, values.size());

		return *this;
	}

}
