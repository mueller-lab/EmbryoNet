#pragma once

#include <vector>

//
// Linear assignment via Hungarian algorithm implementation.
// This is literal code from scikit-learn Python library translated to C++.
//
// Original code is 3-Clause BSD license based.
// URL: https://github.com/scikit-learn/scikit-learn/blob/master/sklearn/utils/linear_assignment_.py
//
// Given the cost matrix performs minimal cost linear assignment.
// The main method to use is LinearAssignment::apply. 
// The returned assignment is in a form of vector of pairs
// vector{pair[cost_matrix_row, cost_matrix_column]}.
//

class LinearAssignment
{
	std::vector<std::vector<float> > m_costMatrix;

public:

	LinearAssignment(const std::vector<std::vector<float> > &costMatrix);
	std::vector<std::pair<int, int> > apply() const;

private:

	std::vector<std::pair<int, int> > hungarian(const std::vector<std::vector<float> > &costMatrix) const;
};