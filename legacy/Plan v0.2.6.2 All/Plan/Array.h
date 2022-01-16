#ifndef __ARRAY_H__
#define __ARRAY_H__

template<typename T>
class CSimpleValArrayR : public CSimpleValArray<T>
{
public:
	T& operator[] (int nIndex) const
	{
		return CSimpleArray<T>::operator[](nIndex);
	}
};

#endif // __ARRAY_H__