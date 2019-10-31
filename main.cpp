#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include<fstream>
#include <string>
#include <streambuf> 

using namespace std;

struct block{
	bool is_valid;//�Ƿ��Ѿ�װ�����ݣ�falseΪ�գ�trueΪ�� 
	int tag;//��־λ�����ǲ���̫���� 
	void *data;
};

#include <vector> 

#define MEM_READ_TIME 100//�����ڴ��ʱ�� 
#define CACHE_READ_TIME 1//����cache��ʱ�� 



class Cache{
public:
	
	int cache_size;//cache������С 
	int block_size;//���С 
	int set_size;// ������Ĵ�С
	
	int set_count;//�������
	int hits,reads,writes,replaces,total,total_time;
	
	vector<struct block> blocks;//�൱�ڶ�����һ���ṹ��Ķ�̬���飬blocks 
	
	void init(int p_cache_size,int p_block_size,int p_set_size){
		cache_size = p_cache_size;
		block_size = p_block_size;
		set_size = p_set_size;
		
		total = 0;//����д�ܵĴ��� 
		reads = 0;//���Ĵ��� 
		writes = 0;//д�Ĵ��� 
		hits = 0;//���д��� 
		replaces = 0;//�滻���� ���滻�����е�����滻������ 
		
		total_time = 0;//���ʵ���ʱ�� 
		
		set_count = cache_size / block_size / set_size;//������ӳ�� 
		for(int i = 0;i < set_count * set_size;i++){
			struct block b;
			
			b.is_valid = false;
			b.tag = -1;
			
			blocks.push_back(b);//�ڶ�̬�����������һ���� 
		}
			
	}
	
	void read(int addr){//ʵ�ֶ����� 
		cout << "-- read from " << addr ;
		
		reads++;
		total++;
		
		//found the block
		int tag = addr / block_size ;
		
		int index = (tag/ set_size) % set_count ; 
		
		total_time += CACHE_READ_TIME;//��cache�ж�ȡ���ݵ�ʱ�� 
		
		for(int i = index;i < index + set_size;i++){
			if(blocks[i].is_valid && blocks[i].tag == tag){//hit,���� 
				hits++;
				cout << " read complete!hit!" << endl;
				return;
			}
		}
		
		cout << " miss" << endl;//δ���У�����Ҫ���ڴ��м��� 
		
		//load from memory
		total_time += MEM_READ_TIME;
		
		for(int i = index;i < index + set_size;i++){//�Ѵ��ڴ���ȡ�������ݷŵ�cache�� 
			if(!blocks[i].is_valid){//found a free block
				blocks[i].is_valid = true;//���������� 
				blocks[i].tag = tag;
				
				cout << " -- --- --- memory loaded " << endl;
				return;
			}
//			else{
//				cout<<"memory fail!"<<endl;
//			}
		}
		
		//random replace
		cout << "cache is full! Need replace!" << endl;//�ڷ���cacheʱ�����Ѿ����ˣ���ʱ����Ҫ�����û����� 
		int r = rand() % set_size + index;//����滻���ԣ������ݷ���block�� 
		
		replaces++;
		
		blocks[r].is_valid = true;
		blocks[r].tag = tag;
		cout<<"replace complete!" <<endl;
		return;
			
	}
	
	void write(int addr){//ʵ��д���� 
		cout << "-- write to " << addr << endl;
		
		writes++;
		total++;
		
		//�ҵ�Ҫд��cache
		 int tag=addr/block_size;
		 
		 int index=(tag/ set_size) % set_count ;
		 
		 total_time+=CACHE_READ_TIME;
		 
		 for(int i = index;i < index + set_size;i++){
			if(!blocks[i].is_valid){//is_validΪfalse��Ϊ�գ�����д������ 
				blocks[i].tag=tag;
				blocks[i].is_valid=true;
				hits++;
				cout << "write complete��hit!" << endl;
				return;
			}
		}
		
		cout<<"aim_cache is full! miss!"<<endl;
		
//		index++;//��ȡд��������ԣ�ֱ��д����һ�������� 
		
//		for(int i = index;i < index + set_size;i++){
//			if(!blocks[i].is_valid){//is_validΪfalse��Ϊ�գ�����д������ 
//				blocks[i].tag=tag;
//				blocks[i].is_valid=true;
//				cout << "write complete��" << endl;
//				return;
//			}
//		}
		
//		func_find(index,tag);	
		
	}
//	void func_find(int index,int tag){
//		for(int i = index;i < index + set_size;i++){
//			if(!blocks[i].is_valid){//is_validΪfalse��Ϊ�գ�����д������ 
//				blocks[i].tag=tag;
//				blocks[i].is_valid=true;
//				cout << "write_no complete��" << endl;
//				return;
//			}
//		}
//		index++;
//		func_find(index,tag);
//	}
	
	void show_statistic(){//�����յļ�������� 
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
//    //��ʧ�ܣ�·������ȷ
//    if(!infile)
//        cout << "Open File Fail!" << endl;
//    //��ȡ�ı����ݵ��ַ���
//    string readStr((std::istreambuf_iterator<char>(infile)),  std::istreambuf_iterator<char>()); 
//    
//    return readStr;
//}

int main() {
	//init cache
	Cache c(128,2,64);
	
	char rw;//read or write
	int addr;//
	 
	//load memory trace file,����trace�ļ� 
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
