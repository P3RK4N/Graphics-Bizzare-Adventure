#pragma once
#include "ApplicationComponent.h"

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX::PackedVector;
using namespace DirectX;

namespace Engine
{
	class Light : public ApplicationComponent
	{
		RTTI_DECLARATIONS(Light, ApplicationComponent)

	public:
		Light(const XMCOLOR& color) 
			: m_Color(color) {}
		virtual ~Light() {}

		const XMCOLOR& getColor() const { return m_Color; }
		void setColor(XMCOLOR color) { m_Color = color; }
		const XMVECTOR& getColorVector() const { return XMLoadColor(&m_Color); }

	private:
		XMCOLOR m_Color{};
	};
}
