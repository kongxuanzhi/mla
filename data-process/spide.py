# PART1：数据爬取

# coding: utf-8
import pandas as pd
import requests
import random
import json
import time
header={
    'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36',
    'Connection':'keep-alive'
}
cookies = '_lxsdk_cuid=1651950e6dfc8-02b4e01b2f82ec-6b1b1279-100200-1651950e6dfc8; _lxsdk=FA2EEC709AFE11E883FE1F23FDF\
          D6ADBB39CE732DF384D9DADFD1909B110AD64; v=3; _lx_utm=utm_source%3DBaidu%26utm_medium%3Dorganic; __mta=121983668.153372\
          8057235.1533731802598.1533731803845.10'
cookie = {}
for line in cookies.split(';'):
    name, value = cookies.strip().split('=', 1)
    cookie[name] = value
love_apartment = pd.DataFrame(columns=['data','score','city','comment','nick'])
for i in range(0,1000):
    j = random.randint(1,1000)
    print(str(i)+' '+str(j))
    try:
        time.sleep(2)
        url = 'https://m.maoyan.com/mmdb/comments/movie/1175253.json?_v_=yes&offset='+str(j)
        html = requests.get(url=url, cookies=cookie,headers=header).content
        data = json.loads(html.decode('utf-8'))['cmts']
        for item in data:
            love_apartment = love_apartment.append({'data':item['time'].split(' ')[0],'city':item['cityName'],
                                                    'score':str(item['score']),'comment':item['content'],'nick':item['nick']},
                                                   ignore_index=True)
        love_apartment.to_csv('爱情公寓.csv',index=False)
    except:
        continue