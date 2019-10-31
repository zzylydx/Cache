#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include<fstream>
#include <string>
#include <streambuf> 

using namespace std;

struct block{
	bool is_valid;//是否已经装入数据，false为空，true为满 
	int tag;//标志位（还是不是太懂） 
	void *data;
};

#include <vector> 

#define MEM_READ_TIME 100//操作内存的时间 
#define CACHE_READ_TIME 1//操作cache的时间 



class Cache{
public:
	
	int cache_size;//cache容量大小 
	int block_size;//块大小 
	int set_size;// 这是组的大小
	
	int set_count;//组的数量
	int hits,reads,writes,replaces,total,total_time;
	
	vector<struct block> blocks;//相当于定义了一个结构体的动态数组，blocks 
	
	void init(int p_cache_size,int p_block_size,int p_set_size){
		cache_size = p_cache_size;
		block_size = p_block_size;
		set_size = p_set_size;
		
		total = 0;//读和写总的次数 
		reads = 0;//读的次数 
		writes = 0;//写的次数 
		hits = 0;//命中次数 
		replaces = 0;//替换次数 （替换策略中的随机替换次数） 
		
		total_time = 0;//访问的总时间 
		
		set_count = cache_size / block_size / set_size;//组相连映射 
		for(int i = 0;i < set_count * set_size;i++){
			struct block b;
			
			b.is_valid = false;
			b.tag = -1;
			
			blocks.push_back(b);//在动态数组的最后添加一个块 
		}
			
	}
	
	void read(int addr){//实现读操作 
		cout << "-- read from " << addr ;
		
		reads++;
		total++;
		
		//found the block
		int tag = addr / block_size ;
		
		int index = (tag/ set_size) % set_count ; 
		
		total_time += CACHE_READ_TIME;//从cache中读取数据的时间 
		
		for(int i = index;i < index + set_size;i++){
			if(blocks[i].is_valid && blocks[i].tag == tag){//hit,命中 
				hits++;
				cout << " read complete!hit!" << endl;
				return;
			}
		}
		
		cout << " miss" << endl;//未命中，则需要从内存中加载 
		
		//load from memory
		total_time += MEM_READ_TIME;
		
		for(int i = index;i < index + set_size;i++){//把从内存中取出的数据放到cache中 
			if(!blocks[i].is_valid){//found a free block
				blocks[i].is_valid = true;//存有数据了 
				blocks[i].tag = tag;
				
				cout << " -- --- --- memory loaded " << endl;
				return;
			}
//			else{
//				cout<<"memory fail!"<<endl;
//			}
		}
		
		//random replace
		cout << "cache is full! Need replace!" << endl;//在放入cache时缓存已经满了，这时候需要采用置换策略 
		int r = rand() % set_size + index;//随机替换策略，将数据放入block中 
		
		replaces++;
		
		blocks[r].is_valid = true;
		blocks[r].tag = tag;
		cout<<"replace complete!" <<endl;
		return;
			
	}
	
	void write(int addr){//实现写操作 
		cout << "-- write to " << addr << endl;
		
		writes++;
		total++;
		
		//找到要写的cache
		 int tag=addr/block_size;
		 
		 int index=(tag/ set_size) % set_count ;
		 
		 total_time+=CACHE_READ_TIME;
		 
		 for(int i = index;i < index + set_size;i++){
			if(!blocks[i].is_valid){//is_valid为false则为空，可以写入数据 
				blocks[i].tag=tag;
				blocks[i].is_valid=true;
				hits++;
				cout << "write complete！hit!" << endl;
				return;
			}
		}
		
		cout<<"aim_cache is full! miss!"<<endl;
		
//		index++;//采取写不分配策略，直接写入下一个缓冲区 
		
//		for(int i = index;i < index + set_size;i++){
//			if(!blocks[i].is_valid){//is_valid为false则为空，可以写入数据 
//				blocks[i].tag=tag;
//				blocks[i].is_valid=true;
//				cout << "write complete！" << endl;
//				return;
//			}
//		}
		
//		func_find(index,tag);	
		
	}
//	void func_find(int index,int tag){
//		for(int i = index;i < index + set_size;i++){
//			if(!blocks[i].is_valid){//is_valid为false则为空，可以写入数据 
//				blocks[i].tag=tag;
//				blocks[i].is_valid=true;
//				cout << "write_no complete！" << endl;
//				return;
//			}
//		}
//		index++;
//		func_find(index,tag);
//	}
	
	void show_statistic(){//将最终的计算结果输出 
		cout << "Cache size:" << cache_size << endl;
		cout << "block size:" << block_size << endl;
		cout << "set size:" << set_size << endl << endl;
		
		cout << "total r/w:" << total << endl;
		cout << "hit r/w:" << hits << endl ;
		cout << "replace :" << replaces << endl << endl;
				
		if(total == 0) total = 1;
		
		cout << "hit rate:" <<( hits*1.0/total)*100 << "%"<<endl;
		cout<<"miss rate:"<<((total-hits)*1.0/total)*100<<"%"<<endl<<endl;
		
		cout << "total time:" << total_time << endl;
		
		cout << "average time: " << total_time * 1.0 / total << endl;
		
	}
	
	
	Cache(int p_cache_size,int p_block_size,int p_set_size)	{
		init(p_cache_size,p_block_size,p_set_size);
	}
	
	~Cache(){
		//release the blocks
		if(set_count){
			blocks.clear();
		}
	}
	

};

//string Read_str(string filepath)
//{
//    ifstream infile;
//    infile.open(filepath);
//    //打开失败，路径不正确
//    if(!infile)
//        cout << "Open File Fail!" << endl;
//    //读取文本内容到字符串
//    string readStr((std::istreambuf_iterator<char>(infile)),  std::istreambuf_iterator<char>()); 
//    
//    return readStr;
//}

int main() {
	//init cache
	Cache c(128,2,64);
	
	char rw;//read or write
	int addr;//
	 
	//load memory trace file,加载trace文件 
	freopen("C:\\gcc.txt","r",stdin);
	while(scanf("%x %c",&addr,&rw)==2){
			
		//cin	>> rw >> addr;
				
		/*
		if(rw == -1 && addr == -1)
			break;
		*/	
		if(rw == 'R')
			c.read(addr);
		else if(rw == 'W')
			c.write(addr);
	} 
	fclose(stdin);
	
	c.show_statistic();	
	
	return 0;
}
