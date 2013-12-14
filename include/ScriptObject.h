#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H

namespace dscript {
	
	template<typename Base>
	class InheritanceHider {
	private:
		Base* object;
	public:
		inline InheritanceHider() { object = nullptr; }
		inline InheritanceHider(const InheritanceHider& other) {
			if(other.object) {
				object = other.object->clone();
			} else {
				object = nullptr;
			}
		}
		inline InheritanceHider(InheritanceHider&& other) {
			object = other.object;
			other.object = nullptr;
		}
		inline InheritanceHider(const Base& b) { 
			object = b.clone();
		}
		inline ~InheritanceHider() {
			if(object) {
				delete object;
			}
		}
		inline InheritanceHider& operator=(const InheritanceHider& other) {
			if(this != &other) {
				if(object) {
					delete object;
				}
				if(other.object) {
					object = other.object->clone();
				} else {
					object = nullptr;
				}
			}
			return *this;
		}
		inline Base* getWrapped() {
			return object;
		}
	};
	
	class ObjectBase {
	public:
		ObjectBase();
		virtual ~ObjectBase();
		virtual Type* getType() const =0;
		virtual ObjectBase* clone() const =0;
	};
	
	typedef InheritanceHider<ObjectBase> ScriptObject;
	
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