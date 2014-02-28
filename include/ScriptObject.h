#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H

#include <string>
#include <memory>

namespace dscript {
	
	class Type;
	
	class ObjectBase {
	public:
		ObjectBase();
		virtual ~ObjectBase();
		virtual Type* getType() const =0;
		virtual ObjectBase* clone() const =0;
	};
	
	typedef std::unique_ptr<ObjectBase> ScriptObject;
	
	template<class T>
	class CPPObjectWrapper : public ObjectBase {
	public:
		CPPObjectWrapper(const T val) : value(val) {}
		virtual ~CPPObjectWrapper() {}
		virtual Type* getType() const {
			return type;
		}
		virtual const T& getValue() const { return value; }
		virtual ObjectBase* clone() const { return new CPPObjectWrapper<T>(value); }
	private:
		static Type* type;
		T value;
	};
	
	typedef CPPObjectWrapper<int> ScriptInt;
	typedef CPPObjectWrapper<std::string> ScriptString;
	typedef CPPObjectWrapper<float> ScriptReal;
	typedef CPPObjectWrapper<bool> ScriptBool;
}



#endif