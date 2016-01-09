#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include<cstdlib>
using namespace std;

class point{
public:
	point(int a = 0, int b = 0){
		x = a;
		y = b;
		istarget = 0;
		isobstacle = 0;
		isvisit = 0;
	}

	int x, y, d;
	bool istarget, isobstacle, isvisit;
	vector<point*> parent;
	vector<point*> neighbor;

};


int m, n;
ofstream fout("output_new.txt");

void swap_point(point* &a,point* &b){
	point* temp;
	temp = a;
	a = b;
	b = temp;
};


void min_heapify(vector<point*>& in, int i){
	int left, right;
	left = 2*i + 1;
	right = 2*i + 2;
	int heap_size = in.size();
	int smallest;
	if(left <= heap_size-1 && in[left]->d < in[i]->d) smallest = left;
	else smallest = i;
	if(right <= heap_size-1&& in[right]->d < in[smallest]->d) smallest = right;

	if(smallest != i){
		swap_point(in[i], in[smallest]);
		min_heapify(in, smallest);
	}
};


void build_min_heap(vector<point*>& in){
	int i;
	for(i = in.size()/2 -1 ; i >=0 ; --i){
		min_heapify(in, i);
	}
};


point* heap_minmum(vector<point*>& in){
	return in[0];
};

point* heap_extract_min(vector<point*>& in){
	int heap_size = in.size();
	if(heap_size < 1) cout << "heap underflow\n" ;
	point* min;
	min = in[0];
	in[0] = in[heap_size-1];
	in.pop_back();
	min_heapify(in,0);
	return min;
};

int find_parent(int i){
	if(i%2 == 0) return(i - 2) / 2;
	else return(i - 1) / 2;
};

void heap_decrease_key(vector<point*> &in, int key, point* v){
	int heap_size = in.size();
	if(key > v->d){
		cout << "new key > current key" << endl;
		return void();
	} 
	v->d = key;
	build_min_heap(in);
};

void initialize(vector< vector<point*> >& all , point start){
	for( int i = 0 ; i < m ; ++i){
		for( int j = 0 ; j < n ; ++j){
			all[i][j]->d = 1e9;
		}
	}
	all[start.x-1][start.y-1]->d = 0;
};

int weight(point* u , point* v){
	if(u->istarget == 1 || v->istarget == 1) return 1;
	else return 2;
};

void relax(vector<point*> &heap, point* &u, point* &v){
		//cout << u->x << ' ' <<u->y << "->" << v->x << ' ' << v->y << "weight : " << weight(u,v) <<endl;
	if(v->d > u->d + weight(u,v)) {
		v->d = u->d + weight(u,v);
		v->parent.push_back(u);
		heap_decrease_key(heap, v->d, v);
	}
};


void dijkstra(vector< vector<point*> > &all, point start){
	initialize(all, start);
	vector<point*> heap;
	for( int i = 0 ; i < m ; ++i){
		for( int j = 0 ; j < n ; ++j){	
			heap.push_back(all[i][j]);
		}
	}
	build_min_heap(heap);
	point* u;
	vector<point*>::iterator itp;
	while(!heap.empty()){
		for (std::vector<point*>::iterator it = heap.begin();it != heap.end(); ++it)
		fout << (*it)->x << ' ' << (*it)->y << ' ' <<  (*it)->d << ' ' <<endl;
	fout <<endl;
		
		u = heap_extract_min(heap);
		cout << '(' << u->x << ',' << u->y  << ')'  << ' ' <<  u->d   <<"   "<<endl;
		
		for(itp = u->neighbor.begin() ; itp != u->neighbor.end() ; ++itp) 
			relax(heap, u ,(*itp));
	}

};

