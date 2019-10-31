# Cache-
<h2>有关Cache模拟器的部分功能的实现</h2>
<h3>采用的相关策略和操作</h3>
    <h4>写操作</h4>
        1、使用写回策略：只修改cache的内容不立即写入主存，只当此行被换出时才写回主存；
        2、写不分配：发生写入缺失时只修改下一级存储器中的块；对一cache行的多次写命中都在cache中快速完成修改， 只是需被替换时才写回速度较慢的主存，减少了访问主存的次数从而提高了效率。
    <h4>映射方式</h4>
        多路组映射
    <h4>置换策略</h4>
        我采用了最简单的随机置换：从Cache中读取数据时，数据未在Cache中需要从内存中读入，如果在读入时Cache已经满了，则需要采用置换策略进行置换，即随机选取一个组进行置换数据。
<h3>关键代码</h3>
    <h4>可见参数解释</h4>
          参数名&nbsp&nbsp&nbsp&nbsp      含义	&nbsp&nbsp&nbsp     值<br/>
          cache_size	&nbsp&nbsp&nbsp&nbsp缓存的大小&nbsp&nbsp&nbsp 8/16/32/64/128/256/512/1024<br/>
          block_size	&nbsp&nbsp&nbsp&nbsp块的大小&nbsp&nbsp&nbsp	  4/2<br/>
          set_size	  &nbsp&nbsp&nbsp&nbsp组的大小	&nbsp&nbsp  &nbsp2<br/>
          set_count	  &nbsp&nbsp&nbsp&nbsp组的数量	 &nbsp&nbsp&nbsp Cache_size/block_size/set_size<br/>
          hits	      &nbsp&nbsp&nbsp&nbsp命中次数	 &nbsp&nbsp&nbsp待输出<br/>
          reads	      &nbsp&nbsp&nbsp&nbsp读操作的次数	&nbsp&nbsp&nbsp待输出<br/>
          writes	   &nbsp&nbsp&nbsp&nbsp 写操作的次数	&nbsp&nbsp&nbsp待输出<br/>
          replaces	  &nbsp&nbsp&nbsp&nbsp替换次数	   &nbsp&nbsp&nbsp 待输出<br/>
          total	      &nbsp&nbsp&nbsp&nbsp读和写的总数	&nbsp&nbsp&nbsp待输出<br/>
          total_time&nbsp&nbsp&nbsp&nbsp	访问的总时间&nbsp&nbsp&nbsp	待输出<br/>
          Is_valid	  &nbsp&nbsp&nbsp&nbsp块有效位	&nbsp&nbsp&nbsp  False空True满<br/>
          tag	        &nbsp&nbsp&nbsp&nbsp块标识位	 &nbsp&nbsp&nbsp addr / block_size<br/>
          data	      &nbsp&nbsp&nbsp&nbsp块内数据	 &nbsp&nbsp&nbsp 无<br/>
          MEM_READ_TIME&nbsp&nbsp&nbsp&nbsp	访问内存时间&nbsp&nbsp&nbsp	100<br/>
          CACHE_READ_TIME	&nbsp&nbsp&nbsp&nbsp访问缓存时间	&nbsp&nbsp&nbsp1<br/>
          addr	      &nbsp&nbsp&nbsp&nbsp地址	      &nbsp&nbsp&nbsp待输入<br/>
    <h4>加载trace的源码</h4>
          freopen("C:\\gcc.txt","r",stdin);<br/>
          while(scanf("%x %c",&addr,&rw)==2){	<br/>
            if(rw == 'R')<br/>
              c.read(addr);<br/>
            else if(rw == 'W')<br/>
              c.write(addr);<br/>
          } <br/><br/>
          fclose(stdin);<br/>
          主要是使用了输入输出重定向的相关代码，还有无法打开trace文件，所以我将数据复制到txt中进行测试。<br/>
          <h2>已上传trace文件，可自行下载修改参数进行测试。</h2>

