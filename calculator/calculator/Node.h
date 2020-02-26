#ifndef NODE_H_
#define NODE_H_
#include<iostream>
#include <vector>
namespace Dio {

	class NonCopyable
	{
	protected:
		NonCopyable() {}
	private:
		NonCopyable(const NonCopyable&) = delete;
		const NonCopyable& operator=(const NonCopyable&) = delete;

	};
	//对接口进行继承的话，是用private继承
	//不管哪个类，如果想实现对象语义，只要进行private继承就行了
	//智能指针，也可以实现对象语义，还能支持拷贝	--》释放时机开始不同	计数
	class Node :private NonCopyable
	{
	public:
		virtual ~Node() {}
		virtual double Calc() const = 0;

	};

	class NumberNode :public Node
	{
	protected:
		double _num;
	public:
		NumberNode(const double tmp) :_num(tmp) {}
		~NumberNode() {}
		double Calc() const override;		//加上override是提醒程序员重写的
	};

	class MultiNode :public Node
	{
	public:
		MultiNode(Node* child)
		{
			AppendChild(child, true);
		}
		void AppendChild(Node* child, bool positive)
		{
			_childs.push_back(child);
			_positives.push_back(positive);
		}
		~MultiNode()
		{
			for (auto child : _childs)
			{
				delete child;
			}
		}
	protected:
		std::vector<Node*> _childs;
		std::vector<bool> _positives;

	};

	class SumNode :public MultiNode
	{
	public:
		explicit SumNode(Node* child) :MultiNode(child) {}
		double Calc() const
		{
			auto positive = _positives.begin();
			double res = 0.0;
			for (auto child : _childs)
			{
				if (*positive)
				{
					res += child->Calc();
				}
				else
				{
					res -= child->Calc();
				}
				++positive;
			}
			return res;
		}
	};

	class CalcNode :public MultiNode
	{
	public:
		explicit CalcNode(Node* child) :MultiNode(child) {}
		double Calc() const
		{
			auto positive = _positives.begin();
			double res = 1.0;
			for (auto child : _childs)
			{
				if (*positive)
				{
					double tmp = res;
					res = tmp*child->Calc();
				}
				else
				{
					double divisor = child->Calc();
					if (divisor <-DBL_EPSILON || divisor >DBL_EPSILON)
					{
						divisor = res / child->Calc();
						res = divisor;
					}
					else
					{
						std::cout << "\n除数不能为零" << std::endl;
					}
					
				}
				++positive;
			}
			return res;
		}

	};

	class BinaryNode :public Node
	{
	public:
		BinaryNode(Node* left, Node* right) :_left(left), _right(right) {}	//涉及到面向对象和使用对象		
																			//面向对象	对象语义	//只有一个对象，不支持拷贝		只能使用指针		最后的生命周期由自己管理	起到限制作用
																			//使用对象	值语义		//深拷贝，拷贝后和原对象无关

		~BinaryNode()
		{
			delete _left;
			delete _right;
		}
	protected:
		Node* _left;
		Node* _right;

	};

	class DivideNode :public BinaryNode
	{
	public:
		DivideNode(Node* left, Node* right) :BinaryNode(left, right) {}	//在初始化列表中可以调用父类的构造方法
		double Calc() const override;
	};

	class AddNode :public BinaryNode
	{
	public:
		AddNode(Node* left, Node* right) :BinaryNode(left, right) {}
		double Calc() const override;

	};

	class SubNode :public BinaryNode
	{
	public:
		SubNode(Node* left, Node* right) :BinaryNode(left, right) {}
		double Calc() const override;


	};

	class MultiplyNode :public BinaryNode
	{
	public:
		MultiplyNode(Node* left, Node* right) :BinaryNode(left, right) {}
		double Calc() const override;

	};

	class UnitaryNode :public Node
	{
	public:
		UnitaryNode(Node* child) :_child(child) {}
		~UnitaryNode() { delete _child; }
	protected:
		Node* _child;
	};

	class MinusNode :public UnitaryNode
	{
	public:
		MinusNode(Node* child) :UnitaryNode(child) {}
		double Calc() const override;
	};

}
#endif