int main(){
	ifstream fin("infile.txt");
	if(!fin){
		cout << "cannot open file\n";
		return 0;
	}

	int tempx, tempy;
	fin >> m >> n ;
	fin >> tempx >> tempy;
	point start(tempx, tempy);
	fin >> tempx >> tempy;
	point end(tempx, tempy);

	int i, j;
	vector< vector<point*> > all;

	all.resize(m);
	for( i = 0 ; i < m ; ++ i){
		all[i].resize(n);
	}

	for ( i = 0 ; i < m ; ++i){
		for( j = 0 ; j < n ; ++j){
			point* temp = new point(i + 1, j + 1);
			all[i][j] = temp;
		}
	}
	vector<  vector<point*> >::iterator itx;
	vector <point*> ::iterator ity;
	int t_num, o_num;
	fin >> t_num;
	for( i = 0 ; i < t_num ; ++i){
		fin >> tempx >> tempy;
		all[tempx-1][tempy-1]->istarget =1;
	}

	fin >> o_num;
	for(i = 0 ; i < o_num ; ++i){
		fin >> tempx >>tempy;
		all[tempx-1][tempy-1]->isobstacle =1;
	}

	for( i = 0 ; i < m ; ++i){
		for( j = 0 ; j < n ; ++j){
			
			if(i == 0) {
				if( j + 1 < n){
					if(all[i][j+1]->isobstacle == 0) all[i][j]->neighbor.push_back(all[i][j+1]);
					if(all[i][j]->isobstacle == 0) all[i][j+1]->neighbor.push_back(all[i][j]);
				}
			}

			else{
				if( j + 1 < n){
					if(all[i][j+1]->isobstacle == 0) all[i][j]->neighbor.push_back(all[i][j+1]);
					if(all[i][j]->isobstacle == 0) all[i][j+1]->neighbor.push_back(all[i][j]);
				}

				if( i - 1 >= 0 ){
					if(all[i-1][j]->isobstacle == 0) all[i][j]->neighbor.push_back(all[i-1][j]);
					if(all[i][j]->isobstacle == 0) all[i-1][j]->neighbor.push_back(all[i][j]);
				}

			}

		}
	}
/*	vector <point*> ::iterator itp;
	for(i = 0 ; i < m  ; ++i){
		for(j = 0 ;  j < n ; ++j){
			cout << '(' << all[i][j]->x << ',' << all[i][j]->y  << ')' << ':';
			cout << all[i][j]->neighbor.size() << ' ';
			for( itp = all[i][j]->neighbor.begin() ; itp != all[i][j]->neighbor.end() ; ++itp){
				cout << '(' << (*itp)->x << ',' << (*itp)->y  << ')' << ' ';
			}

			cout << " istarget: " << all[i][j]->istarget ;
			cout << endl;

		}
	}*/

/*	for ( itx = all.begin() ; itx != all.end() ; ++itx){
		for(ity = (*itx).begin() ; ity != (*itx).end() ; ++ity){
			cout << (*ity)->x << ' ' << (*ity)->y << " istarget : " <<  (*ity)->istarget <<  " isobstacle: " <<  (*ity)->isobstacle << endl;
		}
	}
*/
/*	initialize(all, start);
	vector<point*> heap;
	for( int i = 0 ; i < m ; ++i){
		for( int j = 0 ; j < n ; ++j){			
			heap.push_back(all[i][j]);
		}
	}

	build_min_heap(heap);
	for (std::vector<point*>::iterator it = heap.begin();it != heap.end(); ++it)
		fout << (*it)->x << ' ' << (*it)->y << ' ' <<  (*it)->d << endl;
	heap_extract_min(heap);
	heap.pop_back();
	fout << endl;
	for (std::vector<point*>::iterator it = heap.begin();it != heap.end(); ++it)
		fout << (*it)->x << ' ' << (*it)->y << ' ' <<  (*it)->d << endl;
	fout << endl;
	heap_decrease_key(heap, 10, all[5][3]);
	for (std::vector<point*>::iterator it = heap.begin();it != heap.end(); ++it)
		fout << (*it)->x << ' ' << (*it)->y << ' ' <<  (*it)->d << endl;
	heap_extract_min(heap);
	heap.pop_back();
	fout << endl;
	for (std::vector<point*>::iterator it = heap.begin();it != heap.end(); ++it)
		fout << (*it)->x << ' ' << (*it)->y << ' ' <<  (*it)->d << endl;
	fout << endl;
	heap_decrease_key(heap, 2, all[2][2]);
	for (std::vector<point*>::iterator it = heap.begin();it != heap.end(); ++it)
		fout << (*it)->x << ' ' << (*it)->y << ' ' <<  (*it)->d << endl;*/

	dijkstra(all, start);
	return 0;
}
