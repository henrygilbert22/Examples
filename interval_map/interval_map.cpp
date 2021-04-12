#include <map>
#include<iostream>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {


        if (!(keyBegin < keyEnd))
            return;

        typename std::map<K, V>::iterator possBegin;                                                        //Possible Begining position 
        typename std::map<K, V>::iterator possEnd;                                                          //Possible ending positon
        auto lowBegin = m_map.lower_bound(keyBegin);                                                        //Setting my bottom position 
        auto upEnd = m_map.upper_bound(keyEnd);                                                             //Setting my top positon
        auto oldEnd = std::prev(upEnd);                                                                     //Getting postion before upEnd for checking and analysis

        if (!(oldEnd->second == val))                                                                          //If position before the keyEnd is not currently the correct value
            {
            if (!(oldEnd->first < keyEnd) && !(keyEnd < oldEnd->first)) {possEnd = oldEnd;}                    //Setting possEnd to oldEnd if criteria is met
            else {possEnd = m_map.insert_or_assign(upEnd, keyEnd, oldEnd->second);}                         //Otherwise inserting the correct value and setting possEnd to said position                       
            }
        else {possEnd = upEnd;}                                                                             //If oldEnd value is the value, then possEnd will be oldEnd
                                                                                
        if (!(lowBegin == m_map.begin()))                                                                      //Checking if the lowBegin is not just the actual beggining of the map
            {
            auto oldIt = std::prev(lowBegin);                                                               //Then setting iterator to the element before lowBegin
            if (!(oldIt->second == val)) {possBegin = m_map.insert_or_assign(lowBegin, keyBegin, val);}        //If the current iterator's value is not the given value then insert accordingly
            else {possBegin = oldIt;}                                                                       // Otherwise, set possBegin to position before lowBegin
            }
        else{possBegin = m_map.insert_or_assign(lowBegin, keyBegin, val);}                                  //If lowBegin's value did equal the beggining of the map then set possBegin to the correct value inserted there
        
        auto newItBegin = std::next(possBegin);                                                             //Setting new beggining iterator to the position after currently set start
        if (!(newItBegin == m_map.end())) {m_map.erase(newItBegin, possEnd);}                                  //Erasing everything from the begining iterator to the ending iterator if the begin doesn't already equal the end
   
	}


	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};


int main()
{

    interval_map<int,char> myMap(20);
    myMap.assign(1,24,'C');

   


	return 0;
}

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.