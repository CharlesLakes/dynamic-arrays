#include <bits/stdc++.h>
#include "../heap/heap.hpp"

#define PQ_PUSH(pq,node,cost)   pq.push({-cost,node})
#define PQ_TOP(pq)              (make_pair(-pq.top().first,pq.top().second))
#define PQ_POP(pq)              pq.pop()

using namespace std;

template <class container> 
vector<long long> dijkstra(vector<vector<pair<int,int>>> g, int s){
    int n = g.size();
    vector<long long> dist(n, 1e14);
    heap<container<pair<int,int>>,pair<int,int>> pq;

    dist[s] = 0;
    PQ_PUSH(s,s,0);

    while(!pq.empty()){
        auto data = PQ_TOP(pq);
        PQ_POP(pq);
        if(dist[data.second] != data.first) continue;

        for(pair<int,int> edge: g[data.second]){
            if(data.first + edge.second < dist[edge.first]){
                dist[edge.first] = data.first + edge.second;
                PQ_PUSH(pq,edge.first,dist[edge.first]);
            }
        }

    }

    return dist;

}