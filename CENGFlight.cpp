#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}


bool CENGFlight::containSisterHelper(const std::vector<std::string>AirlineNames,const std::string airlineName) const{
    for(int i=0;i<AirlineNames.size();i++){
        if(airlineName==AirlineNames[i]){
            return true;
        }
    }
    return false;
}


void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{

    if(!(navigationMap.search_vertex(airportFrom)) || !(navigationMap.search_vertex(airportTo))){
        PrintCanNotHalt(airportFrom,airportTo,airlineName);
        return;
    }
    int edgeindex;
    if(!(navigationMap.search_edge_v2(airportFrom,airportTo,airlineName,edgeindex))){//airportto check?
        PrintCanNotHalt(airportFrom,airportTo,airlineName);
        return;
    }
    int indexFrom=navigationMap.search_vertex_index(airportFrom);
    int indexTo=navigationMap.search_vertex_index(airportTo);
    
    HaltedFlight mytemp;
    mytemp.airportFrom=airportFrom;
    mytemp.airportTo=airportTo;
    mytemp.airline=airlineName;

    mytemp.w0=navigationMap.get_vertexlist()[indexFrom].edges[edgeindex].weight[0];
    mytemp.w1=navigationMap.get_vertexlist()[indexFrom].edges[edgeindex].weight[1];
    haltedFlights.push_back(mytemp);
    navigationMap.RemoveEdge(airlineName,airportFrom,airportTo);
    
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    if (haltedFlights.empty()) {
        PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
        return;
    }
    
    int haltedsize=haltedFlights.size();
    int i=0;
    for(;i<haltedsize;i++){
        HaltedFlight mytemp=haltedFlights[i];
        if(mytemp.airportFrom==airportFrom&&mytemp.airportTo==airportTo&&mytemp.airline==airlineName){
            break;
        }
        if(i==haltedsize-1){
            PrintCanNotResumeFlight(airportFrom,airportTo,airlineName);
            return;
        }
    }
    
    

    float weight0=haltedFlights[i].w0;
    float weight1=haltedFlights[i].w1;
    haltedFlights.erase(haltedFlights.begin()+i);

    navigationMap.AddEdge(airlineName,airportFrom,airportTo,weight0,weight1);

}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    bool myboolval=false;
    if(alpha==0.0f){
        myboolval=true;
    }
    int startindex=navigationMap.search_vertex_index(startAirportName);
    int endindex=navigationMap.search_vertex_index(endAirportName);
    std::vector<int> intArray;
    
    if(!(navigationMap.search_vertex(startAirportName))){
        PrintPathDontExist(startAirportName,endAirportName);
        return;
    }
    if(!(navigationMap.search_vertex(endAirportName))){
        PrintPathDontExist(startAirportName,endAirportName);
        return;
    }
    
    if(alpha==0.0f || alpha==1.0f){
        if(lruTable.Find(intArray,startindex,endindex,myboolval,true)){
            PrintFlightFoundInCache(startAirportName,endAirportName,myboolval);
            navigationMap.PrintPath(intArray,alpha,true);
            return;
        }

    }
    
    if(navigationMap.HeuristicShortestPath(intArray,startAirportName,endAirportName,alpha)){
        if(alpha==0.0f || alpha==1.0f){
            PrintFlightCalculated(startAirportName,endAirportName,myboolval);
        }
        navigationMap.PrintPath(intArray,alpha,true);
    }
    else{
        PrintPathDontExist(startAirportName,endAirportName);
        //pathdontexist
    }
    
    if(alpha==0 || alpha==1){
        if(lruTable.get_elementCount()==(lruTable.get_size()+1)/2){
            lruTable.RemoveLRU(1);
    }
        lruTable.Insert(intArray,myboolval);
    }

}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    bool myboolval=false;
    if(alpha==0.0){
        myboolval=true;
    }
    std::vector<int> intArray;

    
    
    if(!(navigationMap.search_vertex(startAirportName))){
        PrintPathDontExist(startAirportName,endAirportName);
    }
    if(!(navigationMap.search_vertex(endAirportName))){
        PrintPathDontExist(startAirportName,endAirportName);
    }
    
    if(navigationMap.FilteredShortestPath(intArray,startAirportName,endAirportName,alpha,unwantedAirlineNames)){
        navigationMap.PrintPath(intArray,alpha,true);
    }
    
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    if(!(navigationMap.search_vertex(airportName))){
        PrintSisterAirlinesDontCover(airportName);
    }
    std::vector<GraphVertex> VertexList=navigationMap.get_vertexlist();
    int vertexnumber=VertexList.size();
    std::vector<bool> visited(vertexnumber,false);
    std::vector<int> mystack;
    std::vector<std::string>tmpairlines;
    std::vector<std::string>realtmpairlines;//this doesn^t have startAirlineName
    tmpairlines.push_back(startAirlineName);
    int visitedcounter=0;
    int startindex=navigationMap.search_vertex_index(airportName);

    mystack.push_back(startindex);
    visited[startindex]=true;
    visitedcounter++;
    while(!mystack.empty()){//• Start visiting every airport from “airportName” using “startAirlineName” DFS
        int w=mystack.back();
        mystack.pop_back();
        for(int i=0;i<VertexList[w].edges.size();i++){
            int endvertexind=VertexList[w].edges[i].endVertexIndex;
            if(VertexList[w].edges[i].name==startAirlineName&&visited[endvertexind]==false ){
                visited[endvertexind]=true;
                visitedcounter++;
                mystack.push_back(endvertexind);
            }
        }
    }
    if(visitedcounter==1){//• If at least one other airport is reached, continue the operation if not print the error.
        PrintSisterAirlinesDontCover(airportName);
        return;
    }
    
    
    
    int previouscounter=-1;
    while(visitedcounter!=vertexnumber && visitedcounter!=previouscounter){//Until all airports are visited or your visited airports did not change from the last iteration.
        previouscounter=visitedcounter;
        int airportmaxnon;
        std::vector<int> nonvisitedcounter(vertexnumber,0);//?does it clean itself?
        std::vector<bool> myvisited(vertexnumber,false);//this vector help me to don't increase nonvisitedcounter for same endvertexindexed edges
        for(int j=0;j<vertexnumber;j++){//this loop creates an vector which airport has how many nonvisited neighbour
            if(visited[j]==true){
                for(int k=0;k<VertexList[j].edges.size();k++){
                int myendvertexindex=VertexList[j].edges[k].endVertexIndex;
                if(visited[myendvertexindex]==false && myvisited[myendvertexindex]==false){
                    nonvisitedcounter[j]++;
                    myvisited[myendvertexindex]=true;
                }
                
                }
            }
        }
        int airportmaxnonind=0;
        int tmpselectedvalue;
        int nextindex=0;
        for(;nextindex<vertexnumber;nextindex++){//to find index of "This airport should have the maximum non-visited neighbors."
            tmpselectedvalue= nonvisitedcounter[airportmaxnonind];
            if(nonvisitedcounter[nextindex]>tmpselectedvalue){
                airportmaxnonind=nextindex;
            }
        }
        int l=0;
        if(VertexList[airportmaxnonind].edges.size()==0){
            PrintSisterAirlinesDontCover(airportName);
            return;
        }
        for(;l<VertexList[airportmaxnonind].edges.size();l++){
            int vertexEndIndex=VertexList[airportmaxnonind].edges[l].endVertexIndex;//to check visited or not
            if(!containSisterHelper(tmpairlines,VertexList[airportmaxnonind].edges[l].name) && visited[vertexEndIndex]==false){
                break;
            }
            if(l==VertexList[airportmaxnonind].edges.size()-1){//it means we cannot find a new airline print error
                PrintSisterAirlinesDontCover(airportName);
                return;
            }
        }
        tmpairlines.push_back(VertexList[airportmaxnonind].edges[l].name);
        realtmpairlines.push_back(VertexList[airportmaxnonind].edges[l].name);//add this airline to the sisterairline list

        for(int m=0;m<vertexnumber;m++){//Visit every non-visited airport only using this airline. DFS
            if(visited[m]){
                mystack.push_back(m);
                while(!mystack.empty()){
                    int myvertex=mystack.back();
                    mystack.pop_back();
                    for(int n=0;n<VertexList[myvertex].edges.size();n++){//FOR LOOP BECAUSE WE ARE DEALING WITH MULTIGRAPHS
                        int myendvertexind=VertexList[myvertex].edges[n].endVertexIndex;
                        if(containSisterHelper(tmpairlines,VertexList[myvertex].edges[n].name) && visited[myendvertexind]==false ){
                            visited[myendvertexind]=true;
                            visitedcounter++;
                            mystack.push_back(myendvertexind);
                        }
                    }
                }
            }
        }
        
        
    
    }//END OF WHILE
    if(visitedcounter==previouscounter){
        PrintSisterAirlinesDontCover(airportName);
        return;
    }
    
    if(visitedcounter==vertexnumber){//it means we visited everyhere .HERE WE GO
        airlineNames=realtmpairlines;
        return;
    }
    
    
    
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    if(!(navigationMap.search_vertex(airportName))){
        return -1;
    }
    
    int depth=navigationMap.MaxDepthViaEdgeName(airportName,airlineName);
    return depth;
}