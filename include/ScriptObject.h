#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H

#define SCRIPT_TYPE_DECL(cppname,scriptname) \
	class Script##scriptname : public ObjectBase { \
	public: \
		Script##scriptname ( cppname value); \
		virtual ~Script##scriptname (); \
		virtual cppname getValue() const; \
		virtual Type* getType() const; \
		virtual ObjectBase* clone() const; \
	private: \
		cppname value; \
	}

#define SCRIPT_TYPE_IMPL(cppname,scriptname) \
	Script##scriptname  :: Script##scriptname ( cppname value) { \
		this->value = value; \
	} \
	Script##scriptname  :: ~Script##scriptname () { \
	} \
	cppname Script##scriptname ::getValue() const { \
		return value; \
	} \
	Type* Script##scriptname ::getType() const { \
		static Type* type = new SimpleType(#scriptname); \
		return type; \
	} \
	ObjectBase* Script##scriptname ::clone() const { \
		return new Script##scriptname (value); \
	}


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
		CPPObjectWrapper(const T& val) : value(val) {}
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
	typedef CPPObjectWrapper<double> ScriptReal;
	typedef CPPObjectWrapper<bool> ScriptBool;
	
	//SCRIPT_TYPE_DECL(int,Int);
	//SCRIPT_TYPE_DECL(std::string,String);
	//SCRIPT_TYPE_DECL(double,Real);
	//SCRIPT_TYPE_DECL(bool,Bool);
}

/*
namespace std {
	void swap(ScriptObject& a,ScriptObject& b) {
		ObjectBase* temp = a.object;
		a.object = b.object;
		b.object = temp;
	}
}*/


#endif