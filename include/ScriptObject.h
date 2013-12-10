#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H

namespace dscript {
	
	class ObjectBase {
	public:
		ObjectBase();
		virtual ~ObjectBase();
		virtual Type* getType() const =0;
		virtual ObjectBase* clone() const =0;
	};
	
	class ScriptObject {
	public:
		inline ScriptObject() { object = nullptr; }
		inline ScriptObject(const ScriptObject& other) {
			if(other.object != nullptr) {
				object = other.object->clone();
			} else {
				object = nullptr;
			}
		}
		inline ScriptObject(ScriptObject&& other) {
			object = other.object;
			other.object = nullptr;
		}
		inline ScriptObject(const ObjectBase& b) { object = b.clone(); }
		inline ~ScriptObject() { if(object) { delete object; } }
		inline const ScriptObject& operator=(const ScriptObject& other) {
			if(this != &other) {
				if(object) { delete object; }
				if(other.object != nullptr) {
					object = other.object->clone();
				} else {
					object = nullptr;
				}
			}
			return *this;
		}
		inline ObjectBase* getWrapped() { return object; }
		//friend void std::swap(ScriptObject& a,ScriptObject& b);
	private:
		ObjectBase* object;
	};
	
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