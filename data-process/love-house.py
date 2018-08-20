# https://mp.weixin.qq.com/s?__biz=MjM5MDAxNjkyMA==&mid=2650742450&idx=4&sn=7e6bce39a7d39a321749f7db28244db7&chksm=be408a4589370353c21fe4b6bfcaeb4334b08665e12f17d85fc061a9ba4a6eb58b727db8533d&mpshare=1&scene=1&srcid=0816qHd1LlD4ASnUuLVq8SWR&pass_ticket=PCcjAWFgpMgtnTa2tEbGg9B0iodePWflL5mmZxy05yykEi%2BOw2qlYefL0VGGKbAA#rd
# http://pyecharts.org/#/zh-cn/datasets
# PART2：数据分析
# 图表神器pyecharts的技术文档的网址：http://pyecharts.org/#/zh-cn/charts
# HeatMap（热力图）、Bar（柱状图/条形图）、Line（折线/面积图）、Geo（地理坐标系）、Overlap 类。

# 词云生成wordcloud使用网址：
# https://blog.csdn.net/fly910905/article/details/77763086

import pandas as pd
from pyecharts import Bar, Line,Overlap,Geo
import jieba
from wordcloud import WordCloud,ImageColorGenerator
import matplotlib.pyplot as plt
from collections import Counter

love_apartment_com = pd.read_csv('爱情公寓.csv',encoding='utf-8')
grouped = love_apartment_com.groupby(['city'])
grouped_pct = grouped['score']

# 爱情公寓全国观影图  pyecharts地图资源不全面 对Excel表筛除无效数据
city_com = grouped_pct.agg(['mean', 'count'])
city_com.reset_index(inplace=True)
city_com['mean'] = round(city_com['mean'], 2)
data = [(city_com['city'][i], city_com['count'][i]) for i in range(0, city_com.shape[0])]
geo = Geo('《爱情公寓》全国热力图', title_color="#fff",title_pos="center", width=1200, height=600, background_color='#404a59')
attr, value = geo.cast(data)
geo.add("", attr, value, type="heatmap",  is_visualmap=True, visual_range=[0, 20],visual_text_color="#fff")
geo.render('爱情公寓全国观影图.html')

# 主要城市评分and评论数

city_main = city_com.sort_values('count',ascending=False)[0:20]
attr = city_main['city']
v1 = city_main['count']
v2 = city_main['mean']
line = Line("主要城市评分")
line.add("城市", attr, v2, is_stack=True, xaxis_rotate=30, yaxis_min=0, mark_point=["max", "min"], xaxis_interval=0,
         line_color='lightblue', line_width=4, mark_point_textcolor='black', mark_point_color='lightblue', is_splitline_show=False)

bar = Bar("主要城市评论数")
bar.add("城市", attr, v1, is_stack=True, xaxis_rotate=30, yaxis_min=0, xaxis_interval=0, is_splitline_show=False)
overlap = Overlap()
overlap.add(bar)
overlap.add(line, yaxis_index=1, is_add_yaxis=True)
overlap.render('主要城市评分and评论数.html')

#城市评分
city_score = city_main.sort_values('mean',ascending=False)[0:20]
attr = city_score['city']
v1 = city_score['mean']
line = Line('爱情公寓主要城市评分')
line.add("城市", attr, v1, is_stack=True, xaxis_rotate=30, yaxis_min=0, mark_point=["min", "max"], xaxis_interval=0,
         line_color='lightblue', line_width=4, mark_point_textcolor='black', mark_point_color='lightblue', is_splitline_show=True)
line.render('爱情公寓主要城市评分.html')

# 词云
love_apartment_str = ' '.join(love_apartment_com['comment'])
words_list = []
word_generator = jieba.cut_for_search(love_apartment_str)
for word in word_generator:
    words_list.append(word)
words_list = [k for k in words_list if len(k)>1]
back_color = plt.imread('爱心.jpg')
wc = WordCloud(background_color='white',
               mask=back_color,
            #    font_path='C:\Windows\Fonts\STZHONGS.TTF',
               max_words=2000,
               max_font_size=250,
               random_state=30,
               )
love_apartment_count = Counter(words_list)
wc.generate_from_frequencies(love_apartment_count)
image_colors = ImageColorGenerator(back_color)
plt.figure()
plt.imshow(wc.recolor(color_func=image_colors))
plt.axis('off')
print('生成词云成功!')
wc.to_file('love_apartment.jpg')