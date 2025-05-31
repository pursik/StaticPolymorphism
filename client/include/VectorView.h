#pragma once
#include <vector>

template<class T, class A>
class VectorView : public std::ranges::view_interface<VectorView<T, A>>
{
public:
	VectorView() = default;

	VectorView(const std::vector<T, A>& vec) :
		m_begin(vec.cbegin()), m_end(vec.cend()), m_next(vec.cbegin())
	{}

	auto begin() const { return m_begin; }

	auto end() const { return m_end; }

	auto next()
	{
		if (m_next != m_end)
		{
			auto current = m_next;
			++m_next;
			return current;
		}
		return m_end;
	}

private:
	typename std::vector<T, A>::const_iterator m_begin{}, m_end{}, m_next{};
};

