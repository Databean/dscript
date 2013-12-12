#include "Include.h"
#include "Debug.h"

namespace dscript {
	BinarySetOperator::BinarySetOperator(VarExpr* set, Expression* equalTo) {
		this->set = set;
		this->equalTo = equalTo;
		
		set->setParent(this);
		equalTo->setParent(this);
	}
	BinarySetOperator::~BinarySetOperator() {
		if(set) { delete set; }
		if(equalTo) { delete equalTo; }
	}
	ScriptObject BinarySetOperator::evaluate(Scope* scope) {
		ScriptObject so = equalTo->evaluate(scope);
		set->setValue(so,scope);
		return so;
	}
	bool BinarySetOperator::verify() {
		if(!(set->verify())) {
			std::cout << "left side of set expr invalid" << std::endl;
			return false;
		}
		if(!(equalTo->verify())) {
			std::cout << "right side of set expr invalid" << std::endl;
			return false;
		}
		if(!(set->getType().equals(&equalTo->getType()))) {
			std::cout << "type of set expression not equal to type of variable" << std::endl;
			return false;
		}
		return true;
	}
	Type& BinarySetOperator::getType() {
		return set->getType();
	}
	
	UnarySetOperator::UnarySetOperator(VarExpr* set,std::string op) {
		initialize(set,op,false);
	}
	UnarySetOperator::UnarySetOperator(std::string op,VarExpr* set) {
		initialize(set,op,true);
	}
	void UnarySetOperator::initialize(VarExpr* set,std::string op,bool opFirst) {
		this->set = set;
		set->setParent(this);
		this->op = op;
		this->opFirst = opFirst;
	}
	UnarySetOperator::~UnarySetOperator() {
		delete set;
	}
	Type& UnarySetOperator::getType() {
		return set->getType();
	}
	bool UnarySetOperator::verify() {
		if(!(set->verify())) {
			std::cout << "unary set expr invalid" << std::endl;
			return false;
		}
		Type& t = set->getType();
		CPPType<int>* intType = dynamic_cast<CPPType<int>*>(&t);
		CPPType<float>* realType = dynamic_cast<CPPType<float>*>(&t);
		if(!intType && !realType) {
			std::cout << "unary set expr type is not Int or Real" << std::endl;
			return false;
		}
		return true;
	}
	ScriptObject UnarySetOperator::evaluate(Scope* scope) {
		ScriptObject startVal = set->evaluate(scope);
		
		ScriptReal* sr = dynamic_cast<ScriptReal*>(startVal.getWrapped());
		ScriptInt* si = dynamic_cast<ScriptInt*>(startVal.getWrapped());
		
		ScriptObject newVal;
		ScriptObject ret;
		
		int mod = op == "++" ? 1 : -1;
		
		if(sr) {
			newVal = ScriptObject(ScriptReal(sr->getValue()+mod));
		} else if(si) {
			newVal = ScriptObject(ScriptInt(si->getValue()+mod));
		} else {
			std::cout << "wtf" << std::endl;
		}
		
		set->setValue(newVal,scope);
		if(opFirst) {
			return newVal;
		} else {
			return startVal;
		}
	}
	
	BinaryOperatorType::BinaryOperatorType(Type* left,std::string op,Type* right) {
		this->left = left;
		this->op = op;
		this->right = right;
		name = left->getName() + op + right->getName();
	}
	BinaryOperatorType::~BinaryOperatorType() {
		
	}
	bool BinaryOperatorType::operator<(const BinaryOperatorType& bot) const {
		return name < bot.name;
	}
	bool BinaryOperatorType::operator==(const BinaryOperatorType& bot) const {
		return name == bot.name;
	}
	
	BinaryOperatorFunc::BinaryOperatorFunc() {
		
	}
	BinaryOperatorFunc::~BinaryOperatorFunc() {
		
	}
	
	BinaryOperator::BinaryOperator(Expression* left,std::string op,Expression* right) {
		this->left = left;
		this->op = op;
		this->right = right;
		
		left->setParent(this);
		right->setParent(this);
	}
	BinaryOperator::~BinaryOperator() {
		delete left;
		delete right;
	}
	bool BinaryOperator::verify() {
		if(!(left->verify() && right->verify())) {
			std::cout << "left or right verify failed" << std::endl;
			return false;
		}
		opObj = getDScriptEngine()->getBinaryOperator(BinaryOperatorType(&left->getType(),op,&right->getType()));
		if(!opObj) {
			std::cout << "There is no operator " << left->getType().getName() << " " << op << right->getType().getName() << std::endl;
		}
		return true;
	}
	ScriptObject BinaryOperator::evaluate(Scope* s) {
		return opObj->evaluate(left->evaluate(s),right->evaluate(s));
	}
	Type& BinaryOperator::getType() {
		return opObj->getType();
	}
	
	UnaryOperatorType::UnaryOperatorType(std::string op,Type* right) {
		this->op = op;
		this->right = right;
		name = op + right->getName();
	}
	UnaryOperatorType::~UnaryOperatorType() {
		
	}
	bool UnaryOperatorType::operator<(const UnaryOperatorType& uot) const {
		return name < uot.name;
	}
	bool UnaryOperatorType::operator==(const UnaryOperatorType& uot) const {
		return name == uot.name;
	}
	
	UnaryOperatorFunc::UnaryOperatorFunc() {
		
	}
	UnaryOperatorFunc::~UnaryOperatorFunc() {
		
	}
	
	UnaryOperator::UnaryOperator(std::string op,Expression* right) {
		this->op = op;
		this->right = right;
		
		right->setParent(this);
	}
	UnaryOperator::~UnaryOperator() {
		delete right;
	}
	bool UnaryOperator::verify() {
		if(!(right->verify())) {
			return false;
		}
		opObj = getDScriptEngine()->getUnaryOperator(UnaryOperatorType(op,&right->getType()));
		if(!opObj) {
			std::cout << "There is no operator " << op << right->getType().getName() << std::endl;
			return false;
		}
		return true;
	}
	ScriptObject UnaryOperator::evaluate(Scope* s) {
		return opObj->evaluate(right->evaluate(s));
	}
	Type& UnaryOperator::getType() {
		return opObj->getType();
	}
	
	//makeBinaryOperatorClass(logicalXor,Bool,^^,Bool,Bool)
	
	
	
}