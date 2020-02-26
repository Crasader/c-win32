#include "Node.h"
#include <cfloat>
using namespace Dio;

double Dio::NumberNode::Calc() const
{
	return _num;
}

double Dio::DivideNode::Calc() const
{
	return _left->Calc() / _right->Calc();
}

double Dio::AddNode::Calc() const
{
	return _left->Calc()+_right->Calc();
}

double Dio::SubNode::Calc() const
{
	return _left->Calc()-_right->Calc();
}

double Dio::MultiplyNode::Calc() const
{
	return _left->Calc()*_right->Calc();
}

double Dio::MinusNode::Calc() const
{
	return 0-_child->Calc();
}
