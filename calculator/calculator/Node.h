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
	//�Խӿڽ��м̳еĻ�������private�̳�
	//�����ĸ��࣬�����ʵ�ֶ������壬ֻҪ����private�̳о�����
	//����ָ�룬Ҳ����ʵ�ֶ������壬����֧�ֿ���	--���ͷ�ʱ����ʼ��ͬ	����
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
		double Calc() const override;		//����override�����ѳ���Ա��д��
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
						std::cout << "\n��������Ϊ��" << std::endl;
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
		BinaryNode(Node* left, Node* right) :_left(left), _right(right) {}	//�漰����������ʹ�ö���		
																			//�������	��������	//ֻ��һ�����󣬲�֧�ֿ���		ֻ��ʹ��ָ��		���������������Լ�����	����������
																			//ʹ�ö���	ֵ����		//������������ԭ�����޹�

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
		DivideNode(Node* left, Node* right) :BinaryNode(left, right) {}	//�ڳ�ʼ���б��п��Ե��ø���Ĺ��췽��
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

