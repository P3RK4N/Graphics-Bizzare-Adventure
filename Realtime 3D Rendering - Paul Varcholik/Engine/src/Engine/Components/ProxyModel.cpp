#include "pch.h"
#include "ProxyModel.h"

//#include "RasterizerStates.h
#include "Engine/Core/Mesh.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Engine
{
	RTTI_DEFINITIONS(ProxyModel)

	ProxyModel::ProxyModel(Application& game, const shared_ptr<Camera>& camera, const std::string& modelFileName, float scale) :
		DrawableApplicationComponent(game, camera),
		mModelFileName(modelFileName), mScale(scale),
		mMaterial(game)
	{
	}

	const XMFLOAT3& ProxyModel::Position() const
	{
		return m_Position;
	}

	const XMFLOAT3& ProxyModel::Direction() const
	{
		return m_Direction;
	}

	const XMFLOAT3& ProxyModel::Up() const
	{
		return m_Up;
	}

	const XMFLOAT3& ProxyModel::Right() const
	{
		return m_Right;
	}

	XMVECTOR ProxyModel::PositionVector() const
	{
		return XMLoadFloat3(&m_Position);
	}

	XMVECTOR ProxyModel::DirectionVector() const
	{
		return XMLoadFloat3(&m_Direction);
	}

	XMVECTOR ProxyModel::UpVector() const
	{
		return XMLoadFloat3(&m_Up);
	}

	XMVECTOR ProxyModel::RightVector() const
	{
		return XMLoadFloat3(&m_Right);
	}

	bool& ProxyModel::DisplayWireframe()
	{
		return m_DisplayWireframe;
	}

	void ProxyModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void ProxyModel::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&m_Position, position);
		m_UpdateWorldMatrix = true;
	}

	void ProxyModel::SetPosition(const XMFLOAT3& position)
	{
		m_Position = position;
		m_UpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&m_Direction);
		XMVECTOR up = XMLoadFloat3(&m_Up);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&m_Direction, direction);
		XMStoreFloat3(&m_Up, up);
		XMStoreFloat3(&m_Right, right);
		
		m_UpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void ProxyModel::SetColor(const DirectX::XMFLOAT4& color)
	{
		m_Material.SetSurfaceColor(color);
	}

	void ProxyModel::initialize()
	{
		const auto model = m_Application->Content().Load<Model>(Utility::ToWideString(mModelFileName));
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPosition::CreateVertexBuffer(mApplication->Direct3DDevice(), *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*mApplication->Direct3DDevice(), not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		mMaterial.Initialize();

		auto updateMaterialFunc = [this]() { mUpdateWorldMatrix = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);
	}

	void ProxyModel::Update(const ApplicationTime&)
	{
		if (mUpdateWorldMatrix)
		{
			XMMATRIX worldMatrix = XMMatrixIdentity();
			MatrixHelper::SetForward(worldMatrix, mDirection);
			MatrixHelper::SetUp(worldMatrix, mUp);
			MatrixHelper::SetRight(worldMatrix, mRight);
			MatrixHelper::SetTranslation(worldMatrix, mPosition);

			XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * worldMatrix);
			mUpdateWorldMatrix = false;
			mUpdateMaterial = true;
		}
	}

	void ProxyModel::draw(const ApplicationTime&)
	{
		if (m_UpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * m_Camera->ViewProjectionMatrix());
			m_Material.UpdateTransform(wvp);
			m_UpdateMaterial = false;
		}

		if (m_DisplayWireframe)
		{
			m_Application->getDirect3DDeviceContext()->RSSetState(RasterizerStates::Wireframe.get());
			m_Material.drawIndexed(not_null<ID3D11Buffer*>(m_VertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
			m_Application->getDirect3DDeviceContext()->RSSetState(nullptr);
		}
		else
		{
			mMaterial.DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
		}
	}
}