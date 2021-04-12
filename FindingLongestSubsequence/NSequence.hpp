using namespace std;

template<typename T>
NSequence<T>::NSequence(int initSize)
{


	if(initSize==0)
		initSize=1;

	numOfItems = initSize;
	totalCapacity = initSize;
	items = new T[totalCapacity];

}

template<typename T>
NSequence<T>::NSequence(const NSequence & rhs)
{
	numOfItems = rhs.getSize();
	totalCapacity = rhs.getCapacity();
	items = new T[totalCapacity];

	for(int i=0; i< numOfItems; i++)
		{
		items[i] = rhs.items[i];
		}
}


template<typename T>
NSequence<T> & NSequence<T>:: operator= ( const NSequence & rhs )
{
	numOfItems = rhs.getSize();
        totalCapacity = rhs.getCapacity();
        items = new T[totalCapacity];

        for(int i=0; i< numOfItems; i++)
                {
                items[i] = rhs.items[i];
                }
}

template<typename T>
NSequence<T>::~NSequence()
{
//	delete items;
}

template<typename T>
NSequence<T>::NSequence(NSequence && rhs)
{

	 numOfItems = rhs.getSize();
        totalCapacity = rhs.getCapacity();
        items = new T[totalCapacity];

        for(int i=0; i< numOfItems; i++)
                {
                items[i] = rhs.items[i];
                }

	delete rhs.items;
	rhs.numOfItems=0;
	rhs.toalCapacity = 0;
}

template<typename T>
NSequence<T> & NSequence<T>::operator = (NSequence && rhs )
{
	numOfItems = *rhs.getSize();
        totalCapacity = rhs.getCapacity();
        items = new T[totalCapacity];

        for(int i=0; i< numOfItems; i++)
                {
                items[i] = rhs.items[i];
                }

        delete rhs.items;
        rhs.numOfItems=0;
        rhs.toalCapacity = 0;
}

template<typename T>
bool NSequence<T>::isEmpty() const
{
	if(numOfItems==0){return true;}
	else {return false;}
} 

template<typename T>
int NSequence<T>::getSize() const
{
	return numOfItems;
}

template<typename T>
int NSequence<T>::getCapacity() const
{
	return totalCapacity;
}

template<typename T>
T & NSequence<T>::operator [](int index)
{
	return items[index];
}

template<typename T>
const T & NSequence<T>::operator [](int index) const
{
	return items[index];
}

template<typename T>
T & NSequence<T>::getFirst() const
{
	return items[0];
}

template<typename T>
T & NSequence<T>::getLast() const
{
        return items[numOfItems - 1];
}

template<typename T>
void NSequence<T>::insert(const int pos, const T & d)
{
	int pos1 = pos;

	if(pos1==0)
		pos1=1;

	T *newArray = new T[totalCapacity];

        if(numOfItems+1>totalCapacity)
		{
                newArray = new T[numOfItems+1];
		totalCapacity = numOfItems+1;
        	}


        for(int i =0; i<numOfItems; i++)
                {
                if(i<pos1)
                        {
                        newArray[i]=items[i];
                        if(i==pos1-1)
                                newArray[i] = d;
                        }
                else
                        {
                        newArray[i] =  items[i-1];
                        }
                }

	numOfItems++;

        items = new T[totalCapacity];

        for(int i =0; i<totalCapacity;i++)
                items[i] = newArray[i];


}
template<typename T>
void NSequence<T>::insert(const int pos, T && d)
{
	        int pos1 = pos;

        if(pos1==0) 
                pos1=1;

        T *newArray = new T[totalCapacity];

        if(numOfItems+1>totalCapacity)
                {
                newArray = new T[numOfItems+1];
                totalCapacity = numOfItems+1;
                }


        for(int i =0; i<numOfItems; i++)
                {
                if(i<pos1)
                        {
                        newArray[i]=items[i];
                        if(i==pos1-1)
                                newArray[i] = d;
                        }
                else
                        {
                        newArray[i] =  items[i-1];
                        }
                }

        numOfItems++;

        items = new T[totalCapacity];

        for(int i =0; i<totalCapacity;i++)
                items[i] = newArray[i];
}

template<typename T>
void NSequence<T>::remove(const int pos)
{
        T *newArray = new T[totalCapacity];
        numOfItems--;

        for(int i =0; i<totalCapacity-1; i++)
		{
		if(i<pos)
                	newArray[i] = items[i];
		else
			newArray[i] =  items[i+1];
		}

        items = new T[totalCapacity];

        for(int i =0; i<numOfItems;i++)
                items[i] = newArray[i];

}

template<typename T>
void NSequence<T>::push_back(const T &x)
{
	T *newArray = new T[totalCapacity];

	if((numOfItems+1) > totalCapacity)
		{
		newArray = new T[numOfItems+1];
		totalCapacity = numOfItems+1;
		}

	for(int i=0; i<numOfItems; i++)
		{
		newArray[i] = items[i];
		}

	items = new T[totalCapacity];
	newArray[numOfItems-1] = x;

	 for(int i=0; i<=numOfItems; i++)
                items[i] = newArray[i];

	numOfItems++;

}

template<typename T>
void NSequence<T>::push_back(T &&x)
{
	 T *newArray = new T[totalCapacity];

        if((numOfItems+1) > totalCapacity)
                {
                newArray = new T[numOfItems+1];
                totalCapacity = numOfItems+1;
                }

        for(int i=0; i<numOfItems; i++)
                {
                newArray[i] = items[i];
                }

        items = new T[totalCapacity];
        newArray[numOfItems-1] = x;

         for(int i=0; i<=numOfItems; i++)
                items[i] = newArray[i];

        numOfItems++;
}
template<typename T>
void NSequence<T>::pop_back()
{

	T *newArray = new T[totalCapacity];
	numOfItems--;

	for(int i =0; i<numOfItems; i++)
		newArray[i] = items[i];

	items = new T[totalCapacity];

	for(int i =0; i<numOfItems;i++)
		items[i] = newArray[i];

}

template<typename T>
void NSequence<T>::growCapacity(int newCapacity)
{
	if(newCapacity==0)
		newCapacity = 2*totalCapacity;

	totalCapacity = newCapacity;

	T *tempItems= new T[totalCapacity];

	for(int i =0; i<numOfItems; i++)
		{
		tempItems[i] = items[i];
		}

	items = new T[totalCapacity];

	for(int i =0; i<numOfItems; i++)
                {
                items[i] = tempItems[i];
                }

	delete[] tempItems;
}


template<typename T>
void NSequence<T>::printout(int begin, int end)
{
	for(int i = begin; i<end && i<50; i++)
		{
		cout<<"pos("<<i<<"): "<<items[i]<<endl;
		}
}

