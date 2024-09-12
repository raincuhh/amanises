#pragma once


namespace amanises
{
	class BaseExprAst
	{
	public:
		virtual ~BaseExprAst() = default;

	};

	class NumExprAst : public BaseExprAst
	{
	public:
		NumExprAst(double val);

	private:
		double val;
	};


}
