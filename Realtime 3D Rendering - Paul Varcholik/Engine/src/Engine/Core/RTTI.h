#pragma once
#include <string>

namespace Engine
{
	class RTTI
	{
	public:
		virtual const unsigned int& typeIdInstance() const = 0;
		virtual RTTI* queryInterface(const unsigned id) const { return nullptr; }
		virtual bool is(const unsigned int id) const { return false; }
		virtual bool is(const std::string& name) const { return false; }
		
		template<typename T> 
		T* as() const 
		{
			if(is(T::typeIdClass())) return (T*)this;
			return nullptr;
		}

#define RTTI_DECLARATIONS(type, parentType)														\
		public:																					\
			typedef parentType parent;															\
			static std::string typeName() { return std::string(#type); }						\
			virtual const unsigned int& typeIdInstance() const { return type::typeIdClass(); }	\
			static const unsigned int& typeIdClass() { return s_RunTimeTypeId; }				\
			virtual Engine::RTTI* queryInterface(const unsigned int id) const					\
			{																					\
				if(id == s_RunTimeTypeId) { return (RTTI*)this; }								\
				return parent::queryInterface(id);												\
			}																					\
			virtual bool is(const unsigned int id) const										\
			{																					\
				if(id == s_RunTimeTypeId) { return true; }										\
				return parent::is(id);															\
			}																					\
			virtual bool is(const std::string& name) const										\
			{																					\
				if(name == typeName()) { return true; }											\
				return parent::is(name);														\
			}																					\
		private:																				\
			static unsigned int s_RunTimeTypeId;

#define RTTI_DEFINITIONS(type) unsigned int type::s_RunTimeTypeId = (unsigned int)& type::s_RunTimeTypeId;
		
	};
}
