#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}


template<int MAX_SIZE>
int HashTable<MAX_SIZE>::get_elementCount(){
    return elementCount  ;
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::get_size(){
    return MAX_SIZE ;
}


template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    int key;
    int weight = (isCostWeighted ? 1 : 0);
    key = (PRIMES[0]*startInt+ PRIMES[1]*endInt+ PRIMES[2]*weight)%(MAX_SIZE);
    return key;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    for(int i=0;i<MAX_SIZE;i++){
        elementCount=0;
        table[i].lruCounter=0;
    
    
        table[i].sentinel=EMPTY_MARK;
    
        table[i].isCostWeighted=false;
        table[i].startInt=0;
        table[i].endInt=0;
    }
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    int size=intArray.size();
    if(size==0){
        throw InvalidTableArgException();
    }
    int firstkey=Hash(intArray[0],intArray[size-1],isCostWeighted);
    int key=firstkey;
    int i=1;
    
    while(table[key].sentinel==OCCUPIED_MARK){
                if(table[key].startInt==(intArray[0])&&table[key].endInt==(intArray[size-1])&&table[key].isCostWeighted==isCostWeighted){
                    int pre=table[key].lruCounter;
                    table[key].lruCounter=table[key].lruCounter+1;
                    return pre;
                }
                key=(firstkey+(i*i))%MAX_SIZE;
                i++;

    }
    
    
    if(elementCount==(MAX_SIZE+1)/CAPACITY_THRESHOLD){
        throw TableCapFullException(elementCount);
    }
    table[key].intArray=intArray;
    table[key].lruCounter=1;
    table[key].isCostWeighted=isCostWeighted;
    table[key].startInt=intArray[0];
    table[key].endInt=intArray[size-1];
    table[key].sentinel=OCCUPIED_MARK;
    
    elementCount++;
    return 0;

    
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    int firstkey=Hash(startInt,endInt,isCostWeighted);
    int key=firstkey;
    int i=1;
    while(table[key].sentinel==OCCUPIED_MARK){
        if(table[key].startInt==startInt&&table[key].endInt==endInt&&table[key].isCostWeighted==isCostWeighted){
                if(incLRU){
                    table[key].lruCounter=table[key].lruCounter+1;
                    }
                    intArray=table[key].intArray;
                    return true;
                }
                key=(firstkey+(i*i))%MAX_SIZE;
                i++;
    }
   /*  int size=intArray.size();
    int key=Hash(startInt,endInt,isCostWeighted);
    for(int i=0;i<MAX_SIZE;i++){
        int currentkey=Hash(table[i].startInt,table[i].endInt,table[i].isCostWeighted);
        if(key==currentkey)//????
        {
            if(incLRU){
                table[i].lruCounter+=1;
            }
            return true;
            
        }
    }
    /* TO */
    return false;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
     for(int i=0;i<MAX_SIZE;i++){
        table[i].lruCounter=0;
    
    
        table[i].sentinel=EMPTY_MARK;
        table[i].isCostWeighted=false;
        table[i].startInt=0;
        table[i].endInt=0;
        table[i].intArray.clear();
    }
    elementCount=0;

}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
     
    HashData tempdata=table[0];
    HashData maxdata=tempdata;
    for(int i=1;i<MAX_SIZE;i++){
        if(table[i].lruCounter>maxdata.lruCounter){
            maxdata=table[i];
            
        }
    }
    intArray= maxdata.intArray;
    
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    
    int firstkey=Hash(startInt,endInt,isCostWeighted);
    int key=firstkey;
    int i=1;
    while(table[key].sentinel==OCCUPIED_MARK){
        if(table[key].startInt==startInt&&table[key].endInt==endInt&&table[key].isCostWeighted==isCostWeighted){
            table[key].sentinel=SENTINEL_MARK;
            table[key].isCostWeighted=false;
            table[key].startInt=0;
            table[key].endInt=0;
            table[key].lruCounter=0;
            elementCount-=1;
            intArray=table[key].intArray;
            table[key].intArray.clear();
            return;
            }
            key=(firstkey+(i*i))%MAX_SIZE;
            i++;
    }
        
    
    /* for(int i=0;i<MAX_SIZE;i++){
        if(table[i].startInt == startInt &&
            table[i].endInt == endInt &&
            table[i].isCostWeighted == isCostWeighted)//check intarray?
            {
            table[i].sentinel=SENTINEL_MARK;
            table[i].isCostWeighted=false;
            table[i].startInt=0;
            table[i].endInt=0;
            table[i].lruCounter=0;
            elementCount-=1;
            intArray=table[i].intArray;
            table[i].intArray.clear();
            return;
        
            }

        }*/
        
        return;
    

}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    if(lruElementCount>elementCount){
        return;
    }
    int count=lruElementCount;
    MinPairHeap<int,int> mypq;
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==OCCUPIED_MARK){
            Pair<int,int> newpair;
            newpair.key=table[i].lruCounter;
            newpair.value=i;
            mypq.push(newpair);
            
        }
    }
    while(count){
        int index=mypq.top().value;
        mypq.pop();
        Remove(table[index].intArray,table[index].startInt,table[index].endInt,table[index].isCostWeighted);
        count--;
    }
    
    
    
    
    /*
    HashData tempdata;
    int count=lruElementCount;
    HashData mindata;
    while(count){
        for(int j=0;j<MAX_SIZE;j++){
            if(table[j].lruCounter>0&&table[j].sentinel==OCCUPIED_MARK){
                tempdata=table[j];
                break;
            }
        }
        
        mindata=tempdata;
        for(int i=0;i<MAX_SIZE;i++){
            if(table[i].sentinel==OCCUPIED_MARK &&table[i].lruCounter<mindata.lruCounter&&table[i].lruCounter>0 ){
            mindata=table[i];
            }
        }
        mindata.sentinel=SENTINEL_MARK;
        mindata.intArray.clear();
        mindata.startInt=0;
        mindata.endInt=0;
        mindata.lruCounter=0;
        elementCount--;
        count--;
    }
    
    
    
    /* TO */
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    MaxPairHeap<int,int> mypq;
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].lruCounter>0){
            Pair<int,int> mypair;

            mypair.key=table[i].lruCounter;
            mypair.value=i;
            mypq.push(mypair);

        }
    }
    while(!mypq.empty()){
        int index=mypq.top().value;
        mypq.pop();
        PrintLine(index);
        
    }
}

#endif // HASH_TABLE_HPP